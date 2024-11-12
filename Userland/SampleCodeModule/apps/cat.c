#include <Apps/apps.h>
#include <shell.h>

#define BUFFER_LEN (1<<10)

static char buff[BUFFER_LEN+1];

void cat(int argc, char ** argv){

    read(buff,BUFFER_LEN);

    print(buff);
    
    suicide();
}
