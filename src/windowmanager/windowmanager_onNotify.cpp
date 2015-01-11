#include <X11/Xlib.h>
#include "windowmanager.h"


using namespace std;


void WindowManager::onEnterNotify() {
    if (moveWindow)
        return;

    setCurrentWindow(event.xcrossing.window);
    if (!currentWindow)
        return;

    currentWindow->setActive(true);
    if (currentWindow->windowMode == WindowMode::Tiled)
        currentWorkspace->lastActiveTiledWindow = currentWindow;
}

void WindowManager::onLeaveNotify() {
    if (!currentWindow || moveWindow)
        return;

    setCurrentWindow(nullptr);
}

void WindowManager::onCirculateNotify() {
}

void WindowManager::onConfigureNotify() {
}

void WindowManager::onDestroyNotify() {
    WmWindow* destroyedWindow = currentWindow;
    if (!destroyedWindow)
        destroyedWindow = findWindow(event.xdestroywindow.window);

    bool needNewCurrentWindow = currentWindow == destroyedWindow;
    if (needNewCurrentWindow)
        currentWindow = 0;

    if (destroyedWindow == currentWorkspace->lastActiveTiledWindow)
        currentWorkspace->lastActiveTiledWindow = 0; // @TODO: Select next best tiled window

    destroyedWindow->setWorkspace(0); // remove from tiling
    destroyedWindow->window = 0; // as the window is already destroyed

    // Cleanup struct
    auto it = windows.find(event.xdestroywindow.window);
    if (it != windows.end())
        windows.erase(event.xdestroywindow.window);
    auto it2 = windows.find(destroyedWindow->frame);
    if (it2 != windows.end())
        windows.erase(it2);

    if (needNewCurrentWindow)
        selectNewCurrentWindow();
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
        selectNewCurrentWindow();
}

void WindowManager::onPropertyNotify() {
    // @TODO: Actions
}