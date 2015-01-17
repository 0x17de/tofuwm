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


enum class LogLevel {
    Silent = 0,
    Critical = 1,
    Warning = 2,
    Info = 4,
    Verbose = 4,
    VeryVerbose = 5
};

enum class WmDirection {
    Up, Right, Down, Left
};

enum class WmStopAction {
    Stop,
    Reload,
    Restart
};

class Options;
class WindowManager {
private:
    const Options* const options_;
    bool running_ = true;
    WmStopAction stopAction_ = WmStopAction::Stop;
    std::list<std::string> debugStrings_;
    std::string wmname_ = "tofuwm";
    LogLevel currentLogLevel_ = LogLevel::Warning;

    std::shared_ptr<Display> displayPtr_;
public:
    Display* display;
    Window root;
    std::shared_ptr<KeyGrabber> keyGrabber;
    std::shared_ptr<FontHelper> fontHelper;

    // Viewport
public:
    Geometry desktop_; // For using docks, blocked areas
private:

    // Move window
    WmWindow* moveWindow_ = 0;
    XButtonEvent moveWindowStart_ = {0};
    XWindowAttributes moveWindowAttributes_ = {0};
    // Resize window
    bool moveWindowExpandXPositive_;
    bool moveWindowExpandYPositive_;

    Cursor cursor_;
    XEvent event_;

    WmWindow* currentWindow_ = 0;
    std::vector<Workspace> workspaces_;
    Workspace*currentWorkspace_;

    std::map<Window, std::shared_ptr<WmWindow>> windows_;
    std::list<std::shared_ptr<WmWindow>> dockedWindows_;

    // === Methods
    void stop();
    void reload();
    void restart();

    void onFirstLoad();
    void onLoad();
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
    WmWindow*manageWindow(Window window);
    void removeDestroyedWindow(Window window);
    void unmanageWindow(WmWindow *window);
    WmWindow* findWindow(Window window);
    void selectNewCurrentWindow();
    void changeSplitterDirectionOfWindow(WmWindow* window);
    static inline constexpr int workspaceCount() { return 10; }
public:
    void setCurrentWindow(Window window);
    void setCurrentWindow(WmWindow* window);

    void changeWindowSelection(WmDirection direction);
    void moveWindow(WmDirection direction);
private:

public:
    Atom getAtom(const std::string& protocol);
    std::string getAtomName(Atom atom);

private:
    // === Debug
public:
    void printDebugText();
    void addDebugText(const std::string& text, LogLevel level);
    void debugPrintUnknownAtom(Atom atom, LogLevel level) throw();
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
    WindowManager(const Options* const options);
    ~WindowManager();

    WmStopAction run();

    friend class KeyGrabber;
};

#endif // WINDOWMANAGER_H