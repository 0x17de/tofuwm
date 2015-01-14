#include <iostream>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include "windowmanager/windowmanager.h"
#include "options.h"


using namespace std;


// getpid()
// kill(pid, 0)
// getenv("_")


void prepareRestart() {
    pid_t thisPid = getpid();
    pid_t pid = fork();

    stringstream ss;
    ss << pid;
    string thisPidString = ss.str();

    if (pid == 0) {
        char* programPath = getenv("_");
        char* const paramList[] = {programPath, (char*)"--restartHelper", (char*)thisPidString.c_str(), 0};
        execv(programPath, paramList);
        exit(1);
    }
}

void runWindowManager(const Options* const options) {
    bool normalExit = false;
    while (!normalExit) { // Recover from crashes
        try {
            WindowManager windowManager{options};
            WmStopAction stopAction = windowManager.run();
            switch (stopAction) {
                case WmStopAction::Reload:
                    continue;
                case WmStopAction::Restart:
                    prepareRestart();
                    // Fallthrough
                case WmStopAction::Stop:
                    normalExit = true;
                    break;
            }
        } catch (exception &e) {
            cerr << "Error. Trying to recover from: " << e.what() << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    Options options;
    if (!options.checkOptions(argc, argv))
        return 1;

    if (options.restartHelper) {
        // @TODO: Start helper
    } else {
        runWindowManager(&options);
    }

    return 0;
}