#ifndef KEYGRABBER_H
#define KEYGRABBER_H

#include <X11/Xlib.h>

class KeyGrabber {
    Display* display;
    Window root;
    int workspaceCount;

public:
    KeyGrabber(Display* display, int workspaceCount);
    ~KeyGrabber();

    int defaultModifier();
    int keyWorkspace(int number);
    int keyClose();
    int keyDMenu();
};

#endif // KEYGRABBER_H