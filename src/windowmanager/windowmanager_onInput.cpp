#include <iostream>
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
    for (WmHotkey& hotkey : keyGrabber->hotkeys) {
        if (event.xkey.state == hotkey.modifier()
         && event.xkey.keycode == hotkey.keyCode()) {
            hotkey.onPress();
        }
    }
}

void WindowManager::onKeyRelease() {
    for (WmHotkey& hotkey : keyGrabber->hotkeys) {
        if (event.xkey.state == hotkey.modifier()
         && event.xkey.keycode == hotkey.keyCode()) {
            hotkey.onRelease();
        }
    }
}

void WindowManager::onButtonPress() {
    if (event.xbutton.state & keyGrabber->defaultModifier()) {
        if (event.xbutton.subwindow) {
            moveWindow = findWindow(event.xbutton.subwindow);
            if (!moveWindow)
                return;
            moveWindow->selectNoInput();
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
    if (moveWindow) {
        moveWindow->selectDefaultInput();
        moveWindowStart.subwindow = None;
        moveWindow = 0;
    }
}