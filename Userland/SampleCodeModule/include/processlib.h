#ifndef _PROCESSLIB_H
#define _PROCESSLIB_H

    #include <bool.h>
    #include <stdint.h>
    #include <userlib.h>
    #include <../../../Kernel/include/systemCallDefs.h>
    #include <defs.h>

    int64_t run_process(int64_t function_address, int argc, char ** argv, unsigned int priority, bool foreground);

    int64_t spawn_process(int64_t function_address, int argc, char ** argv, unsigned int priority, bool foreground);

    bool killp(int64_t pid);

    int64_t current_pid();

    void giveup_cpu();
    
    bool blockp(int64_t pid);

    bool unblockp(int64_t pid);

    bool nicep(int64_t pid, int prio);

    void waitp();

    void waitpid(int64_t pid);

    void suicide();

    // ps_t * ps();
    
#endif // _PROCESSLIB_H