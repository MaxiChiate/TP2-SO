#include <Apps/apps.h>

void block(int argc, char ** argv) {

    if (argc == 0) {
        print("pid is required\n");
    } else {
        print("Blocking process ");
        printUinteger((unsigned int) *argv[1]);
        print("\n");
        blockp((int) *argv[1]);
    }

    suicide();
}