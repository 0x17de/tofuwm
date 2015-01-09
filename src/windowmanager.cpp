#include <iostream>
#include <cassert>
#include <X11/Xlib.h>
#include <unistd.h>

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
    XSelectInput(displayPtr.get(), root,
            SubstructureRedirectMask
            | StructureNotifyMask
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
    XSetWindowBackground(display, root, 0xc0c0c0);
    XClearWindow(display, root);
    XFlush(display);
}

void WindowManager::run() {
    initBackground();
    loop();
}

void WindowManager::changeWorkspace(int number) {
    currentWindow = 0;
    if (currentWorkspace == &workspaces[number])
        return; // same workspace
    currentWorkspace->hide();
    currentWorkspace = &workspaces[number];
    currentWorkspace->show();
    // @TODO: Select currentWindow by mouse position
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

void WindowManager::loop() {
    Display *display = displayPtr.get();
    // Loop
    printDebugText();
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
        printDebugText();
    }
}