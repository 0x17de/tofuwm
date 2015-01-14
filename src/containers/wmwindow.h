#ifndef WMWINDOW_H
#define WMWINDOW_H

#include <string>
#include <X11/Xlib.h>
#include "wmcontainer.h"


enum class WindowMode {
    Floating,
    Tiled
};

class WindowManager;
class Workspace;
class WmWindow : public WmFrame {
public:
    WindowManager* wm;
    Window window;
    Window frame;
    Workspace* workspace = 0;
    WindowMode windowMode = WindowMode::Floating;
    bool isMapped = false;

    WmWindow(WindowManager* wm, Window window);
    virtual ~WmWindow();

    virtual WmFrameType containerType();

    void loadWindowProperties();
    void onPropertyChange(Atom atom, int state);

    void show();
    void hide();
    void map();
    int minWindowSize();
    void setActive(bool active);
    bool supportsProtocol(Atom protocol) throw ();
    void close();
    void setDefaultEventMask();
    void setSubstructureEvents(bool enable);

    void resize(int w, int h);
    virtual void realign();
    void relocate(int x, int y, int w, int h);
    void relocate(int x, int y, int w, int h, unsigned int event_mask);

    bool operator==(const Window& window);
};

#endif // WMWINDOW_H