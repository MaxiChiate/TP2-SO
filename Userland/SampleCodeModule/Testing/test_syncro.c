#include <stdint.h>
#include <Testing/syscall.h>
#include <Testing/test_util.h>
#include <semlib.h>

#define SEM_ID 0

#define TOTAL_PAIR_PROCESSES 1

static int64_t global; // shared memory

static void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

#define N 5
#define INC 3
#define USE_SEM 1

static void my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n = N;
  int8_t inc = INC;
  int8_t use_sem = USE_SEM;

//   if (argc != 3)
//     return -1;

//   if ((n = satoi(argv[0])) <= 0)
//     return -1;
//   if ((inc = satoi(argv[1])) == 0)
//     return -1;
//   if ((use_sem = satoi(argv[2])) < 0)
//     return -1;

  if (use_sem)

    if(!is_sem_alive(SEM_ID)) {

      if(new_sem(1) == -1)  {

        print("Test_sync: Error creating semaphore\n");
        suicide();
      }        
    }

// Ahora debería encontrarlo:
    if(!is_sem_alive(SEM_ID)) {
      
      print("Test_sync: Error finding existing semaphore\n");
      suicide();
    }


  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      down(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      down(SEM_ID);
  }

  if (use_sem)
    if(kill_sem(SEM_ID))  {

      print("Test_sync: Error killing semaphore\n");
      suicide();
    }

  suicide();
}

static void my_process_dec(uint64_t argc, char *argv[]) {
  uint64_t n = N;
  int8_t inc = INC * (-1);
  int8_t use_sem = USE_SEM;

//   if (argc != 3)
//     return -1;

//   if ((n = satoi(argv[0])) <= 0)
//     return -1;
//   if ((inc = satoi(argv[1])) == 0)
//     return -1;
//   if ((use_sem = satoi(argv[2])) < 0)
//     return -1;

  if (use_sem){

    if(!is_sem_alive(SEM_ID)) {

      if(new_sem(1) == -1)  {

        print("Test_sync: Error creating semaphore\n");
        suicide();
      }        
    }

// Ahora debería encontrarlo:
    if(!is_sem_alive(SEM_ID)) {
      
      print("Test_sync: Error finding existing semaphore\n");
      suicide();
    }}


  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      down(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      down(SEM_ID);
  }

  if (use_sem){
    if(kill_sem(SEM_ID))  {

      print("Test_sync: Error killing semaphore\n");
      suicide();
    }}

  suicide();
}

void test_sync(int argc, char ** argv) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  // if (argc != 2)
  //   return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {

    pids[i] = spawn_process((int64_t) &my_process_dec, 3, argvDec, 1, 1);
    pids[i + TOTAL_PAIR_PROCESSES] = spawn_process((int64_t) &my_process_inc, 3, argvInc, 1, 1);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    my_wait(pids[i]);
    my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }


  char s[4];
  s[0] = '0' + global%10;
  s[1] = '0' + (global/10)%10;
  s[2] = '0' + (global/100)%10;
  s[3] = '\0';
  print(s);

  suicide();
}