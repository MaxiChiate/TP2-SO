//halt.c
#include <process/halt.h>

static bool halting;
static int64_t hlt_pid;
static bool eohlt;

static inline void exit_halt_state();
static inline void enter_halt_state();

void init_hlt()   {

    halting = false;
    eohlt = false;

    char * argv[] = {HALT_NAME, NULL};

    hlt_pid = create_process((int64_t) &run_hlt, 1, argv, 0, 0);
}

void unblock_hlt()   {

    force_unblock(hlt_pid);
}

void block_hlt() {

    force_block(hlt_pid);
    exit_halt_state();
}

void run_hlt(int argc, char ** argv)    {
    
    while(true)     {

        halting = true;

        enter_halt_state();

        _hlt();

        halting = false;

        _force_timertick_int();
    }
}

bool is_halting()   {

    return halting;
}

bool end_of_halt()  {

    bool to_ret = eohlt;
    eohlt = false;

    return to_ret;
}

bool is_halt_id(int64_t pid)   {

    return hlt_pid == pid;
}

static inline void exit_halt_state() {

    picMasterMask(0xFC);
}

static inline void enter_halt_state() {
// Al ser el unico proceso corriendo, para maximizar eficiencia y 
// facilitar el bloquear por teclado. Se desactiva el timer tick.
    picMasterMask(0xFD);
}