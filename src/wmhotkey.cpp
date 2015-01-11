#include "wmhotkey.h"


WmHotbutton::WmHotbutton(Display *display, Window window, unsigned int buttonCode, unsigned int modifier, unsigned int mask, std::function<void()> onPress, std::function<void()> onRelease) :
display(display),
window(window),
buttonCode_(buttonCode),
modifier_(modifier),
mask_(mask),
onPress_(onPress),
onRelease_(onRelease)
{
    XGrabButton(display, buttonCode, modifier, window, true, mask, GrabModeAsync, GrabModeAsync, None, None);
}

WmHotbutton::~WmHotbutton() {
    XUngrabButton(display, buttonCode_, modifier_, window);
}

unsigned int WmHotbutton::buttonCode() {
    return buttonCode_;
}

unsigned int WmHotbutton::modifier() {
    return modifier_;
}

unsigned int WmHotbutton::mask() {
    return mask_;
}

void WmHotbutton::onPress() {
    if (onPress_)
        onPress_();
}

void WmHotbutton::onRelease() {
    if (onRelease_)
        onRelease_();
}

WmHotkey::WmHotkey(Display *display, Window window, int keyCode, unsigned int modifier, std::function<void()> onPress, std::function<void()> onRelease) :
display(display),
window(window),
keyCode_(keyCode),
modifier_(modifier),
onPress_(onPress),
onRelease_(onRelease)
{
    XGrabKey(display, keyCode, modifier, window, true, GrabModeAsync, GrabModeAsync);
}

WmHotkey::~WmHotkey() {
    XUngrabKey(display, keyCode_, modifier_, window);
}

unsigned int WmHotkey::keyCode() {
    return keyCode_;
}

unsigned int WmHotkey::modifier() {
    return modifier_;
}

void WmHotkey::onPress() {
    if (onPress_)
        onPress_();
}

void WmHotkey::onRelease() {
    if (onRelease_)
        onRelease_();
}

