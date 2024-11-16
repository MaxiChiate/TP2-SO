#ifndef IPCLIB_H
#define IPCLIB_H

    #include <userlib.h>

    void pipe(int fd[2]);
    void set_stdout_fd(int64_t pid, int new_fd);
    void set_stdin_fd(int64_t pid, int new_fd);
    void set_stdio(int fdin, int fdout);
    int get_stdout_fd(int64_t pid);
    int get_stdout_fd(int64_t pid);
    void close(int fd);
    int consume_stdin();
    
#endif //IPCLIB_H