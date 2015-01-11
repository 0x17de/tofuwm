#ifndef WMHOTKEY_H
#define WMHOTKEY_H


#include <functional>
#include <X11/Xlib.h>


class WmHotbutton {
    Display* display;
    Window window;
    unsigned int buttonCode_;
    unsigned int modifier_;
    unsigned int mask_;
    std::function<void()> onPress_, onRelease_, onMotion_;
public:
    WmHotbutton(Display* display, Window window, unsigned int buttonCode, unsigned int modifier, unsigned int mask, std::function<void()> onPress, std::function<void()> onRelease, std::function<void()> onMotion);
    ~WmHotbutton();

    unsigned int buttonCode();
    unsigned int modifier();
    unsigned int mask();
    void onPress();
    void onRelease();
    void onMotion();
};

class WmHotkey {
    Display* display;
    Window window;
    int keyCode_;
    unsigned int modifier_;
    std::function<void()> onPress_, onRelease_;
public:
    WmHotkey(Display* display, Window window, int keyCode, unsigned int modifier, std::function<void()> onPress, std::function<void()> onRelease);
    ~WmHotkey();

    unsigned int keyCode();
    unsigned int modifier();
    void onPress();
    void onRelease();
};


#endif // WMHOTKEY_H