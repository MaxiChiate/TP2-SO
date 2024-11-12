#ifndef __process__management
#define __process__management

    #include <interrupts.h>
    #include <bool.h>
    #include <time.h>
    #include <memory_manager.h>
    #include <systemCalls.h>
    #include <systemCallDefs.h>
    #include <collections/queue.h>
    #include <lib.h>
    #include <process/halt.h>
    #include <process/ipc_management.h>
    
    #define K 1024
    #define STACK_SPACE 4*K
    #define PROCESS_AMOUNT 64

    #define STATE_PUSHED_SIZE 15 //Register pushed amount in pushSate macro
    #define CONTEXT_PUSHED_SIZE 6

    #define GLOBAL_SS       0x0
    #define GLOBAL_RFLAGS   0x202
    #define GLOBAL_CS       0x8

    #define INITIAL_ALIGN   0x0

    #define STACK_WIDTH 8

    #define DEFAULT_PARENT_PID 0
    #define INITIAL_PROCESS_ID 1    
    #define INITIAL_CURRENT_PROCESS_AMOUNT 2    // init (sh) y halt

    /*
    * Initializes scheduler
    */
    void scheduler_init(uint64_t init_address, int argc, char * argv[]);

    /*
    * Returns the value of the scheduler_on flag
    */
    bool get_scheduler_status();

    /*
    *  Decides which process will run next
    */
    uint64_t schedule(uint64_t current_stack_pointer);

    /*
    *  New process, with parent. @return: pid of new process.
    */
    int64_t create_process(uint64_t function_address, int argc, char * argv[], unsigned int priority, bool foreground);

    /*
    *  Kills a process according to its stack pointer.
    */
    bool kill_process_by_sp(uint64_t sp_to_delete);

    /*
    *  Kills a process according to its pid
    */
    bool kill_process_by_pid(int64_t pid);

    /*
    *  Return pid of current process.
    */
    int64_t get_current_pid();

    /*
    * Current process gives up the cpu.
    */
    void give_up_cpu();

    /*
    * Blocks a process
    */
    bool block_process(int64_t pid);

    /*
    * Unblocks a process
    */
    bool unblock_process(int64_t pid);

    /*
    *  Changess a process' priority
    */
    bool change_process_priority(int64_t pid, int prio);

    /*
    * Waits for a process to finish
    */
    void wait();

    /*
    * Waits for the process whose id is pid to finish
    */
    void waitpid(int64_t pid);

    ps_t ** get_ps(ps_t ** to_assign);

    void suicide();

    void force_block(int64_t pid);
    void force_unblock(int64_t pid);
    bool is_blocked_by_pid(int64_t pid);

    void kill_fg_process();

    void set_stdout_fd(int64_t pid, int new_fd);
    void set_stdin_fd(int64_t pid, int new_fd);

    void standard_write(char * buf, int size);
    void standard_read(char * buf, int size);

#endif //__process__management