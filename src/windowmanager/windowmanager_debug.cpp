#include <iostream>
#include <stdexcept>
#include <string>
#include <X11/Xlib.h>
#include <sstream>

#include "windowmanager.h"


using namespace std;


void WindowManager::debugPrintUnknownAtom(Atom atom, LogLevel level) throw() {
    if (level > currentLogLevel_)
        return;

    string name = getAtomName(atom);
    stringstream ss;
    ss << "UnkCMsg ATOM " << name;
    addDebugText(ss.str(), level);
}

void WindowManager::addDebugText(const std::string& text, LogLevel level) {
    if (level > currentLogLevel_)
        return;

    static string lastText;
    if (lastText == text) {
        debugStrings_.front() += ".";
        cout << ".";
        cout.flush();
    } else {
        debugStrings_.push_front(text);
        cout << endl << text;
        cout.flush();
        if (debugStrings_.size() > 30)
            debugStrings_.pop_back();
        lastText = text;
    }
}

void WindowManager::printDebugText() {
    GC gc = XDefaultGC(display, XDefaultScreen(display));
    fontHelper->setFont(gc);

    XClearWindow(display, root);

    int y = 40;
    for (string &text : debugStrings_) {
        XDrawString(display, root, gc, 20, y, text.c_str(), text.length());
        y += 16;
    }
}