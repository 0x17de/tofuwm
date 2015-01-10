#include <X11/Xlib.h>
#include "windowmanager.h"


using namespace std;


void WindowManager::onEnterNotify() {
    if (moveWindow)
        return;

    setCurrentWindow(event.xcrossing.window);
    currentWindow->setActive(true);
}

void WindowManager::onLeaveNotify() {
    if (!currentWindow || moveWindow)
        return;

    currentWindow->setActive(false);
    setCurrentWindow(0);
}

void WindowManager::onCirculateNotify() {
}

void WindowManager::onConfigureNotify() {
}

void WindowManager::onDestroyNotify() {
    bool newCurrentWindow = currentWindow && *currentWindow == event.xdestroywindow.window;

    auto it = windows.find(event.xdestroywindow.window);
    it->second->window = 0; // as it is already destroyed
    auto it2 = windows.find(it->second->frame);
    windows.erase(it);
    windows.erase(it2);

    if (newCurrentWindow)
        currentWindow = 0;
    // @TODO: Select new currentWindow
}

void WindowManager::onGravityNotify() {
}

void WindowManager::onReparentNotify() {
}

void WindowManager::onMapNotify() {
}

void WindowManager::onUnmapNotify() {
    WmWindow* w = findWindow(event.xunmap.window);
    if (!w) return;

    if (w == currentWindow)
        currentWindow = 0; // @TODO: Select new currentWindow

}