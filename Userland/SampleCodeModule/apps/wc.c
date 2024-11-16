#include <Apps/apps.h>


void wc(int argc, char ** argv) {

    char ans[BUFF_SIZE] = {'\0'};
    int enter_counter = 0;

    int length = read_into_buffer(ans, BUFF_SIZE);

    for(int i = 0; i < length; i++)  {

        enter_counter += ans[i] == KEY_ENTER;
    }

    itoa(enter_counter, ans);

    putEnter();
    print(ans);
    print(" enters!\n");

    suicide();
}