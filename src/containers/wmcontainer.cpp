#include "wmcontainer.h"


using namespace std;


WmContainer::WmContainer() {
}

WmContainer::~WmContainer() {
}

WmContainer* WmContainer::parentContainer() {
    return parentContainer_;
}

void WmContainer::parentContainer(WmContainer* parentContainer) {
    parentContainer_ = parentContainer;
}

std::list<WmFrame*>& WmContainer::children() {
    return children_;
}

void WmContainer::swapChildren(WmContainer* other) {
    children_.swap(other->children_);
    for (WmFrame* child : children_)
        child->parent(this);
    for (WmFrame* child : other->children_)
        child->parent(other);
}

size_t WmContainer::size() {
    return children_.size();
}

void WmContainer::add(WmFrame* frame, WmFrame* after) {
    frame->parent(this);

    if (children_.size() > 0) {
        // Calculate scale of new window
        double sum = 0.0;
        for (auto &child : children_) {
            sum += child->splitRatio();
        }
        frame->splitRatio(sum / children_.size());
    }

    if (after) {
        // Try to insert after specified element
        for (auto it = begin(children_); it != end(children_); ++it) {
            if (*it == after) {
                ++it; // To insert after it.
                children_.insert(it, frame);
                return;
            }
        }
    }
    // If not found, it will be inserted at the end
    children_.push_back(frame);
}

void WmContainer::remove(WmFrame* frame) {
    for (auto it = begin(children_); it != end(children_); ++it) {
        if (*it == frame)
            it = children_.erase(it);
    }
    frame->parent(nullptr);
}