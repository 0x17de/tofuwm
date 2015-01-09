#ifndef DISPLAY_H
#define DISPLAY_H

struct Free_XCloseDisplay {
    inline void operator()(Display* display) {
        XCloseDisplay(display);
    }
};

#endif // DISPLAY_H