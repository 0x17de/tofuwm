#ifndef WMFRAME_H
#define WMFRAME_H


#include "geometry.h"


enum class WmFrameType {
    Window,
    Frame,
    Splitter
};

class WmContainer;
class WmFrame {
private:
    double splitRatio_ = 1.0;
    Geometry geometry_ = {-1, -1, -1, -1};
    WmContainer* parent_ = 0;
public:
    WmFrame();
    virtual ~WmFrame();

    virtual WmFrameType containerType() = 0;
    virtual void realign();
    virtual void relocate(int x, int y, int width, int height) = 0;

    WmContainer* parent();
    void parent(WmContainer* parent);
    Geometry& geometry();
    double splitRatio();
    void splitRatio(double newRatio);
};


#endif // WMFRAME_H