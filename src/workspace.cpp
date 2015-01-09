#include "workspace.h"


using namespace std;


Workspace::Workspace() {
}

Workspace::~Workspace() {
}

void Workspace::hide() {
    for(shared_ptr<WmWindow> w : windows)
        w->hide();
}

void Workspace::show() {
    for(shared_ptr<WmWindow> w : windows)
        w->show();
}

void Workspace::addWindow(std::shared_ptr<WmWindow> w) {
    windows.push_back(w);
    w->show();
}
