#include <iostream>
#include <X11/Xlib.h>
#include <sstream>
#include "windowmanager.h"


using namespace std;


void WindowManager::onKeyPress() {
    for (WmHotkey& hotkey : keyGrabber->hotkeys) {
        if (event.xkey.state == hotkey.modifier()
         && event.xkey.keycode == hotkey.keyCode()) {
            hotkey.onPress();
            break;
        }
    }
}

void WindowManager::onKeyRelease() {
    for (WmHotkey& hotkey : keyGrabber->hotkeys) {
        if (event.xkey.state == hotkey.modifier()
         && event.xkey.keycode == hotkey.keyCode()) {
            hotkey.onRelease();
            break;
        }
    }
}

void WindowManager::onButtonPress() {
    for (WmHotbutton& hotbutton : keyGrabber->hotbuttons) {
        if (event.xbutton.state == hotbutton.modifier()
         && event.xbutton.button == hotbutton.buttonCode()) {
            hotbutton.onPress();
            break;
        }
    }
}

void WindowManager::onButtonRelease() {
    for (WmHotbutton& hotbutton : keyGrabber->hotbuttons) {
        if (event.xbutton.state == hotbutton.modifier()
         && event.xbutton.button == hotbutton.buttonCode()) {
            hotbutton.onRelease();
            break;
        }
    }
}

static unsigned int motionMasks =
                Button1MotionMask
                |Button2MotionMask
                |Button3MotionMask
                |Button4MotionMask
                |Button5MotionMask;
void WindowManager::onMotion() {
    stringstream ss;
    ss << event.xmotion.state;
    addDebugText(ss.str());

    unsigned int state = event.xmotion.state &~ motionMasks;

    for (WmHotbutton& hotbutton : keyGrabber->hotbuttons) {
        if (state == hotbutton.modifier()) {
            hotbutton.onMotion();
            break;
        }
    }
}

void WindowManager::onMousePress() {
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

void WindowManager::onMouseRelease() {
    if (moveWindow) {
        moveWindow->selectDefaultInput();
        moveWindowStart.subwindow = None;
        moveWindow = 0;
    }
}

void WindowManager::onMouseMotion() {
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