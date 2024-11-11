#include <process/process_management.h>

#define BEGINNIN_PROCESS_ADDRESS(process_index) ((uint64_t) stacks + (process_index + 1) * STACK_SPACE)
#define IN_RANGE(i) ((i) >= 0 && (i) < PROCESS_AMOUNT)

// Complete with ticks as quantum, each position represents the priority status.

#define QUANTUM_AMOUNT 3
static unsigned int quantum[QUANTUM_AMOUNT] = {0b001, 0b010, 0b100};

typedef enum process_state {BLOCKED, READY, RUNNING, TERMINATED} process_state_t;

typedef struct pcb {
    
    uint64_t align;
    uint64_t stack_segment;
    uint64_t stack_pointer;
    uint64_t base_pointer;
    uint64_t register_flags;
    uint64_t code_segment;
    uint64_t instruction_pointer;
    
    queue_t waiting_me;

    int64_t process_id;
    int64_t parent_process_id;
    process_state_t state;
    bool foreground;

    unsigned int quantum;
    unsigned int priority;

    int argc;      //rsi
    char ** argv; //rdi
} pcb_t;

static pcb_t pcbs[PROCESS_AMOUNT];
static uint64_t stacks[PROCESS_AMOUNT][STACK_SPACE];

/*-------------------------------------------------------------------------------------------------------*/
static unsigned int current_process;
static int64_t process_id_counter;
static unsigned int current_amount_processes;
static unsigned int current_blocked_processes;
static unsigned int started_at;
static bool scheduler_on;
static bool initializing;
static bool force_next;
/*-------------------------------------------------------------------------------------------------------*/

static bool can_change_state(process_state_t old, process_state_t new);
static bool block_process_by_index(int p);
static bool unblock_process_by_index(int p);
static bool run_process_by_index(int p);

static void chronometer();

static bool is_blocked(int p);
static bool not_blocked(int p);
static bool is_alive(int p);
static bool not_alive(int p);
static bool is_ready(int p);
static bool not_ready(int p);
static bool is_running(int p);
static bool not_running(int p);

static bool not_alive(int p);

static void next_process();
static void find_next();

static unsigned int get_quantum(unsigned int priority);

static void update_pcb(unsigned int p, uint64_t new_sp);
static void load_stackcontext_from_pcb(unsigned int p);

int static new_process(uint64_t function_address, int argc, char * argv[], unsigned int priority, bool foreground);

static int get_index_by_sp(uint64_t sp);
static int get_index_by_pid(int64_t pid);

static bool kill_process(int p);

static bool wait4(int64_t pid);
static void wake_up_processes_waiting_me();

static ps_t process_status(int p);

/*-------------------------------------------------------------------------------------------------------------------*/



void scheduler_init(uint64_t init_address, int argc, char * argv[])   {

    if(!scheduler_on)   {
            
        _cli();

        for (int i  = 0; i < PROCESS_AMOUNT; i++) {

            pcbs[i].state = TERMINATED;
        }  

        current_process = 0;
        process_id_counter = INITIAL_PROCESS_ID;
        current_amount_processes = 0;
        current_blocked_processes = 0;
        started_at = 0;

        current_process = new_process(init_address, argc, argv, QUANTUM_AMOUNT-1, true);

        init_hlt();

        scheduler_on =  true;
        initializing =  true;
        force_next =    true;

        _sti();
        _force_timertick_int();
    }
}

#include <stringPrinter.h>
uint64_t schedule(uint64_t current_stack_pointer) {

    if(!scheduler_on)   return current_stack_pointer;

    if(current_amount_processes == 0) return -1;

    initializing ? (initializing = false) : update_pcb(current_process, current_stack_pointer); 

// Si bien halt desactiva el timer tick, se contempla el caso en el caso en el que 
// se dispare una int antes de que esta se bloquee.
// No rompe la logica del halt ya que cuando se desbloquea o bloquea a este proceso
// se activa force_next.

    if( force_next || 
             (!is_halting() && alarmAtTicks(pcbs[current_process].quantum + started_at)))   {
        
        force_next = false;
        next_process();
    }
    
    return pcbs[current_process].stack_pointer;
}



int64_t create_process(uint64_t function_address, int argc, char * argv[], unsigned int priority, bool foreground) {

    if(current_amount_processes == PROCESS_AMOUNT) return -1;

    int new_process_index = new_process(function_address, argc, argv, priority, foreground);
    pcbs[new_process_index].parent_process_id = get_current_pid();
    return pcbs[new_process_index].process_id;
}



bool kill_process_by_sp(uint64_t sp_to_delete)  {

    return kill_process(get_index_by_sp(sp_to_delete));
}



bool kill_process_by_pid(int64_t pid)   {
    
    return kill_process(get_index_by_pid(pid));
}



