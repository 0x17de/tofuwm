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

void WindowManager::onConfigureRequest() {
    if (event.xconfigurerequest.window) {
        stringstream ss;
        ss << "CONFIG WND " << event.xconfigurerequest.window;
        addDebugText(ss.str());

        WmWindow* w = findWindow(event.xconfigurerequest.window);
        if (w == nullptr) {
            // Not known to the window manager
            XWindowChanges xchanges;
            xchanges.x = event.xconfigurerequest.x;
            xchanges.y = event.xconfigurerequest.y;
            xchanges.width = event.xconfigurerequest.width;
            xchanges.height = event.xconfigurerequest.height;
            xchanges.border_width = event.xconfigurerequest.border_width;
            xchanges.sibling = event.xconfigurerequest.above;
            xchanges.stack_mode = event.xconfigurerequest.detail;
            XConfigureWindow(display, root,
                    event.xconfigurerequest.value_mask, &xchanges);
        } else if (w->window == event.xconfigurerequest.window) {
            if (w->windowMode == WindowMode::Floating) {
                w->relocate(event.xconfigurerequest.x, event.xconfigurerequest.y,
                        event.xconfigurerequest.width, event.xconfigurerequest.height,
                        event.xconfigurerequest.value_mask);
            }
        }
    }
}

void WindowManager::onResizeRequest() { // @TODO: fix resize request
    if (event.xresizerequest.window) {
        stringstream ss;
        ss << "RESIZE WND " << event.xresizerequest.window;
        addDebugText(ss.str());

        WmWindow* w = findWindow(event.xresizerequest.window);
        if (w && w->window == event.xresizerequest.window) {
            if (w->windowMode == WindowMode::Floating)
                w->resize(event.xresizerequest.width, event.xresizerequest.height);
        }
    }
}

void WindowManager::onCirculateRequest() {
}