#include <iostream>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <unistd.h>
#include <sstream>

#include "windowmanager.h"
#include "display.h"


using namespace std;


WindowManager::WindowManager()
{
    displayPtr = shared_ptr<Display>(XOpenDisplay(0), Free_XCloseDisplay());
    if (!displayPtr) throw runtime_error("Display is not open");
    display = displayPtr.get();
    root = XDefaultRootWindow(display);

    keyGrabber = make_shared<KeyGrabber>(display);
    fontHelper = make_shared<FontHelper>(display);

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
    selectInput(ResizeRedirectMask | SubstructureRedirectMask);
}

WmWindow* WindowManager::addWindow(Window window) {
    shared_ptr<WmWindow> wPtr(make_shared<WmWindow>(display, root, window));
    WmWindow* w = wPtr.get();
    windows.insert(make_pair(w->window, wPtr));
    windows.insert(make_pair(w->frame, wPtr));

    w->setWorkspace(currentWorkspace);

    stringstream ss;
    ss << desktop.x << ":" << desktop.w << ":" << desktop.y << ":" << desktop.h;
    addDebugText(ss.str());

    XWindowAttributes attributes;
    XGetWindowAttributes(display, w->frame, &attributes);
    w->relocate(desktop.x + (desktop.w - attributes.width) / 2,
            desktop.y + (desktop.h - attributes.height) / 2,
            attributes.width, attributes.height);

    w->selectDefaultInput();
    w->setDefaultEventMask();
    w->show();

    currentWindow = w;

    return w;
}

void WindowManager::setCurrentWindow(Window window) {
    currentWindow = findWindow(window);
    stringstream ss;
    ss << "CURRENT WINDOW 0x" << hex << currentWindow;
    addDebugText(ss.str());
}

void WindowManager::changeWorkspace(int number) {
    currentWindow = 0;
    if (currentWorkspace == &workspaces[number])
        return; // same workspace

    currentWorkspace->hide();
    currentWorkspace = &workspaces[number];
    if (moveWindow)
        moveWindow->setWorkspace(currentWorkspace);
    currentWorkspace->show();
    // @TODO: Select currentWindow by mouse position
}

void WindowManager::calculateDesktopSpace() {
    XWindowAttributes rAttr;
    XGetWindowAttributes(display, root, &rAttr);

    Geometry& d = desktop;
    d.x = rAttr.x;
    d.y = rAttr.y;
    d.w = rAttr.width;
    d.h = rAttr.height;

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