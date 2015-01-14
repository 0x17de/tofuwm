#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <string>
#include <sstream>
#include "windowmanager/windowmanager.h"


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

struct Options {
    bool restart = false;
    bool restartHelper = false;
    pid_t oldProcessPid = 0;

    bool checkOptions(int argc, char* argv[]) {
        Options options;

        try {
            vector<char *> args(argv+1, argv + argc);
            for (auto it = begin(args); it != end(args); ++it) {
                auto nextParameter = [&] {
                    ++it; // Next parameter
                    if (it == args.end())
                        throw out_of_range("--restartHelper requires the process id of the old process.");
                };
                if (*it == "--restartHelper") {
                    restartHelper = true;
                    nextParameter();
                    istringstream is(*it);
                    is >> options.oldProcessPid;
                } else if (*it == "--restart") {
                    restart = true;
                }
            }
        } catch(exception& e) {
            cerr << "Error when parsing arguments: " << e.what() << endl;
            return false;
        }

        return true;
    }
};

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