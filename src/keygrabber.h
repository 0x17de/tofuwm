#ifndef KEYGRABBER_H
#define KEYGRABBER_H

#include <X11/Xlib.h>

class KeyGrabber {
    Display* display;
    Window root;

public:
    KeyGrabber(Display* display);
    ~KeyGrabber();

    int defaultModifier();
    int keyWorkspace1();
    int keyWorkspace2();
    int keyClose();
    int keyDMenu();
};

#endif // KEYGRABBER_H