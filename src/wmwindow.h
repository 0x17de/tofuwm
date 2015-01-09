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
    Atom getAtom(const std::string& protocol);
    bool supportsProtocol(const std::string& protocol) throw ();
    bool supportsProtocol(Atom protocol) throw ();
    void close();
    void setDefaultEventMask();
};

#endif // WMWINDOW_H