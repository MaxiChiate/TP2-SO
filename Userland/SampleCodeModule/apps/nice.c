#include <Apps/apps.h>

void nice(int argc, char ** argv) {

    if (argc < 2) {
        print("App requires pid and new priority\n");
    } else {
        print("Changing ");
        printUinteger((unsigned int) *argv[1]);
        print("priority to ");
        printUinteger((unsigned int) *argv[1]);
        nicep((int) *argv[1], (int) *argv[2]);
    }

    suicide();
}