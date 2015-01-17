#include <X11/Xlib.h>
#include "windowmanager.h"


using namespace std;


void WindowManager::onEnterNotify() {
    if (event_.xcrossing.mode != NotifyNormal || moveWindow_)
        return;

    setCurrentWindow(event_.xcrossing.window);
    if (!currentWindow_)
        return;

    if (currentWindow_->windowMode == WindowMode::Tiled)
        currentWorkspace_->lastActiveTiledWindow = currentWindow_;
}

void WindowManager::onLeaveNotify() {
    if (event_.xcrossing.mode != NotifyNormal || moveWindow_)
        return;
    setCurrentWindow(nullptr);
}

void WindowManager::onCirculateNotify() {
}

void WindowManager::onConfigureNotify() {
}

void WindowManager::onDestroyNotify() {
    removeDestroyedWindow(event_.xdestroywindow.window);
}

void WindowManager::onGravityNotify() {
}

void WindowManager::onReparentNotify() {
}

void WindowManager::onMapNotify() {
    WmWindow* w = findWindow(event_.xunmap.window);
    if (!w) return;
    w->isMapped = true;
}

void WindowManager::onUnmapNotify() {
    WmWindow* w = findWindow(event_.xunmap.window);
    if (!w
     || w->frame == event_.xunmap.window
     || w->window != event_.xunmap.window)
        return;

    w->isMapped = false;
    unmanageWindow(w);

    if (w == currentWindow_)
        selectNewCurrentWindow();
}

void WindowManager::onPropertyNotify() {
    // @TODO: Actions
}