#include <memory_manager.h>
#include <process/process_management.h>
#include <time.h>

//TODO: Releer todo y ver si compila. Habiendo configurado GDB.

#define BEGINNIN_PROCESS_ADDRESS(process_index) (uint64_t) stacks + (process_index+1)*STACK_SPACE -1
#define IN_RANGE(i) ((i) > 0 && (i) < PROCESS_AMOUNT)
#define VALID_FD(fd) ((fd) >= 0 || (fd) < MAX_FDS)

#define OVERFLOW ( (uint64_t) (-1))

// Complete with ticks as quantum, each position represents the priority status.

#define QUANTUM_AMOUNT 3
static unsigned int quantum[QUANTUM_AMOUNT] = {0b0001, 0b0010, 0b0100};

// Nuevo estado para la implementación de waitpid
typedef enum preocess_state {BLOCKED, READY, RUNNING, TERMINATED} process_state_t;

typedef struct pcb {
    
    uint64_t align;
    uint64_t stack_segment;
    uint64_t stack_pointer;
    uint64_t base_pointer;
    uint64_t register_flags;
    uint64_t code_segment;
    uint64_t instruction_pointer;

    char * argv[]; //rdi
    int argc;      //rsi
    
    pid_t process_id;
    pid_t parent_process_id;
    process_state_t state;

    unsigned int quantum;

} pcb_t;

static pcb_t pcbs [PROCESS_AMOUNT];
static uint64_t stacks [PROCESS_AMOUNT][STACK_SPACE];

/*-------------------------------------------------------------------------------------------------------------------*/

static unsigned int current_process = 0;
static pid_t process_id_counter = INITIAL_PROCESS_ID;
static unsigned int current_amount_process = 0;
static unsigned int started_at = 0;


static bool can_change_state(process_state_t old, process_state_t new);
static bool block_process_by_index(int p);
static bool unblock_process_by_index(int p);
static bool run_process_by_index(int p);

static void chronometer();

static bool is_blocked(int p);
static bool is_alive(int p);
static bool not_alive(int p);
static bool is_dead(int p);
static bool not_dead(int p);
static bool is_ready(int p);
static bool not_ready(int p);

static bool not_alive(int p);
static bool terminate(int p);

static void next_process();
    
static unsigned int get_quantum(unsigned int priority);

void refresh_pcb_from_stackcontext(unsigned int p);
void refresh_stackcontext_from_pcb(unsigned int p);

int static new_process(uint64_t function_address, int argc, char * argv[], unsigned int priority);

static int get_index_by_sp(uint64_t sp);
static int get_index_by_pid(uint64_t pid);

static bool kill_process(int p);

/*-------------------------------------------------------------------------------------------------------------------*/



void scheduler_init(uint64_t init_address, int argc, char * argv[])   {

    for (int i  = 0; i < PROCESS_AMOUNT; i++) {

        pcbs[i].state = TERMINATED;
    }

    new_process(init_address, argc, argv);
}



uint64_t schedule(uint64_t current_stack_pointer) {

    if(current_amount_process == 0)  return current_stack_pointer;

    refresh_pcb_from_stackcontext(current_process);

    if(alarmAtTicks(pcbs[current_process].quantum + started_at))   {
        
        next_process();
    }
    
    return pcbs[current_process].stack_pointer;
}



uint64_t create_process(uint64_t parent_pid, uint64_t function_address, int argc, char * argv[], unsigned int priority) {

    if(current_amount_process == PROCESS_AMOUNT) return OVERFLOW;
    
    int new_process_index = new_process(function_address, argc, argv);
    pcbs[new_process_index].parent_process_id = parent_pid;
    return pcbs[new_process_index].process_id;
}



bool kill_process_by_sp(uint64_t sp_to_delete)  {

    return kill_process(get_index_by_sp(sp_to_delete));
}



bool kill_process_by_pid(uint64_t pid)   {
    
    return kill_process(get_index_by_pid(pid));
}



pid_t get_current_pid()  {

    return pcbs[current_process].process_id;
}



void give_up_cpu()  {

    next_process();
}



bool block_process(uint64_t pid)    {
    
    return block_process_by_index(get_index_by_pid(pid));
}


bool unblock_process(uint64_t pid)    {
    
    return unblock_process_by_index(get_index_by_pid(pid));
}


bool change_process_priority(uint64_t pid, int prio)  {

    int index = get_index_by_pid(pid);

    if(prio > 0 && prio < QUANTUM_AMOUNT)     {

        pcbs[index].quantum = get_quantum(prio);
        return true;
    }

    return false;
}


void wait()     {

    int i = 0;

    while(i<PROCESS_AMOUNT) {

        if(is_alive(current_process) && 
            pcbs[current_process].parent_process_id == pcbs[current_process].process_id)  {

                i= (-1);
                give_up_cpu();
        }

        i++;
    }
}



// Espera a que el proceso de process id = pid tenga estado TERMINATED
void waitpid(unsigned int pid) {

    if (!IN_RANGE(pid)) return;
    
    int process_index = get_index_by_pid(pid);

    while(is_alive(process_index))  {

        give_up_cpu();
    }
}

void * process_status()   {
    ;
}


//Based on: Tanenbaum, Modern Operating Systems 4e, 2015 Prentice-Hall. Figure 2-2.

