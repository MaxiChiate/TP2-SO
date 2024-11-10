#include <Apps/apps.h>

void mem(int argc, char ** argv){

    mem_t data = mem_data();

    printUlong(data->total_space);
    print("\n");
    printUlong(data->used_space);
    suicide();
}