#include "wmframe.h"


WmFrame::WmFrame() {
}

WmFrame::~WmFrame() {
}

double WmFrame::splitRatio() {
    return splitRatio_;
}

void WmFrame::splitRatio(double newRatio) {
    splitRatio_ = newRatio;
}

WmContainer* WmFrame::parent() {
    return parent_;
}

void WmFrame::parent(WmContainer* parent) {
    parent_ = parent;
}

Geometry& WmFrame::geometry() {
    return geometry_;
}

void WmFrame::realign() {
    relocate(geometry_.x, geometry_.y, geometry_.w, geometry_.h);
}