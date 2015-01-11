#include "wmsplitter.h"


WmSplitter::WmSplitter(WmSplitterType splitterType) :
splitterType_(splitterType)
{
}

WmSplitter::~WmSplitter() {
}

WmFrameType WmSplitter::containerType() {
    return WmFrameType::Splitter;
}

WmSplitterType WmSplitter::splitterType() {
    return splitterType_;
}

size_t WmSplitter::size() {
    return children.size();
}

void WmSplitter::add(std::shared_ptr<WmFrame> frame) {
    frame->parent(this);
    children.push_back(frame);
}

void WmSplitter::remove(WmFrame* frame) {
    for (auto it = begin(children); it != end(children); ++it) {
        if ((*it).get() == frame)
            children.erase(it);
    }
    frame->parent(0);
}

void WmSplitter::relocate(int x, int y, int width, int height) {
    if (children.size() == 0)
        return;

    // normalize values
    std::list<double> scale;
    double sum = 0.0;
    for (std::shared_ptr<WmFrame>& child : children) {
        const double splitRatio = child->splitRatio();
        scale.push_back(splitRatio);
        sum += splitRatio;
    }

    if (sum == 0.0)
        return;

    for (std::shared_ptr<WmFrame>& child : children) {
        // normalize to one
        const double splitRatio = child->splitRatio() / sum;
        child->splitRatio(splitRatio); // save

        int subWidth, subHeight;
        if (splitterType_ == WmSplitterType::Horizontal) {
            subWidth = width;
            subHeight = height * splitRatio;
        } else {
            subWidth = width * splitRatio;
            subHeight = y;
        }

        Geometry& g = child->geometry();
        g.x = x;
        g.y = y;
        g.h = subHeight;
        g.w = subWidth;

        child->realign();


        if (splitterType_ == WmSplitterType::Horizontal) {
            y += subHeight;
        } else {
            x += subWidth;
        }
    }
}