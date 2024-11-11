#include <Apps/apps.h>

void nice(int argc, char ** argv) {

    if (argc < 2) {
        print(ERROR_MESSAGE_FEW);

    } else if (argc > 2) {
        print(ERROR_MESSAGE_MANY);

    } else {
        print("Changing process ");
        printUinteger((unsigned int) *argv[1]);
        print(" priority to ");
        printUinteger((unsigned int) *argv[1]);
        print("\n");
        nicep((int64_t) *argv[1], (int) *argv[2]);
    }

    suicide();
}