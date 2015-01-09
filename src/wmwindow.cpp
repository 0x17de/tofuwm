#include <X11/Xlib.h>
#include "wmwindow.h"

WmWindow::WmWindow(Display *display, Window window) :
display(display),
window(window)
{
}

WmWindow::~WmWindow() {
}

void WmWindow::hide() {
    XUnmapWindow(display, window);
}

void WmWindow::show() {
    XMapWindow(display, window);
}

Atom WmWindow::getAtom(const std::string& protocol) {
    return XInternAtom(display, protocol.c_str(), false);
}

bool WmWindow::supportsProtocol(const std::string& protocol) throw () {
    Atom wm_delete_window = XInternAtom(display, protocol.c_str(), false);
    return supportsProtocol(wm_delete_window);
}

bool WmWindow::supportsProtocol(Atom protocol) throw () {
    bool found = false;

    int numberOfProtocols;
    Atom* protocols;

    XGetWMProtocols(display, window, &protocols, &numberOfProtocols);

    for (int i = 0; i < numberOfProtocols; ++i) {
        if (protocols[i] == protocol) {
            found = true;
            break;
        }
    }

    XFree(protocols);

    return found;
}

void WmWindow::close() {
    Atom wm_delete_window = getAtom("WM_DELETE_WINDOW");

    if (!supportsProtocol(wm_delete_window)) {
        XDestroyWindow(display, window);
        return;
    }

    Atom wm_protocols = getAtom("WM_PROTOCOLS");

    XClientMessageEvent xevent;
    xevent.type = ClientMessage;
    xevent.window = window;
    xevent.message_type = wm_protocols;
    xevent.format = 32;
    xevent.data.l[0] = wm_delete_window;
    xevent.data.l[1] = CurrentTime;
    XSendEvent(display, window, false, 0, (XEvent *)&xevent);
}

void WmWindow::setDefaultEventMask() {
    XSetWindowAttributes attributes;
    attributes.event_mask = EnterWindowMask | LeaveWindowMask;
    XChangeWindowAttributes(display, window, CWEventMask, &attributes);
}