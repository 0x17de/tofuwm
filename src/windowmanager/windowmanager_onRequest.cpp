#include <iostream>
#include <sstream>
#include <X11/Xlib.h>
#include "windowmanager.h"


using namespace std;


void WindowManager::onMapRequest() {
    if (event_.xmaprequest.parent == root) {
        WmWindow* candidate = findWindow(event_.xmaprequest.window);
        if (!candidate) { // If unknown: add
            addWindow(event_.xmaprequest.window);
        } else {
            candidate->map();
        }
    } else {
        XMapWindow(display, event_.xmaprequest.window);
    }
}

void WindowManager::onConfigureRequest() {
    if (event_.xconfigurerequest.window && !moveWindow_) {
        stringstream ss;
        ss << "CONFIG WND " << event_.xconfigurerequest.window;
        addDebugText(ss.str(), LogLevel::VeryVerbose);

        WmWindow* w = findWindow(event_.xconfigurerequest.window);
        if (w == nullptr) {
            // Not known to the window manager
            XWindowChanges xchanges;
            xchanges.x = event_.xconfigurerequest.x;
            xchanges.y = event_.xconfigurerequest.y;
            xchanges.width = event_.xconfigurerequest.width;
            xchanges.height = event_.xconfigurerequest.height;
            xchanges.border_width = event_.xconfigurerequest.border_width;
            xchanges.sibling = event_.xconfigurerequest.above;
            xchanges.stack_mode = event_.xconfigurerequest.detail;
            XConfigureWindow(display, root,
                    event_.xconfigurerequest.value_mask, &xchanges);
        } else if (w->window == event_.xconfigurerequest.window) {
            if (w->windowMode == WindowMode::Floating) {
                w->relocate(event_.xconfigurerequest.x, event_.xconfigurerequest.y,
                        event_.xconfigurerequest.width, event_.xconfigurerequest.height,
                        event_.xconfigurerequest.value_mask);
            }
        }
    }
}

void WindowManager::onResizeRequest() { // @TODO: fix resize request
    if (event_.xresizerequest.window) {
        stringstream ss;
        ss << "RESIZE WND " << event_.xresizerequest.window;
        addDebugText(ss.str(), LogLevel::VeryVerbose);

        WmWindow* w = findWindow(event_.xresizerequest.window);
        if (w && w->window == event_.xresizerequest.window) {
            if (w->windowMode == WindowMode::Floating)
                w->resize(event_.xresizerequest.width, event_.xresizerequest.height);
        }
    }
}

void WindowManager::onCirculateRequest() {
}