#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <list>
#include <memory>
#include "wmwindow.h"

class Workspace {
    std::list<std::shared_ptr<WmWindow>> windows;

public:
    Workspace();
    ~Workspace();

    void hide();
    void show();

    void addWindow(std::shared_ptr<WmWindow> w);
};

#endif // WORKSPACE_H