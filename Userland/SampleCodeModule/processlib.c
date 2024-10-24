#include <processlib.h>

int64_t run_process(int64_t function_address, int argc, char ** argv, unsigned int priority, bool foreground)   {

    int64_t child_pid = spawn_process(function_address, argc, argv, priority, foreground);
    giveup_cpu();
    return child_pid;
}

int64_t spawn_process(int64_t function_address, int argc, char ** argv, unsigned int priority, bool foreground)   {

    return (int64_t) _int80(SYS_CREATE_PROCESS, (int64_t) argc, (int64_t) argv, (int64_t) priority, (int64_t) foreground, 0);
}

bool kill(int64_t pid)  {

    return (bool) _int80(SYS_KILL_PROCESS, pid, 0, 0, 0, 0);
}

int64_t current_pid()  {

    return _int80(SYS_GET_CURRENT_PID, 0, 0, 0, 0, 0);
}

void giveup_cpu()   {

    _int80(SYS_GIVE_UP_CPU, 0, 0, 0, 0, 0);
}

bool blockp(int64_t pid)    {

    return (bool) _int80(SYS_BLOCK_PROCESS, pid, 0, 0, 0, 0);
}

bool unblockp(int64_t pid)  {

    return (bool) _int80(SYS_UNBLOCK_PROCESS, pid, 0, 0, 0, 0);
}

bool nice(int64_t pid, int prio)    {

    return (bool) _int80(SYS_CHANGE_PROCESS_PRIORITY, pid, (int64_t) prio, 0, 0, 0);
}

void waitp()    {

    _int80(SYS_WAIT, 0, 0, 0, 0, 0);
}

void waitpid(int64_t pid)   {

    _int80(SYS_WAITPID, pid, 0, 0, 0, 0);
}

void suicide()  {

    _int80(SYS_SUICIDE, 0, 0, 0, 0, 0);
}

/* ps_t * ps()  {

    _int80(0xAA);
}
*/
