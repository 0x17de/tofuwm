#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <list>
#include <memory>
#include "containers/wmwindow.h"

enum class WorkspaceMode {
    Floating,
    Tiling
};

enum class WorkspaceTilingMode {
    Horizontal,
    Vertical
};

class Workspace {
    // New windows are created in this mode
    WorkspaceMode workspaceMode_ = WorkspaceMode::Floating;
    WorkspaceTilingMode workspaceTilingMode_ = WorkspaceTilingMode::Horizontal;
    WindowManager* wm;

    void addWindowToTiling(WmWindow* w);
    std::shared_ptr<WmContainer> createNewContainer();
public:

    std::list<WmWindow*> floatingWindows;
    std::list<WmWindow*> windows;
    WmWindow* lastActiveTiledWindow = 0;
    std::shared_ptr<WmContainer> rootContainer;

    Workspace(WindowManager* wm);
    ~Workspace();

    WorkspaceMode workspaceMode();
    void workspaceMode(WorkspaceMode workspaceMode);
    void hide();
    void show();

    void addWindow(WmWindow* w);
    void removeWindow(WmWindow* w);
    void toggleWindowMode(WmWindow* w);
};

#endif // WORKSPACE_H