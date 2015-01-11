#include <sstream>
#include <cmath>
#include <X11/Xlib.h>
#include "windowmanager/windowmanager.h"
#include "wmwindow.h"
#include "../workspace.h"


using namespace std;


WmWindow::WmWindow(WindowManager* wm, Window window) :
        wm(wm),
        window(window)
{
    XWindowAttributes wAttr;
    XGetWindowAttributes(wm->display, window, &wAttr);

    frame = XCreateSimpleWindow(wm->display, wm->root, wAttr.x - 2, wAttr.y - 2, wAttr.width + 4, wAttr.height + 4, 0, 0, 0xff00ff);
    map();

    setDefaultEventMask();
}

WmWindow::~WmWindow() {
    XWindowAttributes wAttr;
    XGetWindowAttributes(wm->display, frame, &wAttr);
    if (window && isMapped)
        XReparentWindow(wm->display, window, wm->root, wAttr.x, wAttr.y);
    XDestroyWindow(wm->display, frame);
}

void WmWindow::map() {
    setSubstructureEvents(false);
    XMapWindow(wm->display, frame);
    XMapWindow(wm->display, window);
    XReparentWindow(wm->display, window, frame, 2, 2);
    isMapped = true;
    setSubstructureEvents(true);
}

void WmWindow::setSubstructureEvents(bool enable) {
        // No event during remapping
        XSetWindowAttributes attributes;
        attributes.override_redirect = 1;
        attributes.event_mask = enable ? SubstructureRedirectMask | EnterWindowMask | LeaveWindowMask : 0;
        XChangeWindowAttributes(wm->display, frame, CWEventMask | CWOverrideRedirect, &attributes);
};

std::shared_ptr<WmWindow> WmWindow::shared() {
    return shared_from_this();
}

WmFrameType WmWindow::containerType() {
    return WmFrameType::Window;
}

void WmWindow::hide() {
    XUnmapWindow(wm->display, frame);
}

void WmWindow::show() {
    XMapWindow(wm->display, frame);
}

void WmWindow::setActive(bool active) {
    XSetWindowBackground(wm->display, frame, active ? 0x00ff00 : 0xff0000);
    XClearWindow(wm->display, frame);
}

bool WmWindow::supportsProtocol(Atom protocol) throw () {
    bool found = false;

    int numberOfProtocols;
    Atom* protocols;

    if (!XGetWMProtocols(wm->display, window, &protocols, &numberOfProtocols))
        return false;

    for (int i = 0; i < numberOfProtocols; ++i) {
        if (protocols[i] == protocol) {
            found = true;
            break;
        }
    }

    XFree(protocols);

    return found;
}

int WmWindow::minWindowSize() {
    return 5;
}

void WmWindow::resize(int w, int h) {
    XWindowChanges xchanges;
    xchanges.width = max(minWindowSize(), w - 4);
    xchanges.height = max(minWindowSize(), h - 4);
    XConfigureWindow(wm->display, window, CWWidth|CWHeight, &xchanges);
    xchanges.width += 4;
    xchanges.height += 4;
    XConfigureWindow(wm->display, frame, CWWidth|CWHeight, &xchanges);
}

void WmWindow::realign() {
    Geometry& g = geometry();
    relocate(g.x, g.y, g.w, g.h);
}

void WmWindow::relocate(int x, int y, int w, int h, unsigned int event_mask) {
    stringstream ss;
    ss << "RELOC " << window << ": " << x << ":" << y << ":" << w << ":" << h;
    wm->addDebugText(ss.str(), LogLevel::Verbose);

    // We don't care about other masks
    event_mask &= CWX | CWY | CWWidth | CWHeight;
    unsigned int event_mask_window = event_mask & (CWWidth | CWHeight);

    XWindowChanges xchanges;
    if (event_mask_window) {
        xchanges.width = max(minWindowSize(), w - 4);
        xchanges.height = max(minWindowSize(), h - 4);
        XConfigureWindow(wm->display, window, event_mask & ~(CWX | CWY), &xchanges);
    }
    if (event_mask) {
        xchanges.x = x;
        xchanges.y = y;
        xchanges.width += 4;
        xchanges.height += 4;
        XConfigureWindow(wm->display, frame, event_mask, &xchanges);
    }
}

void WmWindow::relocate(int x, int y, int w, int h) {
    relocate(x, y, w, h, CWX | CWY | CWHeight | CWWidth);
}

void WmWindow::close() {
    Atom wm_delete_window = wm->getAtom("WM_DELETE_WINDOW");

    if (!supportsProtocol(wm_delete_window)) {
        XDestroyWindow(wm->display, window);
        return;
    }

    Atom wm_protocols = wm->getAtom("WM_PROTOCOLS");

    XClientMessageEvent xevent;
    xevent.type = ClientMessage;
    xevent.window = window;
    xevent.message_type = wm_protocols;
    xevent.format = 32;
    xevent.data.l[0] = wm_delete_window;
    xevent.data.l[1] = CurrentTime;
    XSendEvent(wm->display, window, false, 0, (XEvent*) &xevent);
}

void WmWindow::setDefaultEventMask() {
    XSetWindowAttributes attributes;

    attributes.event_mask = PropertyChangeMask | StructureNotifyMask;
    XChangeWindowAttributes(wm->display, window, CWEventMask, &attributes);

    attributes.event_mask = EnterWindowMask | LeaveWindowMask | SubstructureRedirectMask;
    XChangeWindowAttributes(wm->display, frame, CWEventMask, &attributes);
}

bool WmWindow::operator==(const Window& window) {
    return this->window == window;
}