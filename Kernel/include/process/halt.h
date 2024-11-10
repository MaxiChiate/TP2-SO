//halt.h
#ifndef __HALT_H
#define __HALT_H

    #include <process/process_management.h>
    #include <interrupts.h>
    #include <bool.h>

    #define HALT_NAME "halt"

    void init_hlt();
    void run_hlt(int argc, char ** argv);

    void unblock_hlt();
    void block_hlt();

    bool is_halting();

#endif