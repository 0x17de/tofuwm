
#include <iostream>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <unistd.h>
#include <sstream>

#include "windowmanager.h"
#include "display.h"


using namespace std;


void WindowManager::loop() {
    // Loop
    printDebugText();
    while(running) {
        XNextEvent(display, &event);
        switch (event.type) {
            case MapRequest:
                addDebugText("EVENT MapRequest");
                onMapRequest(); break;
            case EnterNotify:
                addDebugText("EVENT EnterNotify");
                onEnter(); break;
            case LeaveNotify:
                addDebugText("EVENT LeaveNotify");
                onLeave(); break;
            case ConfigureRequest:
                addDebugText("EVENT ConfigureRequest");
                onConfigureRequest(); break;
            case ResizeRequest:
                addDebugText("EVENT ResizeRequest");
                onResizeRequest(); break;
            case CirculateRequest:
                addDebugText("EVENT CirculateRequest");
                onCirculateRequest(); break;
            case KeyPress:
                addDebugText("EVENT KeyPress");
                onKeyPress(); break;
            case ButtonPress:
                addDebugText("EVENT ButtonPress");
                onButtonPress(); break;
            case MotionNotify:
                addDebugText("EVENT MotionNotify");
                onMotion(); break;
            case ButtonRelease:
                addDebugText("EVENT ButtonRelease");
                onButtonRelease(); break;
            case CirculateNotify:
                addDebugText("EVENT CirculateNotify");
                onCirculateNotify(); break;
            case ConfigureNotify:
                addDebugText("EVENT ConfigureNotify");
                onConfigureNotify(); break;
            case DestroyNotify:
                addDebugText("EVENT DestroyNotify");
                onDestroyNotify(); break;
            case GravityNotify:
                addDebugText("EVENT GravityNotify");
                onGravityNotify(); break;
            case MapNotify:
                addDebugText("EVENT MapNotify");
                onMapNotify(); break;
            case ReparentNotify:
                addDebugText("EVENT ReparentNotify");
                onReparentNotify(); break;
            case UnmapNotify:
                addDebugText("EVENT UnmapNotify");
                onUnmapNotify(); break;
        }
        printDebugText();
    }
}