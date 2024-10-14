#include <userlib.h>
#include <shell.h>
#include <processlib.h>
// #include <Testing/test_util.h>
// #include <Testing/tests.h>

#include <test_util.h>
#include <tests.h>
#include <stdint.h>

int64_t test_processes(/*uint64_t argc, char *argv[]*/);

#define MAX_PROCESSES 62 //64 - sampleCodeModule and test_processes


int main() {

    char * testargv[] = {"test_process", "62", NULL};
    waitpid(run_process((int64_t)test_processes, 2, testargv, 1, true));
    // run_process(current_pid(), &test_prio, 0, NULL, 1, true);


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
