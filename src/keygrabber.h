#ifndef KEYGRABBER_H
#define KEYGRABBER_H

#include <string>
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

    unsigned int key(const std::string &character);
};

#endif // KEYGRABBER_H