#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <memory>
#include <X11/Xlib.h>
#include "keygrabber.h"
#include "workspace.h"

class WindowManager {
private:
    bool running = true;

    // Move window
    XButtonEvent start = {0};
    XWindowAttributes attributes = {0};

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

public:
    WindowManager();
    ~WindowManager();

    void run();
};

#endif // WINDOWMANAGER_H