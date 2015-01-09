#ifndef FONTHELPER_H
#define FONTHELPER_H


#include <X11/Xlib.h>


class FontHelper {
    XFontStruct* font;
    Display* display;
public:
    FontHelper(Display* display);
    ~FontHelper();

    void setFont(GC gc);
    XFontStruct* get();
};


#endif // FONTHELPER_H