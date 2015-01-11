#include <sstream>
#include "keygrabber.h"
#include "windowmanager/windowmanager.h"


using namespace std;


KeyGrabber::KeyGrabber(WindowManager* wm, int workspaceCount) :
wm(wm),
workspaceCount(workspaceCount)
{
    hotbuttons.emplace_back(wm->display, wm->root,
            1, defaultModifier(), ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
            nullptr, nullptr);
    hotbuttons.emplace_back(wm->display, wm->root,
            3, defaultModifier(), ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
            nullptr, nullptr);

    hotkeys.emplace_back(wm->display, wm->root,
            keyDMenu(), defaultModifier(),
            [=] {
                wm->addDebugText("DMENU SPAWN");
                char *const parmList[] = {(char *) "dmenu_run", 0};
                wm->spawn("/usr/bin/dmenu_run", parmList);
            }, nullptr);
    hotkeys.emplace_back(wm->display, wm->root,
            keyClose(), defaultModifier() | ShiftMask,
            [=] {
                wm->addDebugText("WINDOW CLOSE");
                if (wm->currentWindow)
                    wm->currentWindow->close();
                wm->selectNewCurrentWindow();
            }, nullptr);

    for (int i = 0; i < workspaceCount; ++i) {
        hotkeys.emplace_back(wm->display, wm->root,
                keyWorkspace(i), defaultModifier(),
                [=] {
                    stringstream ss;
                    ss << "WORKSPACE " << (i+1);
                    wm->addDebugText(ss.str());
                    wm->changeWorkspace(i);
                }, nullptr);
    }
}

KeyGrabber::~KeyGrabber() {
}

int KeyGrabber::defaultModifier() {
    return Mod4Mask;
}

int KeyGrabber::keyWorkspace(int number) {
    const char* const workspaceKeys[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    return XKeysymToKeycode(wm->display, XStringToKeysym(workspaceKeys[number]));
}

int KeyGrabber::keyClose() {
    return XKeysymToKeycode(wm->display, XStringToKeysym("q"));
}

int KeyGrabber::keyDMenu() {
    return XKeysymToKeycode(wm->display, XStringToKeysym("d"));
}