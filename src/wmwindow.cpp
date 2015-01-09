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

bool WmWindow::supportsProtocol(const std::string& protocol) throw () {
    bool found = false;

    int numberOfProtocols;
    Atom* protocols;

    Atom wm_delete_window = XInternAtom(display, protocol.c_str(), False);
    XGetWMProtocols(display, window, &protocols, &numberOfProtocols);

    for (int i = 0; i < numberOfProtocols; ++i) {
        if (protocols[i] == wm_delete_window) {
            found = true;
            break;
        }
    }

    XFree(protocols);

    return found;
}

bool WmWindow::close() {
    if (!supportsProtocol("WM_DELETE_WINDOW"))
        return false;

    XDestroyWindow(display, window);
    return true;
}