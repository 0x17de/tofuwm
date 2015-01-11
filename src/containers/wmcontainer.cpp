#include "wmcontainer.h"

WmContainer* WmContainer::parentContainer() {
    return parentContainer_;
}

void WmContainer::parentContainer(WmContainer* parentContainer) {
    parentContainer_ = parentContainer;
}
