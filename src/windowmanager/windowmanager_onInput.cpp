#include <iostream>
#include <sstream>
#include <cmath>
#include <X11/Xlib.h>
#include "windowmanager.h"


using namespace std;


void WindowManager::onMotion() {
    if (event.xbutton.state & keyGrabber->defaultModifier()) {
        if (moveWindow) {
            int xdiff = event.xbutton.x_root - moveWindowStart.x_root;
            int ydiff = event.xbutton.y_root - moveWindowStart.y_root;
            if (moveWindowStart.button == 1) { // Left mouse
                moveWindow->relocate(moveWindowAttributes.x + xdiff, moveWindowAttributes.y + ydiff, moveWindowAttributes.width, moveWindowAttributes.height);
            } else if (moveWindowStart.button == 3) { // Right mouse
                moveWindow->relocate(moveWindowAttributes.x + (moveWindowExpandXPositive ? 0 : xdiff), moveWindowAttributes.y + (moveWindowExpandYPositive ? 0 : ydiff),
                        moveWindowAttributes.width + (moveWindowExpandXPositive ? xdiff : -xdiff), moveWindowAttributes.height + (moveWindowExpandYPositive ? ydiff : -ydiff));
            }
        }
    }
}

void WindowManager::onKeyPress() {
    if (event.xkey.state & keyGrabber->defaultModifier()) {
        if (event.xkey.state & ShiftMask) {
            if (event.xkey.keycode == keyGrabber->keyClose()) {
                addDebugText("WINDOW CLOSE");
                if (currentWindow)
                    currentWindow->close();
                currentWindow = 0; // @TODO: Select next window by mouse position
            }
        } else {
            if (event.xkey.keycode == keyGrabber->keyDMenu()) {
                addDebugText("DMENU SPAWN");
                char *const parmList[] = {(char *) "dmenu_run", 0};
                spawn("/usr/bin/dmenu_run", parmList);
            } else {
                for (int i = 0; i < workspaceCount(); ++i) {
                    if (event.xkey.keycode == keyGrabber->keyWorkspace(i)) {
                        stringstream ss;
                        ss << "WORKSPACE " << (i+1);
                        addDebugText(ss.str());
                        changeWorkspace(i);
                    }
                }
            }
        }
    }
}

void WindowManager::onKeyRelease() {
}

void WindowManager::onButtonPress() {
    if (event.xbutton.state & keyGrabber->defaultModifier()) {
        if (event.xbutton.subwindow) {
            moveWindow = findWindow(event.xbutton.subwindow);
            if (!moveWindow)
                return;
            currentWindow = moveWindow;
            currentWindow->setActive(true);
            XGetWindowAttributes(display, moveWindow->frame, &moveWindowAttributes);
            moveWindowStart = event.xbutton;
            XRaiseWindow(display, moveWindow->frame);
            if (moveWindowStart.button == 3) { // Right mouse
                moveWindowExpandXPositive = moveWindowStart.x_root >= moveWindowAttributes.x + moveWindowAttributes.width / 2;
                moveWindowExpandYPositive = moveWindowStart.y_root >= moveWindowAttributes.y + moveWindowAttributes.height / 2;
            }
        }
    }
}

void WindowManager::onButtonRelease() {
    moveWindowStart.subwindow = None;
    moveWindow = 0;
}