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


WindowManager::WindowManager()
{
    displayPtr = shared_ptr<Display>(XOpenDisplay(0), Free_XCloseDisplay());
    if (!displayPtr) throw runtime_error("Display is not open");
    display = displayPtr.get();
    root = XDefaultRootWindow(display);

    keyGrabber = make_shared<KeyGrabber>(this, workspaceCount());
    fontHelper = make_shared<FontHelper>(display);

    for (int i = 0; i < workspaceCount(); ++i)
        workspaces.emplace_back(this);
    currentWorkspace = &workspaces[0];
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
    if (currentWindow && currentWindow->windowMode == WindowMode::Tiled) {
        WmContainer *parent = currentWindow->parent();
        if (!parent)
            return;

        switch (parent->containerType()) {
            case WmFrameType::Splitter:
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
                    splitter->add(newSplitter, window);
                    splitter->remove(window);
                    newSplitter->add(window->shared());
                    newSplitter->splitRatio(window->splitRatio());
                    splitter->realign();
                }
                break;
        }
    }
}

WmWindow* WindowManager::addWindow(Window window) {
    shared_ptr<WmWindow> wPtr(make_shared<WmWindow>(this, window));
    WmWindow* w = wPtr.get();
    windows.insert(make_pair(w->window, wPtr));
    windows.insert(make_pair(w->frame, wPtr));

    currentWorkspace->addWindow(w);

    stringstream ss;
    ss << desktop.x << ":" << desktop.w << ":" << desktop.y << ":" << desktop.h;
    addDebugText(ss.str());

    XWindowAttributes attributes;
    XGetWindowAttributes(display, w->frame, &attributes);

    w->selectDefaultInput();
    w->setDefaultEventMask();
    w->show();

    if (!currentWindow)
        setCurrentWindow(w);

    w->relocate(desktop.x + (desktop.w - attributes.width) / 2,
            desktop.y + (desktop.h - attributes.height) / 2,
            attributes.width, attributes.height);

    return w;
}

void WindowManager::removeDestroyedWindow(Window w) {
    WmWindow* destroyedWindow;
    if (currentWindow && currentWindow->window == w)
        destroyedWindow = currentWindow;
    else
        destroyedWindow = findWindow(w);

    if (!destroyedWindow)
        return;

    destroyedWindow->window = 0;

    removeWindow(destroyedWindow);
}

void WindowManager::removeWindow(WmWindow* w) {
    if (w == currentWorkspace->lastActiveTiledWindow)
        currentWorkspace->lastActiveTiledWindow = 0; // @TODO: Select next best tiled window

    w->workspace->removeWindow(w); // remove from tiling

    // Cleanup struct
    auto it = windows.find(w->window);
    if (it != windows.end())
        windows.erase(it);
    auto it2 = windows.find(w->frame);
    if (it2 != windows.end())
        windows.erase(it2);

    if (currentWindow == w) {
        currentWindow = 0;
        selectNewCurrentWindow();
    }
}

void WindowManager::setCurrentWindow(Window window) {
    setCurrentWindow(findWindow(window));
}

void WindowManager::setCurrentWindow(WmWindow* window) {
    if (currentWindow)
        currentWindow->setActive(false);

    currentWindow = window;

    stringstream ss;
    ss << "CURRENT WINDOW 0x" << hex << currentWindow;
    addDebugText(ss.str());

    if (currentWindow) {
        currentWindow->setActive(true);
        if (currentWindow->windowMode == WindowMode::Tiled)
            currentWorkspace->lastActiveTiledWindow = currentWindow;
    }
    // @TODO: Set _NET_ACTIVE_WINDOW
}

void WindowManager::changeWorkspace(int number) {
    if (currentWorkspace == &workspaces[number])
        return; // same workspace
    setCurrentWindow(nullptr);

    currentWorkspace->hide();
    currentWorkspace = &workspaces[number];
    if (moveWindow) {
        moveWindow->workspace->removeWindow(moveWindow);
        currentWorkspace->addWindow(moveWindow);
    }
    currentWorkspace->show();

    selectNewCurrentWindow();
    // @TODO: update _NET_CURRENT_DESKTOP, _NET_NUMBER_OF_DESKTOPS
}

void WindowManager::calculateDesktopSpace() {
    XWindowAttributes rAttr;
    XGetWindowAttributes(display, root, &rAttr);

    Geometry& d = desktop;
    d.x = rAttr.x;
    d.y = rAttr.y;
    d.w = rAttr.width;
    d.h = rAttr.height;

    // @TODO: docked, when: _NET_WM_WINDOW_TYPE => _NET_WM_WINDOW_TYPE_DOCK
    for (shared_ptr<WmWindow>& w : dockedWindows) {
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

Atom WindowManager::getAtom(const std::string& protocol) {
    return XInternAtom(display, protocol.c_str(), false);
}

void WindowManager::selectNewCurrentWindow() {
    if (currentWorkspace->windows.size() > 0)
        setCurrentWindow(currentWorkspace->windows.front());
    else
        setCurrentWindow(nullptr);
}

void WindowManager::spawn(const std::string& cmd, char *const argv[]) {
    addDebugText(cmd);
    pid_t pid = fork();
    if (pid == 0) {
        execv(cmd.c_str(), argv);
        exit(0);
    }
}

WmWindow* WindowManager::findWindow(Window window) {
    auto it = windows.find(window);
    if (it == windows.end())
        return 0;
    return it->second.get();
}