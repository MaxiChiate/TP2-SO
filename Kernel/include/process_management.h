#ifndef __process__management
#define __process__management

#include <interrupts.h>
#include <stdbool.h>

#define K 1024
#define STACK_SPACE 4*K
#define PROCESS_AMOUNT 64

#define STATE_PUSHED_SIZE 15 //Register pushed amount in pushSate macro
#define CONTEXT_PUSHED_SIZE 6

#define GLOBAL_SS       0x0
#define GLOBAL_RFLAGS   0x202
#define GLOBAL_CS       0x8

#define INITIAL_ALIGN   0x0

#define DEFAULT_PARENT_PID 0
#define INITIAL_PROCESS_ID 1

typedef uint64_t pid_t;


void scheduler_init()

/*
 *  Decides which process will run next
 */
uint64_t schedule(uint64_t current_stack_pointer);

/*
 *  New process, with parent. @return: pid of new process.
 */
uint64_t create_process(uint64_t parent_pid, uint64_t function_address, int argc, char * argv[])

/*
 *  Kills a process according to its stack pointer.
 */
bool kill_process_by_sp(uint64_t sp_to_delete);

/*
 *  Kills a process according to its pid
 */
bool kill_process_by_pid(uint64_t pid);

/*
 *  Return pid of current process.
 */
pid_t get_current_pid();

/*
 * Current process gives up the cpu.
 */
void give_up_cpu();

/*
 * Blocks a process
 */
bool block_process(uint64_t pid);

/*
 * Unblocks a process
 */
bool unblock_process(uint64_t pid);

/*
 *  Changess a process' priority
 */
bool change_process_priority(uint64_t pid, int prio);

/*
 * Waits for a process to finish
 */
void wait();

/*
 * Waits for the process whose id is pid to finish
 */
void waitpid(unsigned int pid);

#endif //__process__management