
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
                addDebugText("EVENT MapRequest", LogLevel::VeryVerbose);
                onMapRequest(); break;
            case EnterNotify:
                addDebugText("EVENT EnterNotify", LogLevel::VeryVerbose);
                onEnterNotify(); break;
            case LeaveNotify:
                addDebugText("EVENT LeaveNotify", LogLevel::VeryVerbose);
                onLeaveNotify(); break;
            case ConfigureRequest:
                addDebugText("EVENT ConfigureRequest", LogLevel::VeryVerbose);
                onConfigureRequest(); break;
            case ResizeRequest:
                addDebugText("EVENT ResizeRequest", LogLevel::VeryVerbose);
                onResizeRequest(); break;
            case CirculateRequest:
                addDebugText("EVENT CirculateRequest", LogLevel::VeryVerbose);
                onCirculateRequest(); break;
            case KeyPress:
                addDebugText("EVENT KeyPress", LogLevel::VeryVerbose);
                onKeyPress(); break;
            case KeyRelease:
                addDebugText("EVENT KeyRelease", LogLevel::VeryVerbose);
                onKeyRelease(); break;
            case ButtonPress:
                addDebugText("EVENT ButtonPress", LogLevel::VeryVerbose);
                onButtonPress(); break;
            case ButtonRelease:
                addDebugText("EVENT ButtonRelease", LogLevel::VeryVerbose);
                onButtonRelease(); break;
            case MotionNotify:
                addDebugText("EVENT MotionNotify", LogLevel::VeryVerbose);
                onMotion(); break;
            case CirculateNotify:
                addDebugText("EVENT CirculateNotify", LogLevel::VeryVerbose);
                onCirculateNotify(); break;
            case ConfigureNotify:
                addDebugText("EVENT ConfigureNotify", LogLevel::VeryVerbose);
                onConfigureNotify(); break;
            case DestroyNotify:
                addDebugText("EVENT DestroyNotify", LogLevel::VeryVerbose);
                onDestroyNotify(); break;
            case GravityNotify:
                addDebugText("EVENT GravityNotify", LogLevel::VeryVerbose);
                onGravityNotify(); break;
            case MapNotify:
                addDebugText("EVENT MapNotify", LogLevel::VeryVerbose);
                onMapNotify(); break;
            case ReparentNotify:
                addDebugText("EVENT ReparentNotify", LogLevel::VeryVerbose);
                onReparentNotify(); break;
            case UnmapNotify:
                addDebugText("EVENT UnmapNotify", LogLevel::VeryVerbose);
                onUnmapNotify(); break;
            case ClientMessage:
                addDebugText("EVENT ClientMessage", LogLevel::VeryVerbose);
                onClientMessage(); break;
        }
        printDebugText();
    }
}