#include <process_management.h>
#include <time.h>

//TODO: Releer todo y ver si compila. Habiendo configurado GDB.

#define BEGINNIN_PROCESS_ADDRESS(process_index) (uint64_t) stacks + (process_index+1)*STACK_SPACE -1;

#define OVERFLOW ( (uint64_t) (-1))

typedef enum preocess_state {BLOCKED, READY, RUNNING} process_state_t;

typedef struct pcb {
    
    uint64_t align;
    uint64_t stack_segment;
    uint64_t stack_pointer;
    uint64_t register_flags;
    uint64_t code_segment;
    uint64_t instruction_pointer;
    
    char * argv[]; //rdi
    int argc;      //rsi
    
    uint64_t process_id;
    uint64_t parent_process_id;
    process_state_t state;

    unsigned int quantum;

} pcb_t;

static pcb_t * pcbs [PROCESS_AMOUNT];
static uint64_t stacks [PROCESS_AMOUNT][STACK_SPACE];

static unsigned int current_process = 0;
static unsigned int current_amount_process = 0;
static unsigned int process_id_counter = INITIAL_PROCESS_ID;
static unsigned int started_at = 0;


static void chronometer() {

    started_at = ticks_elapsed();
}


static void next_process()   {
    
    if(current_amount_process > 0)  {

        do  {
            
            current_process = (current_process+1) % PROCESS_AMOUNT;
        }   while(pcbs[current_process] == NULL || pcbs[current_process]->state != READY);

        chronometer();
    }
    
}


static unsigned int get_quantum(unsigned int priority)    {

    return quantum[priority % QUANTUM_AMOUNT];
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

        (uint64_t) pcbs[p]->argc = tacks[p][i - 5];
        (uint64_t) pcbs[p]->argv = tacks[p][i - 6];        
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

        stacks[p][i - 5] = (uint64_t) pcbs[p]->argc;
        stacks[p][i - 6] = (uint64_t) pcbs[p]->argv;
}


uint64_t schedule(uint64_t current_stack_pointer) {

    refresh_pcb_from_stackcontext(current_process);

    if(alarmAtTicks(pcbs[current_process]->quantum + started_at))   {
        
        next_process();
    }
    
    return pcbs[current_process]->stack_pointer;
}


int static new_process(uint64_t function_address, int argc, char * argv[], unsigned int priority)  {

    if(current_amount_process == PROCESS_AMOUNT) return OVERFLOW;

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

        new_pcb->argc = argc;
        new_pcb->argv = argv

        new_pcb->process_id = process_id_counter++;
        new_pcb->parent_process_id = DEFAULT_PARENT_PID;
        new_pcb->state = READY;

        new_pcb->quantum = get_quantum(priority);

    pcbs[new_process_index] = new_pcb;

    refresh_stackcontext_from_pcb(new_process_index);

// sp prepared to do popState:
    new_pcb->stack_pointer -= (STATE_PUSHED_SIZE + CONTEXT_PUSHED_SIZE) * sizeof(stacks[0][0]);

    return new_process_index;
}


uint64_t create_process(uint64_t parent_pid, uint64_t function_address, int argc, char * argv[], unsigned int priority) {
    
    int new_process_index = new_process(function_address, argc, argv);
    pcbs[new_process_index]->parent_process_id = parent_pid;
    return pcbs[new_process_index]->process_id;
}


bool kill_process(uint64_t sp_to_delete)  {

    if(sp_to_delete != OVERFLOW)    {
        
        for(int i=0; i<PROCESS_AMOUNT; i++) {

            if(pcbs[i]!=NULL && pcbs[i]->stack_pointer == sp_to_delete)  {

                pcbs[i] = NULL;
                return true;
            }
        }
    }
    return false;
}


static uint64_t get_sp_by_pid(uint64_t pid) {

    for(int i=0; i<PROCESS_AMOUNT; i++) {

        if(pcbs[i]!=NULL && pcbs[i]->process_id == pid) {
            return pcbs[i]->stack_pointer;
        }
    }
    return OVERFLOW;
}


bool kill_process_by_pid(uint64_t pid)   {
    
    return kill_process(get_sp_by_pid(pid));
}


void spawn_init_process(void)    {

    int argc = 2;    
    char * argv[argc] = {"_hlt", NULL};
    new_process(&_hlt(void), argc, argv);
}


uint64_t get_current_pid()  {

    return pcbs[current_process]->process_id;
}


void give_up_cpu()  {

    next_process();
}