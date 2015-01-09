#include <iostream>
#include <cassert>
#include <X11/Xlib.h>

#include "windowmanager.h"
#include "display.h"


using namespace std;


WindowManager::WindowManager() :
    displayPtr(XOpenDisplay(0), Free_XCloseDisplay()),
    keyGrabber(displayPtr.get()),
    root(DefaultRootWindow(displayPtr.get()))
{
    assert((bool)displayPtr);
    XSelectInput(displayPtr.get(), root,
            SubstructureRedirectMask
            | EnterWindowMask
            | LeaveWindowMask
    );
    currentWorkspace = &workspaces[0];
}

WindowManager::~WindowManager() {

}

void WindowManager::initBackground() {
    Display *display = displayPtr.get();

    srand(time(0));
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    XSetWindowBackground(display, root, 0 + r << 16 + g << 8 + b);
    XClearWindow(display, root);
    XFlush(display);
}

void WindowManager::run() {
    initBackground();
    loop();
}

void WindowManager::changeWorkspace(int number) {
    if (currentWorkspace == &workspaces[number])
        return; // same workspace
    currentWorkspace->hide();
    currentWorkspace = &workspaces[number];
    currentWorkspace->show();
}

void WindowManager::calculateDesktopSpace() {
    XWindowAttributes rAttr;
    XGetWindowAttributes(displayPtr.get(), root, &rAttr);

    Geometry& d = desktop;
    d.x = rAttr.x;
    d.y = rAttr.x;
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

void WindowManager::loop() {
    Display *display = displayPtr.get();
    // Loop
    while(running) {
        XNextEvent(display, &event);
        if (event.type == MapRequest) {
            onMapRequest();
        } else if (event.type == EnterNotify) {
            onEnter();
        } else if (event.type == LeaveNotify) {
            onLeave();
        } else if (event.type == ConfigureRequest) {
            onConfigureRequest();
        } else if (event.type == CirculateRequest) {
            onCirculateRequest();
        } else if (event.type == KeyPress) {
            onKeyPress();
        } else if (event.type == ButtonPress) {
            onButtonPress();
        } else if (event.type == MotionNotify) {
            onMotion();
        } else if (event.type == ButtonRelease) {
            onButtonRelease();
        }
    }
}