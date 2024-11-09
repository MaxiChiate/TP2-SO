#ifndef __BUFFERS_MANAGER_H
#define __BUFFERS_MANAGER_H

    #include <stdint.h>
    #include <bool.h>
    #include <defs.h>
    #include <systemcallDefs.h>
    #include <colours.h>
    #include <stringPrinter.h>

    #define FIRST_BUFFER 3
    #define MAX_FDS 256
    #define BUFFER_SIZE 32 

    #define STDOUT_COLOR WHITE
    #define STDERR_COLOR RED
    #define BACKGROUND_COLOR BLACK

    typedef struct buffer * buffer_t;


    void init_buffers(int stdio_fd[2], int * stderr_fd);

    int open(rw_flags_t flags_fd);

    void close(int fd);

    int write(int fd, char * buf, int size);

    int kernel_write(int fd, char * buf, int size);

    int read(int fd, char * buf, int size);

    int dup(int fd);

    int dup2(int fd1, int fd2);

    int dup3(int old_fd, int new_fd, rw_flags_t new_flags);

    int pipe(int fds[2]);

#endif //__BUFFERS_MANAGER_H