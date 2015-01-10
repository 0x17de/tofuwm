#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <list>
#include <map>
#include <memory>
#include <X11/Xlib.h>
#include "keygrabber.h"
#include "workspace.h"
#include "wmwindow.h"
#include "fonthelper.h"

struct Geometry {
    int x, y, w, h;
};

class WindowManager {
private:
    bool running = true;
    std::list<std::string> debugStrings;

    // Viewport
    Geometry desktop; // For using docks, blocked areas
    std::list<std::shared_ptr<WmWindow>> dockedWindows;

    // Move window
    WmWindow* moveWindow = 0;
    XButtonEvent moveWindowStart = {0};
    XWindowAttributes moveWindowAttributes = {0};
    // Resize window
    bool moveWindowExpandXPositive;
    bool moveWindowExpandYPositive;

    // Default
    Cursor cursor;
    XEvent event;

    std::shared_ptr<Display> displayPtr;
    Display* display;
    std::shared_ptr<KeyGrabber> keyGrabber;
    std::shared_ptr<FontHelper> fontHelper;

    Window root;
    WmWindow* currentWindow;
    std::map<Window, std::shared_ptr<WmWindow>> windows;

    // Workspaces
    Workspace workspaces[2];
    Workspace* currentWorkspace;

    // === Methods
    void initCursor();
    void initBackground();
    void changeWorkspace(int number);
    void loop();
    void calculateDesktopSpace();
    void spawn(const std::string& cmd, char *const argv[]);
    void setCurrentWindow(Window window);
    WmWindow* findWindow(Window window);

    // === Debug
    void printDebugText();
    void addDebugText(const std::string& text);

    // === Events & Notifications
    void onMotion();
    void onEnter();
    void onLeave();
    void onKeyPress();
    void onKeyRelease();
    void onButtonPress();
    void onButtonRelease();
    void onConfigureRequest();
    void onResizeRequest();
    void onCirculateRequest();
    void onMapRequest();

    void onCirculateNotify();
    void onConfigureNotify();
    void onDestroyNotify();
    void onGravityNotify();
    void onMapNotify();
    void onReparentNotify();
    void onUnmapNotify();

public:
    WindowManager();
    ~WindowManager();

    void run();
};

#endif // WINDOWMANAGER_H