#include <X11/Xlib.h>
#include "wmwindow.h"
#include "workspace.h"

WmWindow::WmWindow(Display *display, Window root, Window window) :
display(display),
root(root),
window(window)
{
    XWindowAttributes wAttr;
    XGetWindowAttributes(display, window, &wAttr);

    frame = XCreateSimpleWindow(display, root, wAttr.x - 2, wAttr.y - 2, wAttr.width + 4, wAttr.height + 4, 0, 0, 0xff00ff);
    XReparentWindow(display, window, frame, 2, 2);
    XMapWindow(display, frame);
}

WmWindow::~WmWindow() {
    workspace->removeWindow(this);
    XWindowAttributes wAttr;
    XGetWindowAttributes(display, frame, &wAttr);
    if (window)
        XReparentWindow(display, window, root, wAttr.x, wAttr.y);
    XDestroyWindow(display, frame);
}

void WmWindow::hide() {
    XUnmapWindow(display, frame);
}

void WmWindow::show() {
    XMapWindow(display, frame);
}

void WmWindow::setActive(bool active) {
    XSetWindowBackground(display, frame, active ? 0x00ff00 : 0xff0000);
    XClearWindow(display, frame);
}

void WmWindow::setWorkspace(Workspace* workspace) {
    this->workspace = workspace;
}

bool WmWindow::staysFloating() {
    return staysFloating_;
}
void WmWindow::toggleFloating() {
    staysFloating_ = !staysFloating_;
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
    XSendEvent(display, window, false, 0, (XEvent*) &xevent);
}

void WmWindow::setDefaultEventMask() {
    XSetWindowAttributes attributes;
    attributes.event_mask = EnterWindowMask | LeaveWindowMask | StructureNotifyMask;
    XChangeWindowAttributes(display, window, CWEventMask, &attributes);
}

bool WmWindow::operator==(const Window& window) {
    return this->window == window;
}