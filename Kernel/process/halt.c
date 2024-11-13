//halt.c
#include <process/halt.h>

static bool halting;
static int64_t hlt_pid;
static bool eohlt;

// Al ser el unico proceso corriendo, para maximizar eficiencia y 
// facilitar el bloquear por teclado. Se desactiva el timer tick.
static inline void block_timertick(void) {

    atomicPicMasterMask(0xFD);
}
static inline void unblock_timertick(void) {
    
    atomicPicMasterMask(0xFC);
}


void init_hlt()   {

    halting = false;
    eohlt = false;

    char * argv[] = {HALT_NAME, NULL};

    hlt_pid = create_process((int64_t) &run_hlt, 1, argv, 2, 0);

    block_hlt();
}

void unblock_hlt()   {

    force_unblock(hlt_pid);
}

void block_hlt() {

    force_block(hlt_pid);
}

void run_hlt(int argc, char ** argv)    {
    
    while(true)     {

        block_timertick();

            _hlt();

        unblock_timertick();
        _force_timertick_int();
    }
}

bool is_halting()   {

    return !is_blocked_by_pid(hlt_pid);
}

bool end_of_halt()  {

    bool to_ret = eohlt;
    eohlt = false;

    return to_ret;
}

bool is_halt_id(int64_t pid)   {

    return hlt_pid == pid;
}

