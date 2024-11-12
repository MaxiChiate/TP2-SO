#include <Apps/apps.h>

void block(int argc, char ** argv) {

    if (argc == 0) {
        print(ERROR_MESSAGE_FEW);

    } else if (argc > 1) {
        print(ERROR_MESSAGE_MANY);

    } else {
        int64_t to_block = satoi(argv[0]);
        if (to_block == SHELL_PID) {
            puts("Cannot block shell");

        } else if (to_block == HLT_PID) {
            puts("Cannot block halt");

        } else if (blockp(to_block)) {
            print("Blocking process ");
            printUinteger(to_block);
            puts("");

        } else {
            print("Process "); printUinteger(to_block); puts(" not found");
        }

    }

    suicide();
}