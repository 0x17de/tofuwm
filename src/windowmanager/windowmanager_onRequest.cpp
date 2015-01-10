#include <iostream>
#include <sstream>
#include <X11/Xlib.h>
#include "windowmanager.h"


using namespace std;


void WindowManager::onMapRequest() {
    if (event.xmaprequest.parent == root) {
        addWindow(event.xmaprequest.window);
    } else {
        XMapWindow(display, event.xmaprequest.window);
    }
}

void WindowManager::onConfigureRequest() { // @TODO: fix window jumping
    if (event.xconfigurerequest.window) {
        stringstream ss;
        ss << "CONFIG WND " << event.xconfigurerequest.window;
        addDebugText(ss.str());

        WmWindow* w = findWindow(event.xconfigurerequest.window);
        if (w && w->window == event.xconfigurerequest.window) {
            addDebugText("ConfigureRequest - would relocate");
            w->relocate(event.xconfigurerequest.x, event.xconfigurerequest.y, event.xconfigurerequest.width, event.xconfigurerequest.height);
        }
    }
}

void WindowManager::onResizeRequest() { // @TODO: fix resize request
    if (event.xresizerequest.window && !moveWindow/*@HOTFIX*/) {
        stringstream ss;
        ss << "RESIZE WND " << event.xresizerequest.window;
        addDebugText(ss.str());

        WmWindow* w = findWindow(event.xresizerequest.window);
        if (w && w->window == event.xresizerequest.window)
            w->resize(event.xresizerequest.width + 4, event.xresizerequest.height + 4);
    }
}

void WindowManager::onCirculateRequest() {
}