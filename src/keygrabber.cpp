#include "keygrabber.h"

KeyGrabber::KeyGrabber(Display* display, int workspaceCount)
:
    display(display),
    root(DefaultRootWindow(display)),
    workspaceCount(workspaceCount)
{
    hotbuttons.emplace_back(display, root, 1, defaultModifier(), ButtonPressMask|ButtonReleaseMask|PointerMotionMask);
    hotbuttons.emplace_back(display, root, 3, defaultModifier(), ButtonPressMask|ButtonReleaseMask|PointerMotionMask);

    hotkeys.emplace_back(display, root, keyDMenu(), defaultModifier());
    hotkeys.emplace_back(display, root, keyClose(), defaultModifier()|ShiftMask);

    for (int i = 0; i < workspaceCount; ++i)
        hotkeys.emplace_back(display, root, keyWorkspace(i), defaultModifier());
}

KeyGrabber::~KeyGrabber() {
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