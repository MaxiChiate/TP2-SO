#include <userlib.h>
#include <shell.h>

int main() {

    spawn_process(current_pid(), &test_processes, 0, NULL, 1, true);
    // spawn_process(current_pid(), &test_prio, 0, NULL, 1, true);


    // char buffer[254] = {'\0'};

    // do	{
    //     read(buffer);
    //     getMenu(buffer);
    // }	while(1);

	return 0;
}