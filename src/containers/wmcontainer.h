#ifndef WMCONTAINER_H
#define WMCONTAINER_H


#include <memory>
#include "wmframe.h"


class WmWindow;

class WmContainer : public WmFrame {
    WmContainer* parentContainer_ = 0;
public:
    WmContainer* parentContainer();
    void parentContainer(WmContainer* parentContainer);

    virtual void add(std::shared_ptr<WmFrame> frame) = 0;
    virtual void remove(std::shared_ptr<WmFrame> frame) = 0;
    virtual void relocate(int x, int y, int width, int height) = 0;
    virtual WmFrameType containerType() = 0;
};


#endif // WMCONTAINER_H