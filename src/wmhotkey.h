#ifndef WMHOTKEY_H
#define WMHOTKEY_H


#include <X11/Xlib.h>


class WmHotbutton {
    Display* display;
    Window window;
    unsigned int buttonCode;
    unsigned int modifier;
    unsigned int mask;
public:
    WmHotbutton(Display* display, Window window, unsigned int buttonCode, unsigned int modifier, unsigned int mask);
    ~WmHotbutton();
};

class WmHotkey {
    Display* display;
    Window window;
    int keyCode;
    unsigned int modifier;
public:
    WmHotkey(Display* display, Window window, int keyCode, unsigned int modifier);
    ~WmHotkey();
};


#endif // WMHOTKEY_H