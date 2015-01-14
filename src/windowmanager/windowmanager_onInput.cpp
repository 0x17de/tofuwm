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
        if (event_.xkey.state == hotkey.modifier()
         && event_.xkey.keycode == hotkey.keyCode()) {
            hotkey.onPress();
            break;
        }
    }
}

void WindowManager::onKeyRelease() {
    for (WmHotkey& hotkey : keyGrabber->hotkeys) {
        if (event_.xkey.state == hotkey.modifier()
         && event_.xkey.keycode == hotkey.keyCode()) {
            hotkey.onRelease();
            break;
        }
    }
}

void WindowManager::onButtonPress() {
    unsigned int state = event_.xbutton.state &~ motionMasks;

    for (WmHotbutton& hotbutton : keyGrabber->hotbuttons) {
        if (state == hotbutton.modifier()
         && event_.xbutton.button == hotbutton.buttonCode()) {
            hotbutton.onPress();
            break;
        }
    }
}

void WindowManager::onButtonRelease() {
    unsigned int state = event_.xbutton.state &~ motionMasks;

    for (WmHotbutton& hotbutton : keyGrabber->hotbuttons) {
        if (state == hotbutton.modifier()
         && event_.xbutton.button == hotbutton.buttonCode()) {
            hotbutton.onRelease();
            break;
        }
    }
}

void WindowManager::onMotion() {
    unsigned int state = event_.xmotion.state &~ motionMasks;

    for (WmHotbutton& hotbutton : keyGrabber->hotbuttons) {
        if (state == hotbutton.modifier()) {
            hotbutton.onMotion();
            break;
        }
    }
}

void WindowManager::onMousePress() {
    if (event_.xbutton.subwindow) {
        moveWindow_ = findWindow(event_.xbutton.subwindow);
        if (!moveWindow_)
            return;
        if (moveWindow_->windowMode == WindowMode::Tiled) {
            moveWindow_ = 0;
            return;
        }

        setCurrentWindow(moveWindow_);
        XGetWindowAttributes(display, moveWindow_->frame, &moveWindowAttributes_);
        moveWindowStart_ = event_.xbutton;

        if (moveWindow_->windowMode == WindowMode::Floating)
            XRaiseWindow(display, moveWindow_->frame);

        if (moveWindowStart_.button == 3) { // Right mouse
            moveWindowExpandXPositive_ = moveWindowStart_.x_root >= moveWindowAttributes_.x + moveWindowAttributes_.width / 2;
            moveWindowExpandYPositive_ = moveWindowStart_.y_root >= moveWindowAttributes_.y + moveWindowAttributes_.height / 2;
        }
    }
}

void WindowManager::onMouseRelease() {
    if (moveWindow_) {
        moveWindowStart_.subwindow = None;
        moveWindow_ = nullptr;
    }
}

void WindowManager::onMouseMotion() {
    if (moveWindow_) {
        int xdiff = event_.xbutton.x_root - moveWindowStart_.x_root;
        int ydiff = event_.xbutton.y_root - moveWindowStart_.y_root;
        if (moveWindowStart_.button == 1) { // Left mouse
            moveWindow_->relocate(moveWindowAttributes_.x + xdiff, moveWindowAttributes_.y + ydiff, moveWindowAttributes_.width, moveWindowAttributes_.height);
        } else if (moveWindowStart_.button == 3) { // Right mouse
            moveWindow_->relocate(moveWindowAttributes_.x + (moveWindowExpandXPositive_ ? 0 : min(xdiff, moveWindowAttributes_.width)), moveWindowAttributes_.y + (moveWindowExpandYPositive_ ? 0 : min(ydiff, moveWindowAttributes_.height)),
                    moveWindowAttributes_.width + (moveWindowExpandXPositive_ ? xdiff : -xdiff), moveWindowAttributes_.height + (moveWindowExpandYPositive_ ? ydiff : -ydiff));
        }
    }
}