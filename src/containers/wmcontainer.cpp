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

std::list<std::shared_ptr<WmFrame>>& WmContainer::children() {
    return children_;
}

void WmContainer::swapChildren(WmContainer* other) {
    children_.swap(other->children_);
    for (shared_ptr<WmFrame>& child : children_)
        child->parent(this);
    for (shared_ptr<WmFrame>& child : other->children_)
        child->parent(other);
}

size_t WmContainer::size() {
    return children_.size();
}

void WmContainer::add(std::shared_ptr<WmFrame> frame, WmFrame* after) {
    frame->parent(this);

    if (after) {
        // Try to insert after specified element
        for (auto it = begin(children_); it != end(children_); ++it) {
            if (it->get() == after) {
                it = children_.insert(it, frame);
                return;
            }
        }
    }
    // If not found, it will be inserted at the end
    children_.push_back(frame);
}

void WmContainer::remove(WmFrame* frame) {
    for (auto it = begin(children_); it != end(children_); ++it) {
        if ((*it).get() == frame)
            it = children_.erase(it);
    }
    frame->parent(nullptr);
}