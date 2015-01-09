#ifndef WMWINDOW_H
#define WMWINDOW_H

#include <X11/Xlib.h>

class WmWindow {
public:
    Display* display;
    Window window;

    WmWindow(Display* display, Window window);
    ~WmWindow();

    void show();
    void hide();
};

#endif // WMWINDOW_H