#include <iostream>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>

#include "windowmanager.h"


using namespace std;


void WindowManager::run() {
    setWmName();
    initCursor();
    initBackground();
    calculateDesktopSpace(); // Initialize space.
    addExistingWindows();
    selectDefaultInput();
    calculateDesktopSpace(); // Find docked windows.
    loop();
}

void WindowManager::setWmName() {
    XTextProperty text;

    text.value = (unsigned char *) wmname.c_str();
    text.encoding = XA_STRING;
    text.format = 8;
    text.nitems = wmname.length();

    XSetWMName(display, root, &text);
}

void WindowManager::initCursor() {
    cursor = XCreateFontCursor(display, XC_arrow);
    XDefineCursor(display, root, cursor);
}

void WindowManager::initBackground() {
    XSetWindowBackground(display, root, 0xc0c0c0);
    XClearWindow(display, root);
    XFlush(display);
}

void WindowManager::addExistingWindows() {
    Window lroot, lparent;
    Window* children;
    unsigned int numberOfChildren;

    Status status = XQueryTree(display, root, &lroot, &lparent, &children, &numberOfChildren);
    if (status == BadWindow)
        throw runtime_error("BadWindow when enumerating windows.");
    if (!children)
        return;

    for (int i = 0; i < numberOfChildren; ++i) {
        WmWindow* w = addWindow(children[i]);
        w->setWorkspace(currentWorkspace);
    }

    XFree(children);
}