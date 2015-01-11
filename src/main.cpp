#include <iostream>
#include <stdexcept>
#include "windowmanager/windowmanager.h"


using namespace std;


int main() {
    bool normalExit = false;
    while (!normalExit) { // Recover from crashes
        try {
            WindowManager windowManager;
            windowManager.run();
            normalExit = true;
        } catch (exception &e) {
            cerr << "Aborted: " << e.what() << endl;
        }
    }
    return 0;
}