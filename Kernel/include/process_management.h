#ifndef __process__management
#define __process__management

#include <interrupts.h>
#include <stdbool.h>

#define STATE_PUSHED_SIZE 15 //Register pushed amount in pushSate macro
#define CONTEXT_PUSHED_SIZE 6

#define GLOBAL_SS       0x0
#define GLOBAL_RFLAGS   0x202
#define GLOBAL_CS       0x8

#define INITIAL_ALIGN   0x0

#define DEFAULT_PARENT_PID 0
#define INITIAL_PROCESS_ID 1

#define OVERFLOW ( (uint64_t) (-1))


/*
 *  Decides which process will run next
 */
uint64_t schedule(uint64_t current_stack_pointer);



uint64_t fork(uint64_t parent_pid, uint64_t function_address, int argc, char * argv[])

/*
 *  Kills a process
 */
bool kill_process(uint64_t sp_to_delete);

/*
 *  Kills a process according to its pid
 */
bool kill_process_by_pid(uint64_t pid);


/*
 *  Creates a new init process, it will kill the old process with INITIAL_PROCESS_ID
 */
void spawn_init_process(void);


#endif //__process__management#endif //__process__management