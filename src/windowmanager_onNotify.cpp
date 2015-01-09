#include <X11/Xlib.h>
#include "windowmanager.h"


using namespace std;


void WindowManager::onCirculateNotify() {
}

void WindowManager::onConfigureNotify() {
}

void WindowManager::onDestroyNotify() {
    if (currentWindow == event.xdestroywindow.window)
        currentWindow = 0;
    windows.erase(event.xdestroywindow.window);
    // @TODO: Select new currentWindow
}

void WindowManager::onGravityNotify() {
}

void WindowManager::onMapNotify() {
}

void WindowManager::onReparentNotify() {
}

void WindowManager::onUnmapNotify() {
}