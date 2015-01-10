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

    selectDefaultInput();
    currentWorkspace = &workspaces[0];
}

WindowManager::~WindowManager() {

}

void WindowManager::initCursor() {
    cursor = XCreateFontCursor(display, XC_arrow);
    XDefineCursor(display, root, cursor);
}

void WindowManager::initBackground() {
    XSetWindowBackground(display, root, 0xc0c0c0);
    XClearWindow(display, root);
    XFlush(display);
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

void WindowManager::run() {
    // @TODO: Scan and add initially existing windows. Then set default event mask.
    initCursor();
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

void WindowManager::addDebugText(const std::string& text) {
    static string lastText;
    if (lastText == text) {
        debugStrings.front() += ".";
    } else {
        debugStrings.push_front(text);
        if (debugStrings.size() > 30)
            debugStrings.pop_back();
        lastText = text;
    }
}

void WindowManager::printDebugText() {
    GC gc = XDefaultGC(display, XDefaultScreen(display));
    fontHelper->setFont(gc);

    XClearWindow(display, root);

    int y = 40;
    for (string &text : debugStrings) {
        XDrawString(display, root, gc, 20, y, text.c_str(), text.length());
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
    // Loop
    printDebugText();
    while(running) {
        XNextEvent(display, &event);
        switch (event.type) {
            case MapRequest:
                addDebugText("EVENT MapRequest");
                onMapRequest(); break;
            case EnterNotify:
                addDebugText("EVENT EnterNotify");
                onEnter(); break;
            case LeaveNotify:
                addDebugText("EVENT LeaveNotify");
                onLeave(); break;
            case ConfigureRequest:
                addDebugText("EVENT ConfigureRequest");
                onConfigureRequest(); break;
            case ResizeRequest:
                addDebugText("EVENT ResizeRequest");
                onResizeRequest(); break;
            case CirculateRequest:
                addDebugText("EVENT CirculateRequest");
                onCirculateRequest(); break;
            case KeyPress:
                addDebugText("EVENT KeyPress");
                onKeyPress(); break;
            case ButtonPress:
                addDebugText("EVENT ButtonPress");
                onButtonPress(); break;
            case MotionNotify:
                addDebugText("EVENT MotionNotify");
                onMotion(); break;
            case ButtonRelease:
                addDebugText("EVENT ButtonRelease");
                onButtonRelease(); break;
            case CirculateNotify:
                addDebugText("EVENT CirculateNotify");
                onCirculateNotify(); break;
            case ConfigureNotify:
                addDebugText("EVENT ConfigureNotify");
                onConfigureNotify(); break;
            case DestroyNotify:
                addDebugText("EVENT DestroyNotify");
                onDestroyNotify(); break;
            case GravityNotify:
                addDebugText("EVENT GravityNotify");
                onGravityNotify(); break;
            case MapNotify:
                addDebugText("EVENT MapNotify");
                onMapNotify(); break;
            case ReparentNotify:
                addDebugText("EVENT ReparentNotify");
                onReparentNotify(); break;
            case UnmapNotify:
                addDebugText("EVENT UnmapNotify");
                onUnmapNotify(); break;
        }
        printDebugText();
    }
}