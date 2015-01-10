#ifndef WMWINDOW_H
#define WMWINDOW_H

#include <string>
#include <X11/Xlib.h>
#include "wmcontainer.h"

class WindowManager;
class Workspace;
class WmWindow : public WmContainer {
    bool staysFloating_ = false;
public:
    WindowManager* wm;
    Window window;
    Window frame;
    Workspace* workspace = 0;

    WmWindow(WindowManager* wm, Window window);
    ~WmWindow();

    virtual WmContainerType containerType();

    void show();
    void hide();
    int minWindowSize();
    void setActive(bool active);
    void setWorkspace(Workspace* newWorkspace);
    bool staysFloating();
    void toggleFloating();
    bool supportsProtocol(Atom protocol) throw ();
    void close();
    void selectNoInput();
    void selectDefaultInput();
    void setDefaultEventMask();

    void resize(int w, int h);
    void relocate(int x, int y, int w, int h);

    bool operator==(const Window& window);
};

#endif // WMWINDOW_H