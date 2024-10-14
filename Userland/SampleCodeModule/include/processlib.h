#ifndef _PROCESSLIB_H
#define _PROCESSLIB_H

    #include <bool.h>
    #include <stdint.h>
    #include <userlib.h>
    
    #define PROCESS_NAME_LENGTH 10

    typedef struct  {

        uint64_t sp;
        uint64_t bp;
        uint64_t id;
        bool foreground;
        int priority;
        char name[PROCESS_NAME_LENGTH+1];
    } ps_t;

    int64_t run_process(int64_t parent_pid, int64_t function_address, int argc, char * argv[], unsigned int priority, bool foreground);

    int64_t spawn_process(int64_t parent_pid, int64_t function_address, int argc, char * argv[], unsigned int priority, bool foreground);

    bool kill(uint64_t pid);

    uint64_t current_pid();

    void giveup_cpu();

    bool blockp(uint64_t pid);

    bool unblockp(uint64_t pid);

    bool nice(uint64_t pid, int prio);

    void waitp();

    void waitpid(unsigned int pid);

    ps_t * ps();

#endif // _PROCESSLIB_H