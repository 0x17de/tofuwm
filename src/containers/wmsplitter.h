#ifndef WMSPLITTER_H
#define WMSPLITTER_H


#include <list>
#include <memory>
#include "wmcontainer.h"


enum class WmSplitterType {
    Horizontal,
    Vertical
};

class WmSplitter : public WmContainer {
    WmSplitterType splitterType_;
public:
    std::list<std::shared_ptr<WmFrame>> children;

    WmSplitter(WmSplitterType splitterType);
    ~WmSplitter();

    WmSplitterType splitterType();

    virtual size_t size();
    void add(std::shared_ptr<WmFrame> frame) override;
    void remove(WmFrame* frame) override;
    void relocate(int x, int y, int width, int height) override;

    virtual WmFrameType containerType();
};


#endif // WMSPLITTER_H