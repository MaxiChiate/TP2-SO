#include <Apps/apps.h>

void ps(int argc, char ** argv) {


    ps_t to_assign;
    int64_t args[] = {(int64_t) &to_assign};
    _int80(SYS_PS, args);

    print_ps(to_assign);

    suicide();


}