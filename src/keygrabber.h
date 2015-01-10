#ifndef KEYGRABBER_H
#define KEYGRABBER_H

#include <list>
#include <X11/Xlib.h>
#include "wmhotkey.h"

class KeyGrabber {
    Display* display;
    Window root;
    int workspaceCount;

    std::list<WmHotbutton> hotbuttons;
    std::list<WmHotkey> hotkeys;

public:
    KeyGrabber(Display* display, int workspaceCount);
    ~KeyGrabber();

    int defaultModifier();
    int keyWorkspace(int number);
    int keyClose();
    int keyDMenu();
};

#endif // KEYGRABBER_H