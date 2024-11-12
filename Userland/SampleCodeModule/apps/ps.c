#include <Apps/apps.h>

void ps(int argc, char ** argv) {


    ps_t * to_print;
    int64_t args[] = {(int64_t) &to_print};

    _int80(SYS_PS, args);

    print_all_ps(&to_print);

    suicide();


}