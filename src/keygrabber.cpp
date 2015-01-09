#include "keygrabber.h"

KeyGrabber::KeyGrabber(Display* display)
:
    display(display),
    root(DefaultRootWindow(display))
{
    XGrabButton(display, 1, defaultModifier(), root, True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(display, 3, defaultModifier(), root, True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    XGrabKey(display, keyDMenu(), defaultModifier(), root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, keyClose(), defaultModifier()|ShiftMask, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, keyWorkspace1(), defaultModifier(), root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, keyWorkspace2(), defaultModifier(), root, True, GrabModeAsync, GrabModeAsync);
}

KeyGrabber::~KeyGrabber() {
    XUngrabButton(display, 1, defaultModifier(), root);
    XUngrabButton(display, 3, defaultModifier(), root);

    XUngrabKey(display, keyDMenu(), defaultModifier(), root);
    XUngrabKey(display, keyClose(), defaultModifier()|ShiftMask, root);
    XUngrabKey(display, keyWorkspace1(), defaultModifier(), root);
    XUngrabKey(display, keyWorkspace2(), defaultModifier(), root);
}

int KeyGrabber::defaultModifier() {
    return Mod4Mask;
}

int KeyGrabber::keyWorkspace1() {
    return XKeysymToKeycode(display, XStringToKeysym("1"));
}

int KeyGrabber::keyWorkspace2() {
    return XKeysymToKeycode(display, XStringToKeysym("2"));
}

int KeyGrabber::keyClose() {
    return XKeysymToKeycode(display, XStringToKeysym("q"));
}

int KeyGrabber::keyDMenu() {
    return XKeysymToKeycode(display, XStringToKeysym("d"));
}