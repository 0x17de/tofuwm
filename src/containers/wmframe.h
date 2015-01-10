#ifndef WMFRAME_H
#define WMFRAME_H


#include "wmcontainer.h"


class WmFrame : public WmContainer {
    virtual WmContainerType containerType();
};


#endif // WMFRAME_H