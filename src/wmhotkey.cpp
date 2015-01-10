#include "wmhotkey.h"


WmHotbutton::WmHotbutton(Display *display, Window window, unsigned int buttonCode, unsigned int modifier, unsigned int mask) :
display(display),
window(window),
buttonCode(buttonCode),
modifier(modifier),
mask(mask)
{
    XGrabButton(display, buttonCode, modifier, window, true, mask, GrabModeAsync, GrabModeAsync, None, None);
}

WmHotbutton::~WmHotbutton() {
    XUngrabButton(display, buttonCode, modifier, window);
}

WmHotkey::WmHotkey(Display *display, Window window, int keyCode, unsigned int modifier) :
        display(display),
        window(window),
        keyCode(keyCode),
        modifier(modifier)
{
    XGrabKey(display, keyCode, modifier, window, true, GrabModeAsync, GrabModeAsync);
}

WmHotkey::~WmHotkey() {
    XUngrabKey(display, keyCode, modifier, window);
}


