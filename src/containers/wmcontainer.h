#ifndef WMCONTAINER_H
#define WMCONTAINER_H


enum class WmContainerType {
    Window,
    Frame,
    Splitter
};

class WmContainer {
    virtual WmContainerType containerType() = 0;
};


#endif // WMCONTAINER_H