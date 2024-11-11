#ifndef __SEM_LIB_H
#define __SEM_LIB_H

    #include <../../../Kernel/include/systemCallDefs.h>
    #include <stdint.h>
    #include <bool.h>
    #include <defs.h>
    #include <userlib.h>

    int8_t new_sem(uint8_t count);

    bool is_sem_alive(uint8_t sem_id);

    bool kill_sem(uint8_t sem_id);

    void up(uint8_t sem_id);
    void down(uint8_t sem_id);

    void wait_stdin();

#endif