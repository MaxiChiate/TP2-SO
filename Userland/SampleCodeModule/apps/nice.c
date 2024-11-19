#include <Apps/apps.h>

#define IN_RANGE(x, y, z) (x >= 0 && x < z)

void nice(int argc, char ** argv) {

    if (argc < 2) {
        print(ERROR_MESSAGE_FEW);

    } else if (argc > 3) {
        print(ERROR_MESSAGE_MANY);

    } else {

        int64_t pid = satoi(argv[1]);
        int new_priority = satoi(argv[2]);

        if (!IN_RANGE(new_priority, 0, 3)) {
            print("Priority ");
            printUinteger(new_priority);
            puts(" is inexistent");

        } else if (nicep(pid, new_priority)) {
            print("Changing process ");
            printUinteger(pid);
            print(" priority to ");
            printUinteger(new_priority);
            puts("");

        } else {
            print("Process "); printUinteger(pid); puts(" not found");
        }
    }

    suicide();
}