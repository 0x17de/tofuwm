#include <iostream>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <unistd.h>
#include <sstream>
#include <containers/wmsplitter.h>

#include "windowmanager.h"
#include "display.h"


using namespace std;


WindowManager::WindowManager(const Options* const options)
:
    options_(options)
{
    displayPtr_ = shared_ptr<Display>(XOpenDisplay(0), Free_XCloseDisplay());
    if (!displayPtr_) throw runtime_error("Display is not open");
    display = displayPtr_.get();
    root = XDefaultRootWindow(display);

    keyGrabber = make_shared<KeyGrabber>(this, workspaceCount());
    fontHelper = make_shared<FontHelper>(display);

    for (int i = 0; i < workspaceCount(); ++i)
        workspaces_.emplace_back(this);
    currentWorkspace_ = &workspaces_[0];
}

WindowManager::~WindowManager() {

}

void WindowManager::selectInput(int mask) {
    XSelectInput(display, root, mask);
}

void WindowManager::selectNoInput() {
    selectInput(0);
}

void WindowManager::selectDefaultInput() {
    selectInput(ResizeRedirectMask | SubstructureRedirectMask | PropertyNotify);
}

void WindowManager::changeSplitterDirectionOfWindow(WmWindow *window) {
    if (currentWindow_ && currentWindow_->windowMode == WindowMode::Tiled) {
        WmContainer *parent = currentWindow_->parent();
        if (!parent)
            return;

        switch (parent->containerType()) {
            case WmFrameType::Splitter: {
                WmSplitter *splitter = (WmSplitter *) parent;

                WmSplitterType newType;
                if (splitter->splitterType() == WmSplitterType::Horizontal)
                    newType = WmSplitterType::Vertical;
                else
                    newType = WmSplitterType::Horizontal;

                if (splitter->size() == 1)
                    splitter->splitterType(newType);
                else {
                    shared_ptr<WmSplitter> newSplitter(make_shared<WmSplitter>(newType));
                    currentWindow_->workspace->containers.push_back(newSplitter);
                    splitter->add(newSplitter.get(), window);
                    splitter->remove(window);
                    newSplitter->add(window);
                    newSplitter->splitRatio(window->splitRatio());
                    splitter->realign();
                }
                break;
            }
            // Not used - to suppress warnings
            case WmFrameType::Frame:
            case WmFrameType::Window:
                break;
        }
    }
}

void WindowManager::stop() {
    running_ = false;
    stopAction_ = WmStopAction::Stop;
}

void WindowManager::reload() {
    running_ = false;
    stopAction_ = WmStopAction::Reload;
}

void WindowManager::restart() {
    running_ = false;
    stopAction_ = WmStopAction::Restart;
}

WmWindow* WindowManager::addWindow(Window window) {
    shared_ptr<WmWindow> wPtr(make_shared<WmWindow>(this, window));
    WmWindow* w = wPtr.get();
    windows_.insert(make_pair(w->window, wPtr));
    windows_.insert(make_pair(w->frame, wPtr));

    currentWorkspace_->addWindow(w);

    stringstream ss;
    ss << desktop_.x << ":" << desktop_.w << ":" << desktop_.y << ":" << desktop_.h;
    addDebugText(ss.str(), LogLevel::Verbose);

    XWindowAttributes attributes;
    XGetWindowAttributes(display, w->frame, &attributes);

    if (!currentWindow_)
        setCurrentWindow(w);

    w->relocate(desktop_.x + (desktop_.w - attributes.width) / 2,
            desktop_.y + (desktop_.h - attributes.height) / 2,
            attributes.width, attributes.height);

    return w;
}

void WindowManager::removeDestroyedWindow(Window w) {
    WmWindow* destroyedWindow;
    if (currentWindow_ && currentWindow_->window == w)
        destroyedWindow = currentWindow_;
    else
        destroyedWindow = findWindow(w);

    if (!destroyedWindow)
        return;

    destroyedWindow->isMapped = false;
    removeWindow(destroyedWindow);
}

