#include <Apps/apps.h>

void block(int argc, char ** argv) {

    if (argc == 0) {
        print(ERROR_MESSAGE_FEW);

    } else if (argc > 1) {
        print(ERROR_MESSAGE_MANY);

    } else {
        print("Blocking process ");
        printUinteger((unsigned int) *argv[1]);
        print("\n");
        blockp((int64_t) *argv[1]);
    }

    suicide();
}