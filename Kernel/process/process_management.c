#include <scheduler.h>

//TODO: Releer todo y ver si compila. Habiendo configurado GDB.

#define K 1024
#define STACK_SPACE 4*K
#define PROCESS_AMOUNT 64
#define NEW_STACK_ADRESS (stacks + PROCESS_AMOUNT*STACK_SPACE - 1 - current_amount_process*STACK_SPACE)
#define BEGINNIN_CURRENT_PROCESS_ADRESS (stacks + PROCESS_AMOUNT*STACK_SPACE - current_process*STACK_SPACE)
#define STATE_PUSHED_SIZE 15 //Register pushed amount in pushSate macro

typedef enum preocess_state {BLOCKED, READY, RUNNING} process_state_t;

extern loadContext(uint64_tstack_segment, uint64_tstack_pointer, );

// TODO: cambiar nombres de campos (rflag -> register_flags, sp->stack_pointer, .)
typedef struct pcb {
    
    uint64_t stack_segment;
    uint64_t stack_pointer;
    uint64_t register_flags;
    uint64_t code_segment;
    uint64_t instruction_pointer;
    uint64_t process_id;
    process_state_t state;

} pcb_t;

static pcb_t pcbs [PROCESS_AMOUNT];
static uint64_t stacks [PROCESS_AMOUNT][STACK_SPACE];

static unsigned int current_process = PROCESS_AMOUNT-1; // Stacks grow backward
static unsigned int current_amount_process = 0;
static unsigned int process_id_counter = 1;


static void next_process()   {
    
    current_process == 0 ?  current_amount_process=PROCESS_AMOUNT-1 : --current_process;
}

static void refresh_pcb(unsigned int current_process)   {

// Address of last param in process context:
    int i= pcbs[current_process].stack_pointer + sizeof(stacks[0][0])*STATE_SAVED + 1;
    
        pcbs[current_process].stack_segment = stacks[current_process][i++],
        pcbs[current_process].stack_pointer = stacks[current_process][i++],
        pcbs[current_process].register_flags = stacks[current_process][i++],
        pcbs[current_process].code_segment = stacks[current_process][i++],
        pcbs[current_process].instruction_pointer = stacks[current_process][i++],
        pcbs[current_process].process_id = stacks[current_process][i++],
        pcbs[current_process].state = stacks[current_process][i++]    

}

uint64_t schedule(uint64_t current_stack_pointer) {

    refresh_pcb();

    da  {
        
        next_process();
    }   while(pcbs[current_process].state != READY);
    
    return pcbs[current_process].stack_pointer;
}

//TODO:
void new_process()  {

    if(current_amount_process == PROCESS_AMOUNT) return;

    current_amount_process++;

    pcb_t new_pcb = {
        
       stack_segment,
       stack_pointer = (uint64_t) NEW_STACK_ADRESS,
       register_flags,
       code_segment,
       instruction_pointer,
        process_id = process_id_counter++,
        state = READY
    };
}

uint64_t schedule(uint64_t sp) {
    next_context();
    return pcbs[current_process].rsp;
}
