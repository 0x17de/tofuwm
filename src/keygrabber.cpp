#include "keygrabber.h"

KeyGrabber::KeyGrabber(Display* display, int workspaceCount)
:
    display(display),
    root(DefaultRootWindow(display)),
    workspaceCount(workspaceCount)
{
    XGrabButton(display, 1, defaultModifier(), root, True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(display, 3, defaultModifier(), root, True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    XGrabKey(display, keyDMenu(), defaultModifier(), root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, keyClose(), defaultModifier()|ShiftMask, root, True, GrabModeAsync, GrabModeAsync);

    for (int i = 0; i < workspaceCount; ++i)
        XGrabKey(display, keyWorkspace(i), defaultModifier(), root, True, GrabModeAsync, GrabModeAsync);
}

KeyGrabber::~KeyGrabber() {
    XUngrabButton(display, 1, defaultModifier(), root);
    XUngrabButton(display, 3, defaultModifier(), root);

    XUngrabKey(display, keyDMenu(), defaultModifier(), root);
    XUngrabKey(display, keyClose(), defaultModifier()|ShiftMask, root);

    for (int i = 0; i < workspaceCount; ++i)
        XUngrabKey(display, keyWorkspace(i), defaultModifier(), root);
}

int KeyGrabber::defaultModifier() {
    return Mod4Mask;
}

int KeyGrabber::keyWorkspace(int number) {
    const char* const workspaceKeys[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    return XKeysymToKeycode(display, XStringToKeysym(workspaceKeys[number]));
}

int KeyGrabber::keyClose() {
    return XKeysymToKeycode(display, XStringToKeysym("q"));
}

int KeyGrabber::keyDMenu() {
    return XKeysymToKeycode(display, XStringToKeysym("d"));
}