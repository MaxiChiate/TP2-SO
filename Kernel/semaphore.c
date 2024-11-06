#include <semaphore.h>
#include <process/process_management.h>

struct sem  {
    
    bool alive;
    uint8_t count;
    bool locked;
    queue_t processes_queue;

} typedef sem_t;

static uint8_t current_sem = 0;
static uint8_t using_sem = 0;

static sem_t semaphores[MAX_SEMAPHORES_AMOUNT];

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


void init_semaphore_store()  {

    for(int i=0; i<MAX_SEMAPHORES_AMOUNT; i++)  {

        semaphores[i].alive = false;
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

static void wait_sem_access(uint8_t sem_id)  {

    while(enter_region(&semaphores[sem_id].locked))   {

        give_up_cpu();
    }
}

void down(uint8_t sem_id)   {

    if(!is_sem_alive(sem_id))   {

        return;
    }

// Accesso exclusivo al semaphore store:

    wait_sem_access(sem_id);

    if(semaphores[sem_id].count == 0)   {

        int64_t pid = get_current_pid();

        if(pid == -1)   {

            leave_region(&semaphores[sem_id].locked);
            return;
        }

        enqueue(semaphores[sem_id].processes_queue, &pid);
        leave_region(&semaphores[sem_id].locked);
        block_process(pid);
    }
    else    {

        semaphores[sem_id].count--;
        leave_region(&semaphores[sem_id].locked);
    }
}


void up(uint8_t sem_id) {

    if(!is_sem_alive(sem_id))   {

        return;
    }

// Accesso exclusivo al semaphore store:

    wait_sem_access(sem_id);

    semaphores[sem_id].count++;

    if(!queue_is_empty(semaphores[sem_id].processes_queue))  {

        int64_t * next_pid = (int64_t *) dequeue(semaphores[sem_id].processes_queue);

        if(next_pid != NULL)    {

            unblock_process(next_pid[0]);
        }
    }

    leave_region(&semaphores[sem_id].locked);
}

