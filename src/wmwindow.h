#ifndef WMWINDOW_H
#define WMWINDOW_H

#include <X11/Xlib.h>

struct WmWindow {
    Display* display;
    Window window;

public:
    WmWindow(Display* display, Window window);
    ~WmWindow();

    void show();
    void hide();
};

#endif // WMWINDOW_H