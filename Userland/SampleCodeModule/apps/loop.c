#include <Apps/apps.h>

void loop(int argc, char ** argv){

    uint64_t pid= current_pid();
    
    while(true){
        print("\nHola, este es mi pid : ");
        printUinteger(pid);

        sleep(1000);
    }

    suicide();
}

