#include <iostream>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <sstream>

#include "windowmanager.h"


using namespace std;


WmStopAction WindowManager::run() {
    //setErrorHandler();
    setWmName();
    initCursor();
    initBackground();
    calculateDesktopSpace(); // Initialize space.
    addExistingWindows();
    selectDefaultInput();
    calculateDesktopSpace(); // Find docked windows.
    onLoad();
    loop();
    return stopAction_;
}

void WindowManager::onLoad() {
    char *const paramList[] = {(char*)"feh", (char*)"--bg-fill", (char*)"/home/it/.config/bg.png", 0};
    spawn("/usr/bin/feh", paramList);
}

static int windowManagerErrorHandler(Display* display, XErrorEvent* event) {
    char buffer[4096];
    XGetErrorText(display, event->error_code, buffer, sizeof(buffer));

    stringstream ss;
    ss << "Error: " << buffer;
    throw runtime_error(ss.str());
}
void WindowManager::setErrorHandler() {
    XSetErrorHandler(windowManagerErrorHandler);
}

void WindowManager::setWmName() {
    XTextProperty text;

    text.value = (unsigned char *) wmname_.c_str();
    text.encoding = XA_STRING;
    text.format = 8;
    text.nitems = wmname_.length();

    XSetWMName(display, root, &text);
}

void WindowManager::initCursor() {
    cursor_ = XCreateFontCursor(display, XC_arrow);
    XDefineCursor(display, root, cursor_);
}

void WindowManager::initBackground() {
    XSetWindowBackground(display, root, 0xc0c0c0);
    XClearWindow(display, root);
    XFlush(display);
}

void WindowManager::addExistingWindows() throw() {
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
        currentWorkspace_->addWindow(w);
    }

    XFree(children);
}