#ifndef __SEM_H
#define __SEM_H

    #include <bool.h>
    #include <Collections/queue.h>
    // #include <process/process_management.h>

    #define MAX_PROCESS_AMOUNT 10
    #define MAX_SEMAPHORES_AMOUNT 100

    void init_semaphore_store();

    int8_t new_sem(uint8_t count);

    bool is_sem_alive(uint8_t sem_id);

    bool kill_sem(uint8_t sem_id);

// Semaforo "Global" para acceder al semaphore store.

    bool enter_region(bool * lock);
    void leave_region(bool * lock);

    void up(uint8_t sem_id);
    void down(uint8_t sem_id);

    void downNtimes(uint8_t sem_id, uint8_t n);
    void upNtimes(uint8_t sem_id, uint8_t n);

#endif 