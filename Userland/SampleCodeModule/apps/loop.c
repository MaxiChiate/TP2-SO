#include <Apps/apps.h>

void loop(int argc, char ** argv){

    uint64_t pid= current_pid();
    
    while(true){
        print("\nHi, this is my pid : ");
        printUinteger(pid);

        sleep(1000);
    }

    suicide();
}

