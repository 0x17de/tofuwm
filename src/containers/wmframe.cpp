#include "wmframe.h"


double WmFrame::splitRatio() {
    return splitRatio_;
}

void WmFrame::splitRatio(double newRatio) {
    splitRatio_ = newRatio;
}

Geometry& WmFrame::geometry() {
    return geometry_;
}

void WmFrame::realign() {
    relocate(geometry_.x, geometry_.y, geometry_.w, geometry_.h);
}