#include "options.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>


using namespace std;


bool Options::checkOptions(int argc, char* argv[]) {
    try {
        vector<char *> args(argv+1, argv + argc);
        for (auto it = begin(args); it != end(args); ++it) {
            auto nextParameter = [&] (const char* errorMessage) {
                ++it; // Next parameter
                if (it == args.end())
                    throw out_of_range(errorMessage);
            };
            if (!strcmp(*it, "--restartHelper")) {
                restartHelper = true;
                nextParameter("--restartHelper requires the process id of the old process.");
                istringstream is(*it);
                is >> oldProcessPid;
            } else if (!strcmp(*it, "--restart")) {
                restart = true;
            }
        }
    } catch(exception& e) {
        cerr << "Error when parsing arguments: " << e.what() << endl;
        return false;
    }

    return true;
}