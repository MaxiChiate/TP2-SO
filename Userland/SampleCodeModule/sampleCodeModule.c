#include <userlib.h>
#include <shell.h>
#include <processlib.h>
#include <Testing/test_util.h>
#include <Testing/tests.h>


int main() {

    waitpid(run_process(test_processes, 0, NULL, 1, true));
    // run_process(current_pid(), &test_prio, 0, NULL, 1, true);


    // char buffer[254] = {'\0'};

    // do	{
    //     read(buffer);
    //     getMenu(buffer);
    // }	while(1);

	return 0;
}