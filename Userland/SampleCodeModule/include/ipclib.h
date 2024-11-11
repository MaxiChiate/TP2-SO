#ifndef IPCLIB_H
#define IPCLIB_H

    #include <userlib.h>


    void pipe(int fd[2]);

    int dup(int fd);

    void dup2(int old_fd, int new_fd);

    int dup3(int old_fd, int new_fd, rw_flags_t new_flags);
    
#endif //IPCLIB_H