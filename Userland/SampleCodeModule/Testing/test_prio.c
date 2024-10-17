#include <test_util.h>
#include <syscall.h>

#define WAIT 1000000000
  
             
#define TOTAL_PROCESSES 3
#define LOWEST 0  
#define MEDIUM 1  
#define HIGHEST 2 

uint64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(int argc, char ** argv) {

  uint64_t pids[TOTAL_PROCESSES];
  char *argvaux[] = {"endless_loop_print", NULL};

  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = spawn_process((int64_t) endless_loop_print, 1, argvaux, 1, true);

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    blockp(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], MEDIUM);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblockp(pids[i]);

  bussy_wait(WAIT);
  print("KILLING...");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill(pids[i]);

  suicide();
}