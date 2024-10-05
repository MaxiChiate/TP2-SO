#include <process_management.h>

//TODO: Releer todo y ver si compila. Habiendo configurado GDB.

#define K 1024
#define STACK_SPACE 4*K
#define PROCESS_AMOUNT 64
#define BEGINNIN_PROCESS_ADDRESS(process_index) (uint64_t) stacks + (process_index+1)*STACK_SPACE -1;
#define STATE_PUSHED_SIZE 15 //Register pushed amount in pushSate macro
#define CONTEXT_PUSHED_SIZE 6 

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
    char * argv[];
    int argc;
    
    uint64_t process_id;
    process_state_t state;

} pcb_t;

static pcb_t * pcbs [PROCESS_AMOUNT];
static uint64_t stacks [PROCESS_AMOUNT][STACK_SPACE];

static unsigned int current_process = 0;
static unsigned int current_amount_process = 0;
static unsigned int process_id_counter = 1;


static void next_process()   {
    
    current_process = (current_process+1) % PROCESS_AMOUNT;
}

static void refresh_pcb_from_stackcontext(unsigned int p)   {

// Address of last parameter in process context:
    int i= pcbs[p]->stack_pointer + sizeof(stacks[0][0])*STATE_PUSHED_SIZE + 1;
    
        pcbs[p]->align = stacks[p][i--];     
        pcbs[p]->stack_segment = stacks[p][i--];
        pcbs[p]->stack_pointer = stacks[p][i--]; 
        pcbs[p]->register_flags = stacks[p][i--];
        pcbs[p]->code_segment = stacks[p][i--];
        pcbs[p]->instruction_pointer = stacks[p][i--];
        pcbs[p]->process_id = stacks[p][i--];
        pcbs[p]->state = stacks[p][i--];

}

static void refresh_stackcontext_from_pcb(unsigned int p)   {

// Address of last parameter in process context:
    int i= pcbs[p]->stack_pointer + sizeof(stacks[0][0])*STATE_PUSHED_SIZE + 1;
    
        stacks[p][i--]= pcbs[p]->align;
        stacks[p][i--]= pcbs[p]->stack_segment;
        stacks[p][i--]= pcbs[p]->stack_pointer;
        stacks[p][i--]= pcbs[p]->register_flags;
        stacks[p][i--]= pcbs[p]->code_segment;
        stacks[p][i--]= pcbs[p]->instruction_pointer;
        stacks[p][i--]= pcbs[p]->process_id;
        stacks[p][i--]= pcbs[p]->state;

}

uint64_t schedule(uint64_t current_stack_pointer) {

    refresh_pcb_from_stackcontext(current_process);

    do  {
        
        next_process();
    }   while(pcbs[current_process]->state != READY);
    
    return pcbs[current_process]->stack_pointer;
}

bool new_process(uint64_t function_address, int argc, char * argv[])  {

    if(current_amount_process == PROCESS_AMOUNT) return false;

    current_amount_process++;

    int new_process_index = 0;
    while(pcbs[new_process_index++] != NULL); 

    pcb_t * new_pcb;

        new_pcb->align = INITIAL_ALIGN;
        new_pcb->stack_segment = GLOBAL_SS;
        new_pcb->stack_pointer =  BEGINNIN_PROCESS_ADDRESS(new_process_index);
        new_pcb->register_flags = GLOBAL_RFLAGS;
        new_pcb->code_segment = GLOBAL_CS;
        new_pcb->instruction_pointer = function_address;
        new_pcb->process_id = process_id_counter++;
        new_pcb->state = READY;

    pcbs[new_process_index] = new_pcb;

    refresh_stackcontext_from_pcb(new_process_index);

// sp prepared to do popstate:
    new_pcb->stack_pointer -= (STATE_PUSHED_SIZE + CONTEXT_PUSHED_SIZE) * sizeof(stacks[0][0]);

    return true;
}

bool kill_process(uint64_t sp_to_delete)  {

    for(int i=0; i<PROCESS_AMOUNT; i++) {

        if(pcbs[i]!=NULL && pcbs[i]->stack_pointer == sp_to_delete)  {

            pcbs[i] = NULL;
            return true;
        }
    }

    return false;
}
