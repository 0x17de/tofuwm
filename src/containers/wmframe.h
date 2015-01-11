#ifndef WMFRAME_H
#define WMFRAME_H


#include "geometry.h"


enum class WmFrameType {
    Window,
    Frame,
    Splitter
};

class WmFrame {
private:
    double splitRatio_ = 1.0;
    Geometry geometry_ = {-1, -1, -1, -1};
public:
    virtual WmFrameType containerType() = 0;
    void realign();
    virtual void relocate(int x, int y, int width, int height) = 0;

    Geometry& geometry();
    double splitRatio();
    void splitRatio(double newRatio);
};


#endif // WMFRAME_H