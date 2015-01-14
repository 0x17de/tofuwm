#ifndef WMCONTAINER_H
#define WMCONTAINER_H


#include <list>
#include <memory>
#include "wmframe.h"


class WmWindow;

class WmContainer : public WmFrame {
    WmContainer* parentContainer_ = 0;
    std::list<WmFrame*> children_;
public:
    WmContainer();
    virtual ~WmContainer();

    WmContainer* parentContainer();
    void parentContainer(WmContainer* parentContainer);
    std::list<WmFrame*>& children();
    void swapChildren(WmContainer* other);

    size_t size();
    void add(WmFrame* frame, WmFrame* after = 0);
    void remove(WmFrame* frame);
    virtual void relocate(int x, int y, int width, int height) = 0;
    virtual WmFrameType containerType() = 0;
};


#endif // WMCONTAINER_H