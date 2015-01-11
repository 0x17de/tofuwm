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
    std::string wmname = "tofuwm";

    std::shared_ptr<Display> displayPtr;
public:
    Display* display;
    Window root;
    std::shared_ptr<KeyGrabber> keyGrabber;
    std::shared_ptr<FontHelper> fontHelper;

    // Viewport
public:
    Geometry desktop; // For using docks, blocked areas
private:

    // Move window
    WmWindow* moveWindow = 0;
    XButtonEvent moveWindowStart = {0};
    XWindowAttributes moveWindowAttributes = {0};
    // Resize window
    bool moveWindowExpandXPositive;
    bool moveWindowExpandYPositive;

    Cursor cursor;
    XEvent event;

    WmWindow* currentWindow = 0;
    std::vector<Workspace> workspaces;
    Workspace* currentWorkspace;

    std::map<Window, std::shared_ptr<WmWindow>> windows;
    std::list<std::shared_ptr<WmWindow>> dockedWindows;

    // === Methods
    void setErrorHandler();
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
    void removeDestroyedWindow(Window window);
    void removeWindow(WmWindow* window);
    WmWindow* findWindow(Window window);
    void selectNewCurrentWindow();
    void changeSplitterDirectionOfWindow(WmWindow* window);
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
    void onPropertyNotify();

    void onClientMessage();
    void onNetWmState();
    void onNetRequestFrameExtents();
    void onNetRequestActiveWindow();

public:
    WindowManager();
    ~WindowManager();

    void run();

    friend class KeyGrabber;
};

#endif // WINDOWMANAGER_H