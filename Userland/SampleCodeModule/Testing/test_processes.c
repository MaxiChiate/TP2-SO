#include <Testing/tests.h>

enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

void test_processes(int argc, char ** argv) {

  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  uint8_t show;
  char *argvAux[] = {0};

  if (argc != 2)  {

    print("test_processes: ERROR incorrect argument amount\nUsage: test_processes <max processes> <show process, 1 or 0>\n");
    suicide();
  }

  if ((max_processes = satoi(argv[0])) <= 0)  {
   
    print("test_processes: ERROR incorrect first argument value");
    suicide();
  }

  if ((show = satoi(argv[1])) > 1)  {
   
    print("test_processes: ERROR incorrect second argument value");
    suicide();
  }

  p_rq p_rqs[max_processes];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {

      if(show)  {

        print("Spawning process...");
      }
      p_rqs[rq].pid = spawn_process((int64_t) &endless_loop, 0, argvAux, 1, false);

      if (p_rqs[rq].pid == -1) {
        print("test_processes: ERROR creating process\n");
        suicide();
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
        if(show)
          print(" Done!\n");
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              
              if(show)
                print("Killing process...");

              if (kill(p_rqs[rq].pid) == -1) {
                print("test_processes: ERROR killing process\n");
                suicide();
              }
              if(show)
                print(" Done!\n");
              p_rqs[rq].state = KILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {

              if(show)
                print("Blocking process...");
              if (blockp(p_rqs[rq].pid) == -1) {
                print("test_processes: ERROR blocking process\n");
                suicide();
              }
              if(show)
                print(" Done!\n");
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {

          if(show)
            print("Unblocking process...");

          if (unblockp(p_rqs[rq].pid) == -1) {
            print("test_processes: ERROR unblocking process\n");
            suicide();
          }
          if(show)
            print(" Done!\n");
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}