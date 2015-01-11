#include "workspace.h"


using namespace std;


Workspace::Workspace() {
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
    for(WmWindow* w : windows)
        w->hide();
}

void Workspace::show() {
    for(WmWindow* w : windows)
        w->show();
}

void Workspace::addWindow(WmWindow* w) {
    windows.push_back(w);
    w->show();
}

void Workspace::removeWindow(WmWindow* w) {
    windows.remove(w);
}
