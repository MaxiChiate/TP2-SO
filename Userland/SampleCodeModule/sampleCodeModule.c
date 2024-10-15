#include <userlib.h>
#include <shell.h>
#include <processlib.h>
// #include <Testing/test_util.h>
// #include <Testing/tests.h>

#include <syscall.h>
#include <test_util.h>
#include <tests.h>
#include <stdint.h>

int64_t test_processes(/*uint64_t argc, char *argv[]*/);

#define MAX_PROCESSES 62 //64 - sampleCodeModule and test_processes


int main() {

    

    // char * testargv[] = {"test_process", "62", NULL};
    // run_process((int64_t)test_processes, 2, testargv, 1, true);

    char * testargv[] = {"test_prio", "62", NULL};
    run_process((int64_t) test_prio, 2, testargv, 1, true);


    waitp();

    // char buffer[254] = {'\0'};

    // do	{
    //     read(buffer);
    //     getMenu(buffer);
    // }	while(1);

	return 0;
}


enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int64_t test_processes(/*uint64_t argc, char *argv[]*/) {

  print("Starting test_processes\n");

  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes = MAX_PROCESSES;
  char *argvAux[] = {"endless_loop", NULL};

//   if (argc != 2)    {
//     print("test_processes: args amount\n");
//     return -1;
//   }

//   if ((max_processes = satoi(argv[1])) <= 0){
//     print("test_processes: max processes format\n");
//     return -1;
//   }

  p_rq p_rqs[max_processes];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {

        print("Spawn new process\n");
        p_rqs[rq].pid = (int) spawn_process((uint64_t) endless_loop, 1, argvAux, 1, true);

      if (p_rqs[rq].pid == -1) {
        print("test_processes: ERROR creating process\n");
        return -1;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              
              print("Killing process\n");
              
              if (!kill(p_rqs[rq].pid)) {
                
                print("test_processes: ERROR killing process\n");
                return -1;
              }
              p_rqs[rq].state = KILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
                print("Blocking process\n");
              if (!blockp(p_rqs[rq].pid)) {
                print("test_processes: ERROR blocking process\n");
                return -1;
              }
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
            print("Unblocking process\n");
          if (!unblockp(p_rqs[rq].pid)) {
            print("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}


#define WAIT 1000000000
  
             
#define TOTAL_PROCESSES 3
#define LOWEST 0  
#define MEDIUM 1  
#define HIGHEST 2 

uint64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio() {
  uint64_t pids[TOTAL_PROCESSES];
  char *argv[] = {"endless_loop_print", NULL};

  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = spawn_process((int64_t) endless_loop_print, 1, argv, 1, true);

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