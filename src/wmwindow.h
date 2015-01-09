#ifndef WMWINDOW_H
#define WMWINDOW_H

#include <string>
#include <X11/Xlib.h>

class WmWindow {
public:
    Display* display;
    Window window;

    WmWindow(Display* display, Window window);
    ~WmWindow();

    void show();
    void hide();
    bool supportsProtocol(const std::string& protocol) throw ();
    bool close();
};

#endif // WMWINDOW_H