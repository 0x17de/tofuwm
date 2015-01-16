#include <iostream>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include "windowmanager/windowmanager.h"
#include "options.h"


using namespace std;


void prepareRestart() {
    pid_t thisPid = getpid();
    pid_t pid = fork();

    if (pid == 0) {
        stringstream ss;
        ss << thisPid;
        string thisPidString = ss.str();

        char* programPath = getenv("_");
        char* const paramList[] = {programPath, (char*)"--restartHelper", (char*)thisPidString.c_str(), 0};
        execv(programPath, paramList);
        exit(1);
    }
}

void runWindowManager(Options* const options) {
    bool normalExit = false;
    while (!normalExit) { // Recover from crashes
        try {
            WindowManager windowManager{options};
            WmStopAction stopAction = windowManager.run();
            switch (stopAction) {
                case WmStopAction::Reload:
                    options->reload = true;
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

void restartWhenOldProcessIsKilled(pid_t oldPid) {
    // Wait for old process to die
    while (kill(oldPid, 0) == 0) {
        sleep(1);
    }

    pid_t pid = fork();
    if (pid == 0) {
        stringstream ss;
        ss << oldPid;
        string thisPidString = ss.str();

        char* programPath = getenv("_");
        char* const paramList[] = {programPath, (char*)"--restart", (char*)thisPidString.c_str(), 0};
        execv(programPath, paramList);
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    Options options;
    if (!options.checkOptions(argc, argv))
        return 1;

    if (options.restartHelper) {
        restartWhenOldProcessIsKilled(options.oldProcessPid);
    } else {
        runWindowManager(&options);
    }

    return 0;
}