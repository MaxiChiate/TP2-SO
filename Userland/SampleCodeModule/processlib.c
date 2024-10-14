#include <processlib.h>



int64_t run_process(int64_t function_address, int argc, char * argv[], unsigned int priority, bool foreground)   {

    int64_t child_pid = spawn_process(parent_pid, function_address, argc, argv, priority, foreground);
    giveup_cpu();
    return child_pid;
}


