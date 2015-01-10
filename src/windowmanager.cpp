#include <iostream>
#include <cassert>
#include <X11/Xlib.h>
#include <unistd.h>
#include <sstream>

#include "windowmanager.h"
#include "display.h"


using namespace std;


WindowManager::WindowManager() :
    displayPtr(XOpenDisplay(0), Free_XCloseDisplay()),
    keyGrabber(displayPtr.get()),
    root(DefaultRootWindow(displayPtr.get())),
    fontHelper(displayPtr.get())
{
    assert((bool)displayPtr);
    XSelectInput(displayPtr.get(), root, SubstructureRedirectMask);
    currentWorkspace = &workspaces[0];
}

WindowManager::~WindowManager() {

}

void WindowManager::initBackground() {
    Display *display = displayPtr.get();

    XSetWindowBackground(display, root, 0xc0c0c0);
    XClearWindow(display, root);
    XFlush(display);
}

void WindowManager::run() {
    // @TODO: Scan and add initially existing windows. Then set default event mask.
    initBackground();
    calculateDesktopSpace();
    loop();
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
    XGetWindowAttributes(displayPtr.get(), root, &rAttr);

    Geometry& d = desktop;
    d.x = rAttr.x;
    d.y = rAttr.y;
    d.w = rAttr.width;
    d.h = rAttr.height;

    for (shared_ptr<WmWindow>& w : dockedWindows) {
        XWindowAttributes wAttr;
        XGetWindowAttributes(displayPtr.get(), w->window, &wAttr);

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

void WindowManager::addDebugText(const std::string& text) {
    debugStrings.push_front(text);
    if (debugStrings.size() > 30)
        debugStrings.pop_back();
}

void WindowManager::printDebugText() {
    GC gc = XDefaultGC(displayPtr.get(), XDefaultScreen(displayPtr.get()));
    fontHelper.setFont(gc);

    XClearWindow(displayPtr.get(), root);

    int y = 40;
    for (string &text : debugStrings) {
        XDrawString(displayPtr.get(), root, gc, 20, y, text.c_str(), text.length());
        y += 16;
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

void WindowManager::loop() {
    Display *display = displayPtr.get();
    // Loop
    printDebugText();
    while(running) {
        XNextEvent(display, &event);
        switch (event.type) {
            case MapRequest:
                onMapRequest(); break;
            case EnterNotify:
                onEnter(); break;
            case LeaveNotify:
                onLeave(); break;
            case ConfigureRequest:
                onConfigureRequest(); break;
            case CirculateRequest:
                onCirculateRequest(); break;
            case KeyPress:
                onKeyPress(); break;
            case ButtonPress:
                onButtonPress(); break;
            case MotionNotify:
                onMotion(); break;
            case ButtonRelease:
                onButtonRelease(); break;
            case CirculateNotify:
                onCirculateNotify(); break;
            case ConfigureNotify:
                onConfigureNotify(); break;
            case DestroyNotify:
                onDestroyNotify(); break;
            case GravityNotify:
                onGravityNotify(); break;
            case MapNotify:
                onMapNotify(); break;
            case ReparentNotify:
                onReparentNotify(); break;
            case UnmapNotify:
                onUnmapNotify(); break;
        }
        printDebugText();
    }
}