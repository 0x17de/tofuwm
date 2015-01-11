#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <list>
#include <map>
#include <vector>
#include <memory>
#include <X11/Xlib.h>
#include "keygrabber.h"
#include "workspace.h"
#include "containers/wmwindow.h"
#include "containers/wmcontainer.h"
#include "fonthelper.h"
#include "geometry.h"


class WindowManager {
private:
    bool running = true;
    std::list<std::string> debugStrings;

    // Viewport
public:
    Geometry desktop; // For using docks, blocked areas
private:
    std::list<std::shared_ptr<WmWindow>> dockedWindows;

    // Move window
    WmWindow* moveWindow = 0;
    XButtonEvent moveWindowStart = {0};
    XWindowAttributes moveWindowAttributes = {0};
    // Resize window
    bool moveWindowExpandXPositive;
    bool moveWindowExpandYPositive;

    // Default
public:
    Display* display;
    Window root;

private:
    std::string wmname = "tofuwm";
    Cursor cursor;
    XEvent event;

    std::shared_ptr<Display> displayPtr;
    std::shared_ptr<KeyGrabber> keyGrabber;
    std::shared_ptr<FontHelper> fontHelper;

    WmWindow* currentWindow = 0;
    std::map<Window, std::shared_ptr<WmWindow>> windows;

    // Workspaces
    std::vector<Workspace> workspaces;
    Workspace* currentWorkspace;

    // === Methods
    void setWmName();
    void initCursor();
    void initBackground();
    void addExistingWindows() throw();
    void selectInput(int mask);
    void selectNoInput();
    void selectDefaultInput();
    void changeWorkspace(int number);
    void loop();
    void calculateDesktopSpace();
    void spawn(const std::string& cmd, char *const argv[]);
    WmWindow* addWindow(Window window);
    WmWindow* findWindow(Window window);
    void selectNewCurrentWindow();
    static inline constexpr int workspaceCount() { return 10; }
public:
    void setCurrentWindow(Window window);
    void setCurrentWindow(WmWindow* window);
private:

public:
    Atom getAtom(const std::string& protocol);

private:
    // === Debug
public:
    void printDebugText();
    void addDebugText(const std::string& text);
    void debugPrintUnknownAtom(Atom atom) throw();
private:
    // === Events & Notifications
    void onMotion();
    void onEnterNotify();
    void onLeaveNotify();
    void onKeyPress();
    void onKeyRelease();
    void onButtonPress();
    void onButtonRelease();
    void onMousePress();
    void onMouseRelease();
    void onMouseMotion();
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

    void onClientMessage();
    void onNetWmState();
    void onNetRequestFrameExtents();
    void onNetRequestActiveWindow();

public:
    WindowManager();
    ~WindowManager();

    void run();

    friend KeyGrabber;
};

#endif // WINDOWMANAGER_H