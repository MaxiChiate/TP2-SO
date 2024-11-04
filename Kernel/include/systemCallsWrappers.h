//systemCallsWrappers.c

#ifndef __SYSTEM_CALLS_WRAPPERS_H
#define __SYSTEM_CALLS_WRAPPERS_H

    #include <bool.h>
    #include <stdint.h>
    #include <systemCallDefs.h>
    #include <systemCalls.h>
    #include <colours.h>
    #include <systemCalls.h>
    #include <videoDriver.h>
    #include <stringPrinter.h>
    #include <buffer.h>
    #include <rtcDriver.h>
    #include <time.h>
    #include <process_management.h>

    void call_syscall(int id, int64_t * args);

    typedef void (*syscall_wrapper)(int64_t * args);

    void sysWrite_wrapper(int64_t * args);
    void sysRead_wrapper(int64_t * args);
    void sysClear_wrapper(int64_t * args);
    void rand_wrapper(int64_t * args);
    void create_process_wrapper(int64_t * args);
    void kill_process_by_pid_wrapper(int64_t * args);
    void block_process_wrapper(int64_t * args);
    void unblock_process_wrapper(int64_t * args);
    void change_process_priority_wrapper(int64_t * args);
    void waitpid_wrapper(int64_t * args);
    void haltcpu_wrapper(int64_t * args);
    void get_current_pid_wrapper(int64_t * args);
    void give_up_cpu_wrapper(int64_t * args);
    void wait_wrapper(int64_t * args);
    void suicide_wrapper(int64_t * args);
    void timeManager_wrapper(int64_t * args);

#endif