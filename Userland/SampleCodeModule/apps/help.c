#include <Apps/apps.h>
#include <shell.h>

void help(int argc, char ** argv) {
    
    if (argc!=1)    {

        print("help: ERROR argument amount\n");
        return;
    }

    print_commands();
    suicide();
}