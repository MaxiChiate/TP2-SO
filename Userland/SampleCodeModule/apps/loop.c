#include <Apps/apps.h>

void loop(int argc, char ** argv){

    uint64_t pid= current_pid();
    
    while(true){
        print("\nHola, este es mi pid : ");
        printUinteger(pid);

        int64_t args[] = {1000}; // 1 segundo
        _int80(SYS_SLEEP, args);
    }

    suicide();
}