int64_t get_current_pid()  {

    return current_amount_processes == 0 ? -1 : pcbs[current_process].process_id;
}



void give_up_cpu()  {

    force_next = true;
    _force_timertick_int();
}

void suicide() {

    kill_process(current_process);

    wake_up_processes_waiting_me();

// Se liberan los argumentos copiados:

    for(int i=0;i <pcbs[current_process].argc ; i++)    {

        mm_free(pcbs[current_process].argv[i]);
    }

    mm_free(pcbs[current_process].argv);

    give_up_cpu();
}



bool block_process(int64_t pid)    {
    
    bool done = block_process_by_index(get_index_by_pid(pid));

    if(done)    {

        if(current_amount_processes == current_blocked_processes)    {
        // Todos los procesos bloqueados, recurro a halt:
            unblock_hlt();
        }

        if(pid == get_current_pid()) {

            give_up_cpu();
        }
    }

    return done;
}


bool unblock_process(int64_t pid)    {
    
    bool done = unblock_process_by_index(get_index_by_pid(pid));
    
    if(is_halting() && done)  {
    // Se desbloqueó un proceso, bloqueo halt y fuerzo ir al siguiente proceso:
        block_hlt();
        force_next = true;
    }

    return done;
}


void force_block(int64_t pid)    {

    block_process_by_index(get_index_by_pid(pid));
}



void force_unblock(int64_t pid)    {

    unblock_process_by_index(get_index_by_pid(pid));
}


bool is_blocked_by_pid(int64_t pid)   {

    return is_blocked(get_index_by_pid(pid));
}


bool change_process_priority(int64_t pid, int prio)  {

    int index = get_index_by_pid(pid);

    if(prio >= 0 && prio < QUANTUM_AMOUNT)     {

        pcbs[index].quantum = get_quantum(prio);
        return true;
    }

    return false;
}


void wait()     {

    bool waiting;
    int i = 0;

    while(i<PROCESS_AMOUNT) {

        if(is_alive(i) && 
            pcbs[i].parent_process_id == pcbs[current_process].process_id)  {
                                            //TODO
                waiting = wait4(pcbs[i].process_id); // Solo se borra de un proceso cuando lo libera, TODO implementar una funcion cp en node y que no acepte repetidos y listo
        }

        i++;
    }

    if(waiting)    {

        block_process_by_index(current_process);
        give_up_cpu();
    }
}



void waitpid(int64_t pid) {

    if(wait4(pid))    {

        block_process_by_index(current_process);
        give_up_cpu();
    }
}

bool get_scheduler_status() {
    
    return scheduler_on;
}

ps_t ** get_ps(ps_t ** to_assign) {

    ps_t ** to_return = mm_malloc(sizeof(to_return[0]) * current_amount_processes);
    
    int k=0;
    for(int i=0; k<current_amount_processes && i<PROCESS_AMOUNT; i++)   {
        
        if (is_alive(i)) {
            
            *to_return[k++] = process_status(i);
        }
    }

    to_return[k] = NULL;

    to_assign = to_return;

    return to_return;
}



static ps_t process_status(int p)   {

    ps_t to_return = {
            .bp = pcbs[p].base_pointer,
            .sp = pcbs[p].stack_pointer,
            .id = pcbs[p].process_id,
            .parent_id = pcbs[p].parent_process_id,
            .state = pcbs[p].state == RUNNING ? 'R' : (pcbs[p].state == READY ? 'r' : (pcbs[p].state == BLOCKED ? 'b' : 't')),
            .foreground = pcbs[p].foreground,
            .priority = pcbs[p].priority
    };

    int i;

    for(i=0; i<PROCESS_NAME_LENGTH&& pcbs[p].argv[0][i]!=0; i++)    {

        to_return.name[i] = pcbs[p].argv[0][i];
    }

    to_return.name[i] = '\0';

    return to_return;
}




static bool can_change_state(process_state_t old, process_state_t new)  {

    switch (old)    {
        
        case RUNNING:       return new != RUNNING ;
        
        case BLOCKED:       return (new == READY) || (new == TERMINATED);

        case READY:         return new != READY;

        default:        return false;
    }   
}

static bool change_state_process(int p, process_state_t state)    {

    if( IN_RANGE(p) && can_change_state(pcbs[p].state, state)) {
    
        if(state == BLOCKED )    {

            if(pcbs[p].state != BLOCKED)    {

                current_blocked_processes++;
            }
        }
        else if (pcbs[p].state == BLOCKED)    {

            current_blocked_processes--;
        }

        pcbs[p].state = state;
        
        return true;
    }

    return false;
}

static bool block_process_by_index(int p)  {

    return change_state_process(p, BLOCKED);
}

