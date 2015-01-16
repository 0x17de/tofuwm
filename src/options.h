#ifndef OPTIONS_H
#define OPTIONS_H


#include <unistd.h>


struct Options {
    bool restart = false;
    bool reload = false;
    bool restartHelper = false;
    pid_t oldProcessPid = 0;

    bool checkOptions(int argc, char* argv[]);
};


#endif // OPTIONS_H