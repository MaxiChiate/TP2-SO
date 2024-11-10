#include <Apps/apps.h>

static void busy_waiting(int d){

    for(int i=0;i<d;i++){

    }

}
void loop(int argc, char ** argv){

    uint64_t pid= current_pid();
    
    while(true){
        print("\nHola, este es mi pid :\n");
        printUinteger(pid);
        busy_waiting(10000);
    }

    suicide();
}

