#include <Apps/apps.h>

void mem(int argc, char ** argv){

    mem_t data = mem_data();
    print("Total memory:");
    printUlong(data->total_space);
    print("\n");
    print("Used memory:");
    printUlong(data->used_space);
    print("\n");
    print("Free memory:");
    printUlong(data->total_space - data->used_space);

    suicide();
}