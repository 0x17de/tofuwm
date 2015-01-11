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
    WmSplitter(WmSplitterType splitterType);
    virtual ~WmSplitter();

    WmSplitterType splitterType();
    void splitterType(WmSplitterType splitterType);

    void relocate(int x, int y, int width, int height) override;

    virtual WmFrameType containerType();
};


#endif // WMSPLITTER_H