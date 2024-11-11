#include <Apps/apps.h>

void block(int argc, char ** argv) {

    int64_t args[] = {(int64_t) argv[0]};
    print("Blocking process ");
    printUinteger(argv[0]);
    print('\n');
    _int80(SYS_BLOCK_PROCESS, args);

    suicide();
}