static bool unblock_process_by_index(int p)  {

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

static inline bool not_blocked(int p)   {

    return !is_blocked(p);
}

static inline bool is_running(int p)    {

    return pcbs[p].state == RUNNING;
}

static inline bool not_running(int p)   {

    return !is_running(p);
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
    
    if(current_amount_processes > 0)  {

        if(is_running(current_process)) unblock_process_by_index(current_process); // set ready

//  Si hay otro proceso ready o solo hay uno pero no soy yo, busco al siguiente:
        if ((current_blocked_processes < current_amount_processes - 1) 
            || not_ready(current_process)) {   

            find_next();
        }

        run_process_by_index(current_process);
        chronometer();
    }
}

static unsigned int get_quantum(unsigned int priority)    {

    return quantum[priority % QUANTUM_AMOUNT];
}

static void update_pcb(unsigned int p, uint64_t new_sp)   {

        pcbs[p].stack_pointer = new_sp;   
}

static void load_stackcontext_from_pcb(unsigned int p)   {

        int i=STACK_SPACE;
        
        stacks[p][--i] = pcbs[p].align;
        stacks[p][--i] = pcbs[p].stack_segment;
        stacks[p][--i] = pcbs[p].stack_pointer;
        stacks[p][--i] = pcbs[p].register_flags;
        stacks[p][--i] = pcbs[p].code_segment;
        stacks[p][--i] = pcbs[p].instruction_pointer;

        stacks[p][i - 5] = pcbs[p].base_pointer;

        stacks[p][i - 6] = (uint64_t) pcbs[p].argc;
        stacks[p][i - 7] = (uint64_t) pcbs[p].argv;
}


static int new_process(uint64_t function_address, int argc, char ** argv, unsigned int priority, bool foreground)  {

    current_amount_processes++;

    int new_process_index = 0;
    while(is_alive(new_process_index))  {
        
        new_process_index++;
    }

    pcb_t new_pcb = {

        .align = INITIAL_ALIGN,
        .stack_segment = GLOBAL_SS,
        .stack_pointer =  (uint64_t) &stacks[new_process_index][STACK_SPACE],
        .base_pointer  =  (uint64_t) &stacks[new_process_index][STACK_SPACE],
        .register_flags = GLOBAL_RFLAGS,
        .code_segment = GLOBAL_CS,
        .instruction_pointer = function_address,

        .argc = argc,
        .argv = (char **) mm_malloc(sizeof(argv[0])*argc),

        .process_id = process_id_counter++,
        .parent_process_id = DEFAULT_PARENT_PID,
        .state = TERMINATED, // Not ready yet
        .foreground = foreground,
        
        .waiting_me = queue_init(),

        .quantum = get_quantum(priority),
        .priority = priority
    };

    if(new_pcb.argv)    {
    
    //Se copia los argumentos:
        for(int i=0; i<argc; i++)   {

            new_pcb.argv[i] = strgetcpy(argv[i]);
        }
    }

    pcbs[new_process_index] = new_pcb;

    load_stackcontext_from_pcb(new_process_index);

    pcbs[new_process_index].stack_pointer = (uint64_t) &stacks[new_process_index][STACK_SPACE-STATE_PUSHED_SIZE-CONTEXT_PUSHED_SIZE];
    pcbs[new_process_index].state = READY;

    return new_process_index;
}

static int get_index_by_sp(uint64_t sp) {

    for(int i=0; i<PROCESS_AMOUNT; i++) {

        if(is_alive(i) && pcbs[i].stack_pointer == sp) {
    
            return i;
        }
    }
    return -1;
}

static int get_index_by_pid(int64_t pid)   {

    for(int i=0; i<PROCESS_AMOUNT; i++) {

        if(is_alive(i) && pcbs[i].process_id == pid) {
    
            return i;
        }
    }
    return -1;
}

static bool kill_process(int p) {
    
    if(IN_RANGE(p) && terminate_process_by_index(p))    {
        
        current_amount_processes--;
        return true;
    }

    return false;
}

static bool wait4(int64_t pid)  {

    int process_index = get_index_by_pid(pid);

    if(process_index < INITIAL_PROCESS_ID || not_alive(process_index))    {

        return false;
    }

    enqueue(pcbs[process_index].waiting_me, current_process);

    return true;
}

static void wake_up_processes_waiting_me()    {

    queue_t waiting = pcbs[current_process].waiting_me;

    while(!queue_is_empty(waiting))    {

        unblock_process_by_index(dequeue(waiting));
    }

    free_queue(waiting);
}

static void find_next()    {

    do  {
                
        current_process = (current_process + 1) % PROCESS_AMOUNT; 

    }   while(not_alive(current_process) || not_ready(current_process));
}