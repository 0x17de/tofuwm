#ifndef KEYGRABBER_H
#define KEYGRABBER_H

#include <list>
#include <X11/Xlib.h>
#include "wmhotkey.h"

class WindowManager;
class KeyGrabber {
    WindowManager* wm;
    int workspaceCount;

public:
    std::list<WmHotbutton> hotbuttons;
    std::list<WmHotkey> hotkeys;

    KeyGrabber(WindowManager* wm, int workspaceCount);
    ~KeyGrabber();

    int defaultModifier();
    int keyWorkspace(int number);
    int keyClose();
    int keyDMenu();
};

#endif // KEYGRABBER_H