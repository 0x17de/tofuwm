#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <list>
#include <memory>
#include "containers/wmwindow.h"

class Workspace {
public:
    std::list<WmWindow*> windows;

    Workspace();
    ~Workspace();

    void hide();
    void show();

    void addWindow(WmWindow* w);
    void removeWindow(WmWindow* w);
};

#endif // WORKSPACE_H