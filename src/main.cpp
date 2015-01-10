#include <iostream>
#include <stdexcept>
#include "windowmanager.h"


using namespace std;


int main() {
    try {
        WindowManager windowManager;
        windowManager.run();
    } catch(exception& e) {
        cerr << "Aborted: " << e.what() << endl;
    }
    return 0;
}