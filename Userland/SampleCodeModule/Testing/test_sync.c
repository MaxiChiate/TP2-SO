#include <stdint.h>
#include <stdio.h>
#include <Testing/syscall.h>
#include <Testing/test_util.h>


#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc)   {

  uint64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[])    {

  uint64_t n;
  int8_t inc;
  int8_t use_sem;
  int8_t mutex;

  if (argc != 4){
    print("Error: Incorrect argc in my_process_inc\n");
    suicide();
}
  if ((n = satoi(argv[0])) <= 0  || 
        (inc = satoi(argv[1])) == 0 ||
        (use_sem = satoi(argv[2])) < 0 ||
        (mutex = satoi(argv[3])) < 0 )  {
        print("Error: arguments in my_process_inc\n");
        suicide();
    }

  if (use_sem)
    if (!is_sem_alive(mutex)) {
      print("test_sync:my_process_inc: ERROR opening semaphore\n");
      suicide();
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      down(mutex);
    slowInc(&global, inc);
    if (use_sem)
      up(mutex);
  }

    print("my_process_inc: OK\n");
    suicide();
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)    {

    print("Error: Incorrect argc in test_sync\n");
    suicide();
  }
    

    // Creamos el semaforo por fuera de los procesos asi sabemos su id (no es argumento de la funcion):

    int8_t mutex = new_sem(1);

    // Se convierte a string para pasarlo en argv:

    int digits = getUintDigits(mutex);
    char mutex_toString[digits+1];
    uIntToString(mutex, mutex_toString, digits);

  char *argvDec[] = {argv[0], "-1", argv[1], mutex_toString, NULL};
  char *argvInc[] = {argv[0], "1", argv[1], mutex_toString, NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)    {
    pids[i] = spawn_process(&my_process_inc, 4, argvDec, 1, true);
    pids[i + TOTAL_PAIR_PROCESSES] = my_create_process(&my_process_inc, 4, argvInc, 1, true);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    waitpid(pids[i]);
    waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
  }


    digits = getUintDigits(global)
    char global_toString[digits+1];
    uIntToString(global, global_toString, digits);

    print("Final value: ");
    print(global_toString);
    putEnter();

    suicide();
}