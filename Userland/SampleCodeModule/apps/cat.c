#include <Apps/apps.h>

#define BUFF_LEN (1<<10)

static char buff[BUFF_LEN];

void cat(int argc, char ** argv){        

    read_until(buff, BUFF_LEN, 0);

    putEnter();
    print(buff);
    suicide();
}

