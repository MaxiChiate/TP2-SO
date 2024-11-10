#include <semlib.h>

int8_t new_sem(uint8_t count)   {

    return (int8_t) _int80(SYS_NEW_SEM, (int64_t *) &count);
}

bool is_sem_alive(uint8_t sem_id)   {

    return (bool) _int80(SYS_IS_SEM_ALIVE, (int64_t *) &sem_id);
}

bool kill_sem(uint8_t sem_id)   {

    return (bool) _int80(SYS_KILL_SEM, (int64_t *) &sem_id);
}

void up(uint8_t sem_id)     {

    _int80(SYS_UP, (int64_t *) &sem_id);
}

void down(uint8_t sem_id)   {
    
    _int80(SYS_DOWN, (int64_t *) &sem_id);
}

void wait_stdin()   {

    _int80(SYS_WAIT_STDIN, NULL);
}