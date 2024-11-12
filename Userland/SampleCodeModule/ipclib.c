#include <ipclib.h>

void pipe(int fd[2]) {

    _int80(SYS_PIPE, (int64_t *) &fd);
}

void set_stdout_fd(int64_t pid, int new_fd) {

    int64_t args[] = {pid, (int64_t) new_fd};

    _int80(SYS_SET_STDOUT_FD, args);
}
            
void set_stdin_fd(int64_t pid, int new_fd)  {

    int64_t args[] = {pid, (int64_t) new_fd};

    _int80(SYS_SET_STDIN_FD, args);
}
            
void close(int fd)  {

    _int80(SYS_CLOSE, (int64_t *) &fd);
}