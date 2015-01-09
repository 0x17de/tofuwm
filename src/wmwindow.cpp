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