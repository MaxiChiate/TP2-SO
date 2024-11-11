#include <processlib.h>

int64_t run_process(int64_t function_address, int argc, char ** argv, unsigned int priority, bool foreground)   {

    int64_t child_pid = spawn_process(function_address, argc, argv, priority, foreground);
    giveup_cpu();
    return child_pid;
}

int64_t spawn_process(int64_t function_address, int argc, char ** argv, unsigned int priority, bool foreground)   {

    int64_t args[] = { function_address, (int64_t) argc, (int64_t) argv, (int64_t) priority, (int64_t) foreground };

    return (int64_t) _int80(SYS_CREATE_PROCESS, args);
}

bool killp(int64_t pid)  {

    return (bool) _int80(SYS_KILL_PROCESS, &pid);
}

int64_t current_pid()  {

    return _int80(SYS_GET_CURRENT_PID, NULL);
}

void giveup_cpu()   {

    _int80(SYS_GIVE_UP_CPU, NULL);
}

bool blockp(int64_t pid)    {

    return (bool) _int80(SYS_BLOCK_PROCESS, &pid);
}

bool unblockp(int64_t pid)  {

    return (bool) _int80(SYS_UNBLOCK_PROCESS, &pid);
}

bool nicep(int64_t pid, int prio)    {

    int64_t args [] = { pid, (int64_t) prio };

    return (bool) _int80(SYS_CHANGE_PROCESS_PRIORITY, args);
}

void waitp()    {

    _int80(SYS_WAIT, NULL);
}

void waitpid(int64_t pid)   {

    _int80(SYS_WAITPID, &pid);
}

void suicide()  {

    _int80(SYS_SUICIDE, NULL);
}

/* ps_t * ps()  {

    _int80(0xAA);
}
*/
