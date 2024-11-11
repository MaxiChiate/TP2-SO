#include <semaphore.h>
#include <process/process_management.h>

struct sem  {
    
    bool alive;
    uint8_t count;
    bool locked;
    queue_t processes_queue;

} typedef sem_t;

static uint8_t current_sem;
static uint8_t using_sem;

static sem_t semaphores[MAX_SEMAPHORES_AMOUNT];


static void init_stdin_sync();
static inline void next_id();
static inline bool in_range(uint8_t id);
static void wait_sem_access(uint8_t sem_id);
static bool spawn_sem(uint8_t id, uint8_t count);



void init_semaphore_store()  {

    for(int i=0; i<MAX_SEMAPHORES_AMOUNT; i++)  {

        semaphores[i].alive = false;
    }

    current_sem = 0;
    using_sem = 0;

    init_stdin_sync();
}



int8_t new_sem(uint8_t count)   {

    if(using_sem == MAX_SEMAPHORES_AMOUNT) {

        return -1;
    }

    next_id();

    if(spawn_sem(current_sem, count))   {

        using_sem++;
        return current_sem;
    }
    return -1;
}



bool is_sem_alive(uint8_t sem_id)   {

    return in_range(sem_id) && semaphores[sem_id].alive;
}



bool kill_sem(uint8_t sem_id)   {

    if(is_sem_alive(sem_id))    {

        semaphores[sem_id].alive = false;
        free_queue(semaphores[sem_id].processes_queue);
        using_sem--;
        return true;
    }

    return false;
}



void down(uint8_t sem_id)   {

    if(!is_sem_alive(sem_id))   {

        return;
    }

    int64_t pid;

// Accesso exclusivo al semaphore store:

    wait_sem_access(sem_id);

    while(semaphores[sem_id].count == 0)   {

        pid = get_current_pid();

        if(pid == -1)   {

            leave_region(&semaphores[sem_id].locked);
            return;
        }

        enqueue(semaphores[sem_id].processes_queue, pid);
        leave_region(&semaphores[sem_id].locked);

    // Pierde el cpu:
        block_process(pid);

        wait_sem_access(sem_id);
    }

    semaphores[sem_id].count--;
    leave_region(&semaphores[sem_id].locked);

}



void up(uint8_t sem_id) {

    if(!is_sem_alive(sem_id))   {

        return;
    }

// Accesso exclusivo al semaphore store:
    wait_sem_access(sem_id);

    semaphores[sem_id].count++;

    if(!queue_is_empty(semaphores[sem_id].processes_queue))  {

        unblock_process(dequeue(semaphores[sem_id].processes_queue));
    }

    leave_region(&semaphores[sem_id].locked);
}



static uint8_t stdin_semid;

void wait_stdin()   {

    down(stdin_semid);
}


void signal_stdin() {

    up(stdin_semid);
}


static void init_stdin_sync()    {

    stdin_semid = new_sem(0);
}

static inline void next_id() {

    if (using_sem == MAX_SEMAPHORES_AMOUNT) {

        return;
    }

    while(semaphores[current_sem].alive)   {

        current_sem = (current_sem + 1) % MAX_SEMAPHORES_AMOUNT;
    }
}

static inline bool in_range(uint8_t id)     {
    
    return id < MAX_SEMAPHORES_AMOUNT;
}

static void wait_sem_access(uint8_t sem_id)  {

    while(enter_region(&semaphores[sem_id].locked))   {

        give_up_cpu();
    }
}

static bool spawn_sem(uint8_t id, uint8_t count)  {

    if(!in_range(id))  {

        return false;
    }

    semaphores[id].alive = true;
    semaphores[id].count = count;
    semaphores[id].locked = false;
    semaphores[id].processes_queue = queue_init();
    return true;
}