#include <scheduler.h>

//TODO: Releer todo y ver si compila. Habiendo configurado GDB.

#define K 1024
#define STACK_SPACE 4*K
#define PROCESS_AMOUNT 64
#define NEW_STACK_ADRESS (stacks + PROCESS_AMOUNT*STACK_SPACE - 1 - current_amount_process*STACK_SPACE)
#define BEGINNIN_CURRENT_PROCESS_ADRESS (stacks + PROCESS_AMOUNT*STACK_SPACE - current_process*STACK_SPACE)
#define STATE_PUSHED_SIZE 15 //Register pushed amount in pushSate macro

#define GLOBAL_SS       0x0
#define GLOBAL_RFLAGS   0x202
#define GLOBAL_CS       0x8
#define INITIAL_ALIGN   0x0

typedef enum preocess_state {BLOCKED, READY, RUNNING} process_state_t;

typedef struct pcb {
    
    uint64_t align;
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

static void refresh_pcb_from_stackcontext(unsigned int p)   {

// Address of last param in process context:
    int i= pcbs[p].stack_pointer + sizeof(stacks[0][0])*STATE_SAVED + 1;
    
        pcbs[p].align = stacks[p][i++];
        pcbs[p].stack_segment = stacks[p][i++];
        pcbs[p].stack_pointer = stacks[p][i++];
        pcbs[p].register_flags = stacks[p][i++];
        pcbs[p].code_segment = stacks[p][i++];
        pcbs[p].instruction_pointer = stacks[p][i++];
        pcbs[p].process_id = stacks[p][i++];
        pcbs[p].state = stacks[p][i++];

}

static void refresh_stackcontext_from_pcb(unsigned int p)   {

// Address of last param in process context:
    int i= pcbs[p].stack_pointer + sizeof(stacks[0][0])*STATE_SAVED + 1;
    
        stacks[p][i++]= pcbs[p].align;
        stacks[p][i++]= pcbs[p].stack_segment;
        stacks[p][i++]= pcbs[p].stack_pointer;
        stacks[p][i++]= pcbs[p].register_flags;
        stacks[p][i++]= pcbs[p].code_segment;
        stacks[p][i++]= pcbs[p].instruction_pointer;
        stacks[p][i++]= pcbs[p].process_id;
        stacks[p][i++]= pcbs[p].state;

}

uint64_t schedule(uint64_t current_stack_pointer) {

    refresh_pcb_from_stackcontext(current_process);

    da  {
        
        next_process();
    }   while(pcbs[current_process].state != READY);
    
    return pcbs[current_process].stack_pointer;
}

//Listo???
void new_process(uint64_t function_address)  {

    if(current_amount_process == PROCESS_AMOUNT) return;

    current_amount_process++;

    pcb_t new_pcb = {
        
        align = INITIAL_ALIGN,
        stack_segment = GLOBAL_SS,
        stack_pointer = (uint64_t) NEW_STACK_ADRESS,
        register_flags = GLOBAL_RFLAGS,
        code_segment = GLOBAL_CS,
        instruction_pointer = function_address,
        process_id = process_id_counter++,
        state = READY
    };

    int new_process_index = PROCESS_AMOUNT-current_amount_process;

    pcbs[new_process_index] = new_pcb;

    refresh_stackcontext_from_pcb(new_process_index);
}