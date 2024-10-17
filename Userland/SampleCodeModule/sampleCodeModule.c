#include <userlib.h>
#include <shell.h>
#include <processlib.h>
#include <Testing/tests.h>


int main() {

    bussy_wait(100000000);
    
    print("Entrega parcial 2, grupo 2");putnEnters(2);

    print("test_prio:");putnEnters(2);

    bussy_wait(100000000);

    char * prioargv[] = {"test_prio", "62", NULL};
    waitpid(run_process((int64_t) test_prio, 2, prioargv, 1, true));

    putnEnters(2);print("\ntest_process:\n");putnEnters(2);

    bussy_wait(100000000);
    char * testargv[] = {"test_process", "62", NULL};
    run_process((int64_t)test_processes, 2, testargv, 1, true);

    waitp();

    // char buffer[254] = {'\0'};

    // do	{
    //     read(buffer);
    //     getMenu(buffer);
    // }	while(1);

	return 0;
}


