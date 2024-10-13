
#include <Testing/syscall.h>
#include <Testing/test_util.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes being run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

uint64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio() {
  uint64_t pids[TOTAL_PROCESSES];
  char *argv[] = {"endless_loop_print", NULL};

  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = spawn_process(current_pid(), (uint64_t) &endless_loop_print, 1, argv, 1, true);

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
}