static bool can_change_state(process_state_t old, process_state_t new)  {

    switch (old)    {
        
        case RUNNING:       return new != RUNNING;
        
        case BLOCKED:       return new == READY;

        case READY:         return new == RUNNING;

        default:        return false;
    }   
}

static bool change_state_process(int p, process_state_t state)    {

    if( IN_RANGE(p) && can_change_state(pcbs[p].state, state)) {

        pcbs[p].state = state;
        return true;
    }

    return false;
}

static inline bool block_process_by_index(int p)  {

    return change_state_process(p, BLOCKED);
}

static inline bool unblock_process_by_index(int p)  {

    return change_state_process(p, READY);
}

static inline bool run_process_by_index(int p)  {

    return change_state_process(p, RUNNING);
}

static inline bool terminate_process_by_index(int p)    {

    return change_state_process(p, TERMINATED);
}

static inline bool is_blocked(int p)    {

    return pcbs[p].state == BLOCKED;
}

static inline void chronometer() {

    started_at = ticks_elapsed();
}

static inline bool is_alive(int p)  {

    return pcbs[(p)].state != TERMINATED;
}

static inline bool not_alive(int p)    {
    
    return !is_alive(p);
}

static inline bool is_ready(int p) {

    return pcbs[p].state == READY;
}

static inline bool not_ready(int p) {

    return !is_ready(p);
}

static void next_process()   {
    
    if(current_amount_process > 0)  {

        do  {
            
            current_process = (current_process + 1) % PROCESS_AMOUNT; 
        }   while(not_alive(current_process) || not_ready(current_process));

        run_process_by_index(current_process);
        chronometer();
    }
}

static unsigned int get_quantum(unsigned int priority)    {

    return quantum[priority % QUANTUM_AMOUNT];
}

static void refresh_pcb_from_stackcontext(unsigned int p)   {

// Address of last parameter in process context:
    int i= pcbs[p].stack_pointer + sizeof(stacks[0][0])*STATE_PUSHED_SIZE + 1;
    
        pcbs[p].align = stacks[p][i--];     
        pcbs[p].stack_segment = stacks[p][i--];
        pcbs[p].stack_pointer = stacks[p][i--]; 
        pcbs[p].register_flags = stacks[p][i--];
        pcbs[p].code_segment = stacks[p][i--];
        pcbs[p].instruction_pointer = stacks[p][i--];
        pcbs[p].process_id = stacks[p][i--];
        pcbs[p].state = stacks[p][i--];

        (uint64_t) pcbs[p].argc = tacks[p][i - 5];
        (uint64_t) pcbs[p].argv = tacks[p][i - 6];        
}

static void refresh_stackcontext_from_pcb(unsigned int p)   {

// Address of last parameter in process context:
    int i= pcbs[p].stack_pointer + sizeof(stacks[0][0])*STATE_PUSHED_SIZE + 1;
    
        stacks[p][i--]= pcbs[p].align;
        stacks[p][i--]= pcbs[p].stack_segment;
        stacks[p][i--]= pcbs[p].stack_pointer;
        stacks[p][i--]= pcbs[p].register_flags;
        stacks[p][i--]= pcbs[p].code_segment;
        stacks[p][i--]= pcbs[p].instruction_pointer;
        stacks[p][i--]= pcbs[p].process_id;
        stacks[p][i--]= pcbs[p].state;

        stacks[p][i - 5] = (uint64_t) pcbs[p].argc;
        stacks[p][i - 6] = (uint64_t) pcbs[p].argv;
}

static int new_process(uint64_t function_address, int argc, char * argv[], unsigned int priority)  {

    current_amount_process++;

    int new_process_index = 0;
    while(is_alive(new_process_index++));

    pcb_t * new_pcb;

        new_pcb.align = INITIAL_ALIGN;
        new_pcb.stack_segment = GLOBAL_SS;
        new_pcb.stack_pointer =  BEGINNIN_PROCESS_ADDRESS(new_process_index);
        new_pcb.register_flags = GLOBAL_RFLAGS;
        new_pcb.code_segment = GLOBAL_CS;
        new_pcb.instruction_pointer = function_address;

        new_pcb.argc = argc;
        new_pcb.argv = argv

        new_pcb.process_id = process_id_counter++;
        new_pcb.parent_process_id = DEFAULT_PARENT_PID;
        new_pcb.state = READY;

        new_pcb.quantum = get_quantum(priority);

    pcbs[new_process_index] = new_pcb;

    refresh_stackcontext_from_pcb(new_process_index);

// sp prepared to do popState:
    new_pcb.stack_pointer -= (STATE_PUSHED_SIZE + CONTEXT_PUSHED_SIZE) * sizeof(stacks[0][0]);

    return new_process_index;
}

static int get_index_by_sp(uint64_t sp) {

    for(int i=0; i<PROCESS_AMOUNT; i++) {

        if(not_alive(i) && pcbs[i].stack_pointer == sp) {
    
            return i;
        }
    }
    return -1;
}

static int get_index_by_pid(uint64_t pid)   {

    for(int i=0; i<PROCESS_AMOUNT; i++) {

        if(not_alive(i) && pcbs[i].process_id == pid) {
    
            return i;
        }
    }
    return -1;
}

static bool kill_process(int p) {

    return IN_RANGE(p) ? terminate_process_by_index(p) : false;
}
