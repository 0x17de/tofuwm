#include <sstream>
#include <containers/wmsplitter.h>
#include "keygrabber.h"
#include "windowmanager/windowmanager.h"


using namespace std;


KeyGrabber::KeyGrabber(WindowManager* wm, int workspaceCount) :
wm(wm),
workspaceCount(workspaceCount)
{
    const unsigned int defaultModifier = Mod4Mask;

    // @CONFIGURE: Choose hotkeys with their actions.

    // Move active window
    hotbuttons.emplace_back(wm->display, wm->root,
            1, defaultModifier, ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
            [=] { wm->onMousePress(); },
            [=] { wm->onMouseRelease(); },
            [=] { wm->onMouseMotion(); });
    // Resize active window
    hotbuttons.emplace_back(wm->display, wm->root,
            3, defaultModifier, ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
            [=] { wm->onMousePress(); },
            [=] { wm->onMouseRelease(); },
            [=] { wm->onMouseMotion(); });

    // Reload window manager
    hotkeys.emplace_back(wm->display, wm->root,
            key("r"), defaultModifier|ShiftMask,
            [=] {
                wm->reload();
            }, nullptr);
    // Change tiling mode (horizontal/vertical)
    hotkeys.emplace_back(wm->display, wm->root,
            key("w"), defaultModifier,
            [=] {
                wm->changeSplitterDirectionOfWindow(wm->currentWindow_);
            }, nullptr);
    // Toggle window mode (tiling/floating)
    hotkeys.emplace_back(wm->display, wm->root,
            key("e"), defaultModifier,
            [=] {
                if (wm->currentWindow_)
                    wm->currentWorkspace_->toggleWindowMode(wm->currentWindow_);
            }, nullptr);
    // Spawn dmenu
    hotkeys.emplace_back(wm->display, wm->root,
            key("d"), defaultModifier,
            [=] {
                wm->addDebugText("DMENU SPAWN", LogLevel::Verbose);
                char *const paramList[] = {(char *) "dmenu_run", 0};
                wm->spawn("/usr/bin/dmenu_run", paramList);
            }, nullptr);
    // Spawn terminal
    hotkeys.emplace_back(wm->display, wm->root,
            key("t"), defaultModifier,
            [=] {
                wm->addDebugText("URXVT SPAWN", LogLevel::Verbose);
                char *const paramList[] = {(char *) "urxvt", 0};
                wm->spawn("/usr/bin/urxvt", paramList);
            }, nullptr);
    // Lock screen
    hotkeys.emplace_back(wm->display, wm->root,
            key("l"), defaultModifier,
            [=] {
                wm->addDebugText("SLOCK SPAWN", LogLevel::Verbose);
                char *const paramList[] = {(char *) "slock", 0};
                wm->spawn("/usr/bin/slock", paramList);
            }, nullptr);
    // Close active window
    hotkeys.emplace_back(wm->display, wm->root,
            key("q"), defaultModifier|ShiftMask,
            [=] {
                wm->addDebugText("WINDOW CLOSE", LogLevel::Verbose);
                if (wm->currentWindow_)
                    wm->currentWindow_->close();
                wm->selectNewCurrentWindow();
            }, nullptr);

    const char* const workspaceKeys[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    for (int i = 0; i < workspaceCount; ++i) {
        hotkeys.emplace_back(wm->display, wm->root,
                key(workspaceKeys[i]), defaultModifier,
                [=] {
                    stringstream ss;
                    ss << "WORKSPACE " << (i+1);
                    wm->addDebugText(ss.str(), LogLevel::Verbose);
                    wm->changeWorkspace(i);
                }, nullptr);
    }
}

KeyGrabber::~KeyGrabber() {
}

unsigned int KeyGrabber::key(const std::string& character) {
    return XKeysymToKeycode(wm->display, XStringToKeysym(character.c_str()));
}
