#include <Testing/tests.h>

#define WAIT 1000000000
#define MINOR_WAIT "99000099"  


#define TOTAL_PROCESSES 3
#define LOWEST 0  
#define MEDIUM 1  
#define HIGHEST 2 

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(int argc, char ** argv) {
  argv++;
  if(argc != 1) {

      print("test_prio: ERROR argument amount\nUsage: test_prio\n");
      suicide();
  }

  int64_t pids[TOTAL_PROCESSES];
  char *argvAux[] = {MINOR_WAIT, NULL};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = spawn_process((int64_t) &endless_loop_print, 1, argvAux, MEDIUM, false);

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nicep(pids[i], prio[i]);

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    blockp(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nicep(pids[i], MEDIUM);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblockp(pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    killp(pids[i]);

  print(END_MESSAGE);
  suicide();
}