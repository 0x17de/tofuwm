#ifndef WMWINDOW_H
#define WMWINDOW_H

#include <string>
#include <X11/Xlib.h>

class Workspace;
class WmWindow {
    bool staysFloating_ = false;
public:
    Display* display;
    Window root;
    Window window;
    Window frame;
    Workspace* workspace = 0;

    WmWindow(Display* display, Window root, Window window);
    ~WmWindow();

    void show();
    void hide();
    int minWindowSize();
    void setActive(bool active);
    void setWorkspace(Workspace* newWorkspace);
    bool staysFloating();
    void toggleFloating();
    Atom getAtom(const std::string& protocol);
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