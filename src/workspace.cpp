#include <containers/wmsplitter.h>
#include <X11/Xlib.h>
#include "workspace.h"
#include "windowmanager/windowmanager.h"


using namespace std;


Workspace::Workspace(WindowManager* wm) :
wm(wm)
{
}

Workspace::~Workspace() {
}

WorkspaceMode Workspace::workspaceMode() {
    return workspaceMode_;
}

void Workspace::workspaceMode(WorkspaceMode workspaceMode) {
    workspaceMode_ = workspaceMode;
}

void Workspace::hide() {
    for(WmWindow* w : floatingWindows)
        w->hide();
    for(WmWindow* w : tiledWindows)
        w->hide();
}

void Workspace::show() {
    for(WmWindow* w : floatingWindows)
        w->show();
    for(WmWindow* w : tiledWindows)
        w->show();
}

void Workspace::addWindow(WmWindow* w) {
    if (w->workspace)
        w->workspace->removeWindow(w);
    w->workspace = this;
    if (workspaceMode_ == WorkspaceMode::Floating) {
        w->windowMode = WindowMode::Floating;
        w->addToList(floatingWindows);
        wm->setCurrentWindow(w);
        sendWindowToFront(w);
    } else {
        w->windowMode = WindowMode::Tiled;
        addWindowToTiling(w);
        w->addToList(tiledWindows);
        wm->setCurrentWindow(w);
        sendWindowToBack(w);
    }
}

std::shared_ptr<WmContainer> Workspace::createNewContainer() {
    std::shared_ptr<WmContainer> container;
    switch (workspaceTilingMode_) {
        case WorkspaceTilingMode::Horizontal:
            container = make_shared<WmSplitter>(WmSplitterType::Horizontal);
            break;
        case WorkspaceTilingMode::Vertical:
            container = make_shared<WmSplitter>(WmSplitterType::Vertical);
            break;
    }
    containers.push_back(container);
    Geometry& g = container->geometry();
    g.x = wm->desktop_.x;
    g.y = wm->desktop_.y;
    g.w = wm->desktop_.w;
    g.h = wm->desktop_.h;
    return container;
}

void Workspace::addWindowToTiling(WmWindow* w) {
    WmContainer *container = 0;

    if (lastActiveTiledWindow) {
        container = lastActiveTiledWindow->parent();
    } else if (tiledWindows.size() > 0) {
        container = tiledWindows.back()->parent();
    }

    if (!container) {
        if (!rootContainer)
            rootContainer = createNewContainer();
        container = rootContainer.get();
    }

    container->add(w, lastActiveTiledWindow);
    container->realign();
}

void Workspace::sendWindowToFront(WmWindow* w) {
    XWindowChanges xchanges;
    xchanges.sibling = 0;
    xchanges.stack_mode = Above;
    XConfigureWindow(wm->display, w->frame, CWStackMode, &xchanges);
}
void Workspace::sendWindowToBack(WmWindow* w) {
    XWindowChanges xchanges;
    xchanges.sibling = 0;
    xchanges.stack_mode = Below;
    XConfigureWindow(wm->display, w->frame, CWStackMode, &xchanges);
}

void Workspace::toggleWindowMode(WmWindow* w) { // @TODO: tiled->float does not work correctly -> multiple add remove
    if (w->windowMode == WindowMode::Floating) {
        w->windowMode = WindowMode::Tiled;
        w->removeFromList();
        addWindowToTiling(w);
        w->addToList(tiledWindows);
        lastActiveTiledWindow = w;
        sendWindowToBack(w);
    } else {
        w->windowMode = WindowMode::Floating;
        WmContainer* container = w->parent();
        if (container) {
            container->remove(w);
            WmContainer* parent = checkCleanContainer(container);
            if (parent)
                parent->realign();
        }
        w->removeFromList();
        w->addToList(floatingWindows);
        sendWindowToFront(w);
        if (lastActiveTiledWindow == w)
            lastActiveTiledWindow = 0;
    }
}

WmContainer* Workspace::checkCleanContainer(WmContainer *container) {
    if (container->size() > 0)
        return container;

    WmContainer* parent = container->parent();
    if (parent != 0) {
        parent->remove(container);
        for (auto it = begin(containers); it != end(containers); ++it) {
            if ((*it).get() == container) {
                it = containers.erase(it);
                break;
            }
        }

        return checkCleanContainer(parent);
    }

    // no parent -> we are root container
    rootContainer.reset();
    containers.clear(); // no containers should be available after deleting root container
    return 0; // no parent container
}

void Workspace::removeWindow(WmWindow* w) {
    if (!w) return;
    w->removeFromList();
    if (w->windowMode == WindowMode::Tiled) {
        if (lastActiveTiledWindow == w)
            lastActiveTiledWindow = 0;
        WmContainer* container = w->parent();
        if (container)
            container->remove(w);
        WmContainer* toAlign = checkCleanContainer(container);
        if (toAlign)
            toAlign->realign();
    }
    w->workspace = 0;
    if (w == lastActiveTiledWindow)
        lastActiveTiledWindow = 0;
}
