//systemCallDefs.h

#ifndef SYSTEM_CALL_DEFS_H
#define SYSTEM_CALL_DEFS_H

    #include <bool.h>
    #include <stdint.h>
    #include <defs.h>

    typedef enum    {

            SYS_WRITE = 0,
            SYS_READ,
            SYS_OPEN,
            SYS_CLOSE,
            SYS_DUP,
            SYS_DUP2,
            SYS_DUP3,
            SYS_PIPE,
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
            SYS_PS,
            SYS_TIME,
            SYS_SLEEP,
            SYS_KILL_SEM,
            SYS_UP,
            SYS_DOWN,
            SYS_IS_SEM_ALIVE,
            SYS_NEW_SEM,
            SYS_WAIT_STDIN,
            SYS_MM_MALLOC,
            SYS_MM_FREE,
            SYS_MM_MEM,

            SYSCALL_COUNT
        } syscall_id;

    
    #define PROCESS_NAME_LENGTH 10

    typedef struct  {

        uint64_t sp;
        uint64_t bp;
        uint64_t id;
        uint64_t parent_id;
        char state;
        bool foreground;
        int priority;
        char * name;
    } ps_t;


    typedef enum {
    
        KEY_SHIFT = 'S',
        KEY_CTRL = 'C',
        KEY_CAPS_LOCK = 'M',
        KEY_UP_ARROW = 'U',
        KEY_LEFT_ARROW = 'L',
        KEY_RIGHT_ARROW = 'R',
        KEY_DOWN_ARROW = 'D',
        KEY_TAB = '\t',
        KEY_ENTER = '\n',
        KEY_BACKSPACE = '\b',
        CTRL_C = 3,      
        CTRL_D = EOF       

    } SpecialKey;

    typedef enum { STDIN_FILENO = 0, STDOUT_FILENO, STD_FD_COUNT} std_fd;
    typedef enum {
        
        NONE = 0b000, 
        R    = 0b001, 
        W    = 0b010, 
        RW   = 0b100
    } rw_flags_t;


#endif