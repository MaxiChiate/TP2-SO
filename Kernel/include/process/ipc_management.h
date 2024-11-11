#ifndef __BUFFERS_MANAGER_H
#define __BUFFERS_MANAGER_H

    #include <stdint.h>
    #include <bool.h>
    #include <defs.h>
    #include <systemcallDefs.h>
    #include <colours.h>
    #include <stringPrinter.h>
    #include <Collections/buffer.h>

    #define IS_STANDARD(id) ((id) == STDIN_FILENO || (id) == STDOUT_FILENO)

// Se asume que los fd standard están en las primeras STD_FD_COUNT posiciones
    #define FIRST_BUFFER (STD_FD_COUNT + 1)
    #define MAX_FDS 32
    #define BUFFER_SIZE 128
    #define STD_BUFFER_SIZE 1024 
    #define TOTAL_BUFFER_SIZE (MAX_FDS * BUFFER_SIZE)

    #define STDOUT_COLOR WHITE
    #define BACKGROUND_COLOR BLACK

    void init_ipc();

    int open(rw_flags_t flags_fd);
    int kernel_open(rw_flags_t flags_fd, int index, buffer_t buffer);

    void close(int fd);

    int write(int fd, char * buf, int size);
    int kernel_write(int fd, char * buf, int size);

    int read(int fd, char * buf, int size);

    int dup(int fd);
    int dup2(int fd1, int fd2);
    int dup3(int old_fd, int new_fd, rw_flags_t new_flags);

    void pipe(int fds[2]);
    void kernel_pipe(int fd1, int fd2, buffer_t buffer);

#endif //__BUFFERS_MANAGER_H