#ifndef _PROCESSLIB_H
#define _PROCESSLIB_H

    uint64_t spawn_process(uint64_t parent_pid, uint64_t function_address, int argc, char * argv[], unsigned int priority, bool foreground);

    bool kill_process(uint64_t pid);

    uint64_t current_pid();

    void giveup_cpu();

    bool blockp(uint64_t pid);

    bool unblockp(uint64_t pid);

    bool process_prio(uint64_t pid, int prio);

    void waitp();

    void waitpid(unsigned int pid);

    ps_t * ps();

#endif // _PROCESSLIB_H