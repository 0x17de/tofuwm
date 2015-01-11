#include <iostream>
#include <X11/Xlib.h>
#include <sstream>
#include "windowmanager.h"


using namespace std;


static unsigned int motionMasks =
        Button1MotionMask
        |Button2MotionMask
        |Button3MotionMask
        |Button4MotionMask
        |Button5MotionMask;


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
    unsigned int state = event.xbutton.state &~ motionMasks;

    for (WmHotbutton& hotbutton : keyGrabber->hotbuttons) {
        if (state == hotbutton.modifier()
         && event.xbutton.button == hotbutton.buttonCode()) {
            hotbutton.onPress();
            break;
        }
    }
}

void WindowManager::onButtonRelease() {
    unsigned int state = event.xbutton.state &~ motionMasks;

    for (WmHotbutton& hotbutton : keyGrabber->hotbuttons) {
        if (state == hotbutton.modifier()
         && event.xbutton.button == hotbutton.buttonCode()) {
            hotbutton.onRelease();
            break;
        }
    }
}

void WindowManager::onMotion() {
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
        if (moveWindow->windowMode == WindowMode::Tiled) {
            moveWindow = 0;
            return;
        }

        setCurrentWindow(moveWindow);
        XGetWindowAttributes(display, moveWindow->frame, &moveWindowAttributes);
        moveWindowStart = event.xbutton;

        if (moveWindow->windowMode == WindowMode::Floating)
            XRaiseWindow(display, moveWindow->frame);

        if (moveWindowStart.button == 3) { // Right mouse
            moveWindowExpandXPositive = moveWindowStart.x_root >= moveWindowAttributes.x + moveWindowAttributes.width / 2;
            moveWindowExpandYPositive = moveWindowStart.y_root >= moveWindowAttributes.y + moveWindowAttributes.height / 2;
        }
    }
}

void WindowManager::onMouseRelease() {
    if (moveWindow) {
        moveWindowStart.subwindow = None;
        moveWindow = nullptr;
    }
}

void WindowManager::onMouseMotion() {
    if (moveWindow) {
        int xdiff = event.xbutton.x_root - moveWindowStart.x_root;
        int ydiff = event.xbutton.y_root - moveWindowStart.y_root;
        if (moveWindowStart.button == 1) { // Left mouse
            moveWindow->relocate(moveWindowAttributes.x + xdiff, moveWindowAttributes.y + ydiff, moveWindowAttributes.width, moveWindowAttributes.height);
        } else if (moveWindowStart.button == 3) { // Right mouse
            moveWindow->relocate(moveWindowAttributes.x + (moveWindowExpandXPositive ? 0 : min(xdiff, moveWindowAttributes.width)), moveWindowAttributes.y + (moveWindowExpandYPositive ? 0 : min(ydiff, moveWindowAttributes.height)),
                    moveWindowAttributes.width + (moveWindowExpandXPositive ? xdiff : -xdiff), moveWindowAttributes.height + (moveWindowExpandYPositive ? ydiff : -ydiff));
        }
    }
}