void WindowManager::removeWindow(WmWindow* w) {
    if (w == currentWorkspace_->lastActiveTiledWindow)
        currentWorkspace_->lastActiveTiledWindow = 0; // @TODO: Select next best tiled window

    w->workspace->removeWindow(w); // remove from tiling

    // Cleanup struct
    auto it = windows_.find(w->window);
    if (it != windows_.end())
        windows_.erase(it);
    auto it2 = windows_.find(w->frame);
    if (it2 != windows_.end())
        windows_.erase(it2);

    if (currentWindow_ == w) {
        currentWindow_ = 0;
        selectNewCurrentWindow();
    }
}

void WindowManager::setCurrentWindow(Window window) {
    setCurrentWindow(findWindow(window));
}

void WindowManager::setCurrentWindow(WmWindow* window) {
    if (currentWindow_)
        currentWindow_->setActive(false);

    currentWindow_ = window;

    stringstream ss;
    ss << "CURRENT WINDOW 0x" << hex << currentWindow_;
    addDebugText(ss.str(), LogLevel::Verbose);

    if (currentWindow_) {
        currentWindow_->setActive(true);
        if (currentWindow_->windowMode == WindowMode::Tiled)
            currentWorkspace_->lastActiveTiledWindow = currentWindow_;
    }
    // @TODO: Set _NET_ACTIVE_WINDOW
}

void WindowManager::changeWorkspace(int number) {
    if (currentWorkspace_ == &workspaces_[number])
        return; // same workspace
    setCurrentWindow(nullptr);

    currentWorkspace_->hide();
    currentWorkspace_ = &workspaces_[number];
    if (moveWindow_) {
        moveWindow_->workspace->removeWindow(moveWindow_);
        currentWorkspace_->addWindow(moveWindow_);
    }
    currentWorkspace_->show();

    selectNewCurrentWindow();
    // @TODO: update _NET_CURRENT_DESKTOP, _NET_NUMBER_OF_DESKTOPS
}

void WindowManager::calculateDesktopSpace() {
    XWindowAttributes rAttr;
    XGetWindowAttributes(display, root, &rAttr);

    Geometry& d = desktop_;
    d.x = rAttr.x;
    d.y = rAttr.y;
    d.w = rAttr.width;
    d.h = rAttr.height;

    // @TODO: docked, when: _NET_WM_WINDOW_TYPE => _NET_WM_WINDOW_TYPE_DOCK
    for (shared_ptr<WmWindow>& w : dockedWindows_) {
        XWindowAttributes wAttr;
        XGetWindowAttributes(display, w->window, &wAttr);

        if (wAttr.x == 0 && wAttr.width < rAttr.width
         && wAttr.width > d.x) {
            d.x = wAttr.width;
        }
        if (wAttr.y == 0 && wAttr.height < rAttr.height
         && wAttr.height > d.y) {
            d.y = wAttr.height;
        }
        if (wAttr.x + wAttr.width == rAttr.width
                && wAttr.x < d.w) {
            d.w = wAttr.x;
        }
        if (wAttr.y + wAttr.height == rAttr.height
                && wAttr.y < d.h) {
            d.h = wAttr.y;
        }
    }
}

std::string WindowManager::getAtomName(Atom atom) {
    string result;
    char* name = XGetAtomName(display, atom);
    result = name;
    XFree(name);
    return result;
}

Atom WindowManager::getAtom(const std::string& protocol) {
    return XInternAtom(display, protocol.c_str(), false);
}

void WindowManager::selectNewCurrentWindow() {
    if (currentWorkspace_->windows.size() > 0)
        setCurrentWindow(currentWorkspace_->windows.front());
    else
        setCurrentWindow(nullptr);
}

void WindowManager::spawn(const std::string& cmd, char *const argv[]) {
    addDebugText(cmd, LogLevel::Info);
    pid_t pid = fork();
    if (pid == 0) {
        execv(cmd.c_str(), argv);
        exit(0);
    }
}

WmWindow* WindowManager::findWindow(Window window) {
    auto it = windows_.find(window);
    if (it == windows_.end())
        return 0;
    return it->second.get();
}