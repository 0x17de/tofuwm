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
        XGetWindowAttributes(displayPtr.get(), event.xbutton.subwindow, &attributes);
        start = event.xbutton;
    }
}

void WindowManager::onButtonRelease() {
    start.subwindow = None;
}

void WindowManager::onEnter() {
}

void WindowManager::onLeave() {
}

void WindowManager::onMotion() {
    if (start.subwindow != None) {
        int xdiff = event.xbutton.x_root - start.x_root;
        int ydiff = event.xbutton.y_root - start.y_root;
        XMoveResizeWindow(displayPtr.get(), start.subwindow,
                attributes.x + (start.button == 1 ? xdiff : 0),
                attributes.y + (start.button == 1 ? ydiff : 0),
                max(1, attributes.width + (start.button == 3 ? xdiff : 0)),
                max(1, attributes.height + (start.button == 3 ? ydiff : 0)));
    }
}

void WindowManager::onConfigureRequest() {
}

void WindowManager::onCirculateRequest() {
}