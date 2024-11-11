#ifndef __APPS_H
#define __APPS_H

    #include <heaplib.h>
    #include <processlib.h>
    #include <userlib.h>

    #define ERROR_MESSAGE_FEW "ERROR: too few arguments\n"
    #define ERROR_MESSAGE_MANY "ERROR: too many arguments\n"

    void mem(int argc, char ** argv);
    void loop(int argc, char ** argv);
    void ps(int argc, char ** argv);
    void block(int argc, char ** argv);
    void kill(int argc, char ** argv);
    void nice(int argc, char ** argv);

#endif