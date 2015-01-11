#include <X11/Xlib.h>
#include "windowmanager.h"


using namespace std;


void WindowManager::onEnterNotify() {
    if (event.xcrossing.mode != NotifyNormal)
        return;

    setCurrentWindow(event.xcrossing.window);
    if (!currentWindow)
        return;

    currentWindow->setActive(true);
    if (currentWindow->windowMode == WindowMode::Tiled)
        currentWorkspace->lastActiveTiledWindow = currentWindow;
}

void WindowManager::onLeaveNotify() {
    if (event.xcrossing.mode != NotifyNormal)
        return;
    setCurrentWindow(nullptr);
}

void WindowManager::onCirculateNotify() {
}

void WindowManager::onConfigureNotify() {
}

void WindowManager::onDestroyNotify() {
    removeDestroyedWindow(event.xdestroywindow.window);
}

void WindowManager::onGravityNotify() {
}

void WindowManager::onReparentNotify() {
}

void WindowManager::onMapNotify() {
    WmWindow* w = findWindow(event.xunmap.window);
    if (!w) return;
    w->isMapped = true;
}

void WindowManager::onUnmapNotify() {
    WmWindow* w = findWindow(event.xunmap.window);
    if (!w
     || w->frame == event.xunmap.window
     || w->window != event.xunmap.window)
        return;

    w->isMapped = false;
    removeWindow(w);

    if (w == currentWindow)
        selectNewCurrentWindow();
}

void WindowManager::onPropertyNotify() {
    // @TODO: Actions
}