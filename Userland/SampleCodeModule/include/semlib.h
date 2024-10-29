#ifndef __SEM_LIB_H
#define __SEM_LIB_H

    int8_t new_sem(uint8_t count);

    bool is_sem_alive(uint8_t sem_id);

    bool kill_sem(uint8_t sem_id);

    void up(uint8_t sem_id);
    void down(uint8_t sem_id);

#endif