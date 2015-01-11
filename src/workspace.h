#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <list>
#include <memory>
#include "containers/wmwindow.h"

enum class WorkspaceMode {
    Floating,
    Tiling
};

class Workspace {
    // New windows are created in this mode
    WorkspaceMode workspaceMode_ = WorkspaceMode::Floating;
public:
    std::list<WmWindow*> windows;

    Workspace();
    ~Workspace();

    WorkspaceMode workspaceMode();
    void hide();
    void show();

    void addWindow(WmWindow* w);
    void removeWindow(WmWindow* w);
};

#endif // WORKSPACE_H