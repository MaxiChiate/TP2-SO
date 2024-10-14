#include <stdint.h>
#include <stddef.h>
#include <userlib.h>
#include <processlib.h>
#include <bool.h>



uint64_t my_getpid();
uint64_t my_create_process(char *name, uint64_t argc, char *argv[]);
uint64_t my_nice(uint64_t pid, uint64_t newPrio);
uint64_t my_kill(uint64_t pid);
uint64_t my_block(uint64_t pid);
uint64_t my_unblock(uint64_t pid);
uint64_t my_sem_open(char *sem_id, uint64_t initialValue);
uint64_t my_sem_wait(char *sem_id);
uint64_t my_sem_post(char *sem_id);
uint64_t my_sem_close(char *sem_id);
uint64_t my_yield();
uint64_t my_wait(uint64_t pid);