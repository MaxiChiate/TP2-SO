#include <Apps/apps.h>

void kill(int argc, char ** argv) {

    if (argc == 0) {
        print(ERROR_MESSAGE_FEW);

    } else if (argc > 1) {
        print(ERROR_MESSAGE_MANY);

    } else {
        int64_t to_kill = satoi(argv[0]);
        if (killp(to_kill)) {
            print("Killing process ");
            printUinteger(to_kill);
        } 
        else {
            print("Process "); print(argv[0]); puts(" not found");
        }
    }

    suicide();

}