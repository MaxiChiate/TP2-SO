#ifndef __APPS_H
#define __APPS_H

    #include <heaplib.h>
    #include <processlib.h>
    #include <userlib.h>

    void mem(int argc, char ** argv);
    void loop(int argc, char ** argv);
    void ps(int argc, char ** argv);
    void block(int argc, char ** argv);
    void kill(int argc, char ** argv);
    void nice(int argc, char ** argv);

#endif