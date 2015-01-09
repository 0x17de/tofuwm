#include <iostream>
#include <cassert>
#include <cmath>
#include <X11/Xlib.h>

#include "wmwindow.h"
#include "windowmanager.h"
#include "display.h"


using namespace std;


WindowManager::WindowManager() :
    displayPtr(XOpenDisplay(0), Free_XCloseDisplay()),
    keyGrabber(displayPtr.get()),
    root(DefaultRootWindow(displayPtr.get()))
{
    assert((bool)displayPtr);
    XSelectInput(displayPtr.get(), root, SubstructureRedirectMask);
    currentWorkspace = &workspaces[0];
}

WindowManager::~WindowManager() {

}

void WindowManager::initBackground() {
    Display *display = displayPtr.get();

    srand(time(0));
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    XSetWindowBackground(display, root, 0 + r << 16 + g << 8 + b);
    XClearWindow(display, root);
    XFlush(display);
}

void WindowManager::run() {
    initBackground();
    loop();
}

void WindowManager::changeWorkspace(int number) {
    if (currentWorkspace == &workspaces[number])
        return; // same workspace
    currentWorkspace->hide();
    currentWorkspace = &workspaces[number];
    currentWorkspace->show();
}

void WindowManager::loop() {
    Display *display = displayPtr.get();
    // Loop
    while(running) {
        XNextEvent(display, &event);
        if (event.type == MapRequest) {
            shared_ptr<WmWindow> w(make_shared<WmWindow>(display, event.xmaprequest.window));
            currentWorkspace->addWindow(w);
        } else if (event.type == KeyPress) {
            if (event.xkey.keycode == keyGrabber.keyWorkspace1())
                changeWorkspace(0);
            else if (event.xkey.keycode == keyGrabber.keyWorkspace2())
                changeWorkspace(1);

            // @TODO: raise windows
            /* if (event.xkey.subwindow != None) {
                XRaiseWindow(display, event.xkey.subwindow);
                cout << "Raise" << endl;
            } */
        } else if (event.type == ButtonPress) {
            if (event.xbutton.subwindow != None) {
                XGetWindowAttributes(display, event.xbutton.subwindow, &attributes);
                start = event.xbutton;
            }
        } else if (event.type == MotionNotify) {
            if (start.subwindow != None) {
                int xdiff = event.xbutton.x_root - start.x_root;
                int ydiff = event.xbutton.y_root - start.y_root;
                XMoveResizeWindow(display, start.subwindow,
                        attributes.x + (start.button == 1 ? xdiff : 0),
                        attributes.y + (start.button == 1 ? ydiff : 0),
                        max(1, attributes.width + (start.button == 3 ? xdiff : 0)),
                        max(1, attributes.height + (start.button == 3 ? ydiff : 0)));
            }
        } else if (event.type == ButtonRelease) {
            start.subwindow = None;
        }
    }
}