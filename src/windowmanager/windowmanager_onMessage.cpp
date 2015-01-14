#include <sstream>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "windowmanager.h"


using namespace std;


/*
@TODO: Check for more required messages
http://standards.freedesktop.org/wm-spec/wm-spec-1.3.html

_NET_ACTIVE_WINDOW

_NET_SUPPORTED
_NET_CLIENT_LIST
_NET_NUMBER_OF_DESKTOPS
_NET_DESKTOP_GEOMETRY
_NET_DESKTOP_VIEWPORT
_NET_CURRENT_DESKTOP
_NET_DESKTOP_NAMES
_NET_WORKAREA
_NET_SUPPORTING_WM_CHECK
_NET_VIRTUAL_ROOTS
_NET_DESKTOP_LAYOUT
_NET_SHOWING_DESKTOP
*/

void WindowManager::onClientMessage() {
    static Atom A__NET_WM_STATE = getAtom("_NET_WM_STATE");
    static Atom A__NET_ACTIVE_WINDOW = getAtom("_NET_ACTIVE_WINDOW");
    static Atom A__NET_REQUEST_FRAME_EXTENTS = getAtom("_NET_REQUEST_FRAME_EXTENTS");

    if (A__NET_WM_STATE == event_.xclient.message_type) {
        addDebugText("onNetWmState", LogLevel::VeryVerbose);
        onNetWmState();
    } else if (A__NET_REQUEST_FRAME_EXTENTS == event_.xclient.message_type) {
        addDebugText("onNetRequestFrameExtents", LogLevel::VeryVerbose);
        onNetRequestFrameExtents();
    } else if (A__NET_ACTIVE_WINDOW == event_.xclient.message_type) {
        addDebugText("onNetRequestActiveWindow", LogLevel::VeryVerbose);
        onNetRequestActiveWindow();
    } else {
        debugPrintUnknownAtom(event_.xclient.message_type, LogLevel::Warning);
    }
}

void WindowManager::onNetWmState() {
    // @TODO: onNetWmState
}

void WindowManager::onNetRequestFrameExtents() {
    static Atom A__NET_FRAME_EXTENTS = getAtom("_NET_FRAME_EXTENTS");

    int32_t r[4]{2, 2, 2, 2};
    XChangeProperty(display, event_.xclient.window,
            A__NET_FRAME_EXTENTS, XA_CARDINAL,
            32, PropModeReplace,
            (unsigned const char*)r, 4);
}

void WindowManager::onNetRequestActiveWindow() {
    // @TODO: onNetRequestActiveWindow
}
