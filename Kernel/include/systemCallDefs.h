//systemCallDefs.h

#ifndef SYSTEM_CALL_DEFS_H
#define SYSTEM_CALL_DEFS_H

    #include <bool.h>
    #include <stdint.h>

    typedef enum    {

            SYS_WRITE = 0,
            SYS_READ,
            SYS_CLEAR,
            SYS_RAND,
            SYS_CREATE_PROCESS,
            SYS_KILL_PROCESS,
            SYS_BLOCK_PROCESS,
            SYS_UNBLOCK_PROCESS,
            SYS_CHANGE_PROCESS_PRIORITY,
            SYS_WAITPID,
            SYS_HALTCPU,
            SYS_GET_CURRENT_PID,
            SYS_GIVE_UP_CPU,
            SYS_WAIT,
            SYS_SUICIDE,
            SYS_TIME,
            SYS_KILL_SEM,
            SYS_UP,
            SYS_DOWN,
            SYS_IS_SEM_ALIVE,
            SYS_NEW_SEM,
            SYS_MM_MALLOC,
            SYS_MM_FREE,

            SYSCALL_COUNT
        } syscall_id;

    
    #define PROCESS_NAME_LENGTH 10

    typedef struct  {

        uint64_t sp;
        uint64_t bp;
        uint64_t id;
        bool foreground;
        int priority;
        char name[PROCESS_NAME_LENGTH+1];
    } ps_t;


    // write @fd codes: (en rbx)
    #define STDOUT 						0x01
    #define CHARSTDOUT                  0x02
    #define STDERR 						0x03
    #define RETURN_CHAR                 0xA0	
    #define RETURNANDSTDOUT_CHAR        0xA1

    // read @fd codes: (en rbx)
    #define STRING  0x02
    #define CHAR    0x01

#endif