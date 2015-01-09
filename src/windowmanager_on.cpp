#include <cmath>
#include <X11/Xlib.h>
#include "windowmanager.h"
#include "wmwindow.h"


using namespace std;


void WindowManager::onMapRequest() {
    shared_ptr <WmWindow> w(make_shared<WmWindow>(displayPtr.get(), event.xmaprequest.window));
    currentWorkspace->addWindow(w);
}

void WindowManager::onKeyPress() {
    if (event.xkey.keycode == keyGrabber.keyWorkspace1())
    changeWorkspace(0);
    else if (event.xkey.keycode == keyGrabber.keyWorkspace2())
    changeWorkspace(1);

    // @TODO: raise windows
    /* if (event.xkey.subwindow != None) {
        XRaiseWindow(display, event.xkey.subwindow);
        cout << "Raise" << endl;
    } */
}

void WindowManager::onKeyRelease() {
}

void WindowManager::onButtonPress() {
    if (event.xbutton.subwindow != None) {
        XGetWindowAttributes(displayPtr.get(), event.xbutton.subwindow, &moveWindowAttributes);
        moveWindowStart = event.xbutton;
        if (moveWindowStart.button == 3) { // Right mouse
            moveWindowExpandXPositive = moveWindowStart.x_root >= moveWindowAttributes.x + moveWindowAttributes.width / 2;
            moveWindowExpandYPositive = moveWindowStart.y_root >= moveWindowAttributes.y + moveWindowAttributes.height / 2;
        }
    }
}

void WindowManager::onButtonRelease() {
    moveWindowStart.subwindow = None;
}

void WindowManager::onEnter() {
}

void WindowManager::onLeave() {
}

void WindowManager::onMotion() {
    if (moveWindowStart.subwindow != None) {
        int xdiff = event.xbutton.x_root - moveWindowStart.x_root;
        int ydiff = event.xbutton.y_root - moveWindowStart.y_root;
        if (moveWindowStart.button == 1) { // Left mouse
            XMoveResizeWindow(displayPtr.get(), moveWindowStart.subwindow,
                    moveWindowAttributes.x + xdiff, moveWindowAttributes.y + ydiff,
                    max(1, moveWindowAttributes.width), max(1, moveWindowAttributes.height));
        } else if (moveWindowStart.button == 3) { // Right mouse
            XMoveResizeWindow(displayPtr.get(), moveWindowStart.subwindow,
                    moveWindowAttributes.x + (moveWindowExpandXPositive ? 0 : xdiff), moveWindowAttributes.y + (moveWindowExpandYPositive ? 0 : ydiff),
                    max(1, moveWindowAttributes.width + (moveWindowExpandXPositive ? xdiff : -xdiff)), max(1, moveWindowAttributes.height + (moveWindowExpandYPositive ? ydiff : -ydiff)));
        }
    }
}

void WindowManager::onConfigureRequest() {
}

void WindowManager::onCirculateRequest() {
}