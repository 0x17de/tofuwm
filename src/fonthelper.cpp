#include <iostream>
#include <stdexcept>
#include "fonthelper.h"


using namespace std;


FontHelper::FontHelper(Display* display) :
display(display)
{
    const char* fontname = "-*-helvetica-*-r-*-*-14-*-*-*-*-*-*-*";
    font = XLoadQueryFont(display, fontname);
    if (!font) {
        cout << "Unable to load desired font!" << endl;
        font = XLoadQueryFont(display, "fixed");

        if (!font)
            throw runtime_error("Was not able to load any font.");
    }
}

FontHelper::~FontHelper() {
}

void FontHelper::setFont(GC gc) {
    XSetFont(display, gc, font->fid);
}

XFontStruct* FontHelper::get() {
    return font;
}