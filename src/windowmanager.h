#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <list>
#include <memory>
#include <X11/Xlib.h>
#include "keygrabber.h"
#include "workspace.h"
#include "wmwindow.h"

struct Geometry {
    int x, y, w, h;
};

class WindowManager {
private:
    bool running = true;

    // Viewport
    Geometry desktop; // For using docks, blocked areas
    std::list<std::shared_ptr<WmWindow>> dockedWindows;

    // Move window
    XButtonEvent moveWindowStart = {0};
    XWindowAttributes moveWindowAttributes = {0};
    // Resize window
    bool moveWindowExpandXPositive;
    bool moveWindowExpandYPositive;

    // Default
    XEvent event;

    std::shared_ptr<Display> displayPtr;
    Window root;
    KeyGrabber keyGrabber;

    // Workspaces
    Workspace workspaces[2];
    Workspace* currentWorkspace;

    // === Methods
    void initBackground();
    void changeWorkspace(int number);
    void loop();
    void calculateDesktopSpace();

    void onMotion();
    void onEnter();
    void onLeave();
    void onKeyPress();
    void onKeyRelease();
    void onButtonPress();
    void onButtonRelease();
    void onConfigureRequest();
    void onCirculateRequest();
    void onMapRequest();

public:
    WindowManager();
    ~WindowManager();

    void run();
};

#endif // WINDOWMANAGER_H