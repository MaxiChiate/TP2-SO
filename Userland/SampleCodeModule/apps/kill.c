#include <Apps/apps.h>

void kill(int argc, char ** argv) {

    if (argc < 1) {
        print("App requires a pid\n");
    } else {
        print("Killin process ");
        printUinteger((unsigned int) *argv[1]);
        print("\n");
        killp((int) *argv[1]);
    }

    suicide();

}