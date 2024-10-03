#include <scheduler.h>

//TODO: Releer todo y ver si compila. Habiendo configurado GDB.

#define K 1024
#define STACK_SPACE 4*K
#define PROCESS_AMOUNT 64
#define NEW_STACK_ADRESS (stacks + PROCESS_AMOUNT*STACK_SPACE - 1 - current_amount_process*STACK_SPACE)
#define BEGINNIN_CURRENT_PROCESS_ADRESS (stacks + PROCESS_AMOUNT*STACK_SPACE - current_process*STACK_SPACE)

typedef enum preocess_state {BLOCKED, READY, RUNNING} process_state_t;

extern loadContext(uint64_t ss, uint64_t rsp, );

// TODO: cambiar nombres de campos (rflag -> register_flags, sp->stack_pointer, .)
typedef struct pcb {
    
    uint64_t ss;
    uint64_t rsp;
    uint64_t rflags;
    uint64_t cs;
    uint64_t rip;
    uint64_t pid;
    process_state_t state;
/*
    uint64_t stack_segment;
    uint64_t stack_pointer;
    uint64_t register_flags;
    uint64_t code_segment;
    uint64_t instruction_pointer;
    uint64_t process_id;
    process_state_t state;
*/

} pcb_t;

static pcb_t pcbs [PROCESS_AMOUNT];
static uint64_t stacks [PROCESS_AMOUNT][STACK_SPACE];

static unsigned int current_process = 0;
static unsigned int current_amount_process = 0;
static unsigned int pid_counter = 1;


static void next_process()   {
    
    current_process = (++current_process) % PROCESS_AMOUNT;
}

void next_context() {

    next_process();
}

//TODO:
void new_process()  {

    if(current_amount_process == PROCESS_AMOUNT) return;

    current_amount_process++;

    pcb_t new_pcb = {
        
        ss,
        rsp = (uint64_t) NEW_STACK_ADRESS,
        rflags,
        cs,
        rip,
        pid = pid_counter++,
        state = READY
    };
}

uint64_t schedule(uint64_t sp) {
    next_context();
    return pcbs[current_process].rsp;
}
