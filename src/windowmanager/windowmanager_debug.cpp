#include <iostream>
#include <stdexcept>
#include <X11/Xlib.h>
#include <sstream>

#include "windowmanager.h"


using namespace std;


void WindowManager::debugPrintUnknownAtom(Atom atom) throw() {
    char* name = XGetAtomName(display, atom);
    stringstream ss;
    ss << "UnkCMsg ATOM " << name;
    addDebugText(ss.str());
    XFree(name);
}

void WindowManager::addDebugText(const std::string& text) {
    static string lastText;
    if (lastText == text) {
        debugStrings.front() += ".";
    } else {
        debugStrings.push_front(text);
        if (debugStrings.size() > 30)
            debugStrings.pop_back();
        lastText = text;
    }
}

void WindowManager::printDebugText() {
    GC gc = XDefaultGC(display, XDefaultScreen(display));
    fontHelper->setFont(gc);

    XClearWindow(display, root);

    int y = 40;
    for (string &text : debugStrings) {
        XDrawString(display, root, gc, 20, y, text.c_str(), text.length());
        y += 16;
    }
}