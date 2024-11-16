#include <Apps/apps.h>

#define BUFF_LEN (1<<10)

static char buff[BUFF_LEN];

void cat(int argc, char ** argv){        

    read_into_buffer(buff, BUFF_LEN);

    putEnter();
    print(buff);
    suicide();
}

