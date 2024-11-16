#include <ipclib.h>

void pipe(int fd[2]) {

    _int80(SYS_PIPE, (int64_t *) &fd);

    assert((fd[0] >= 0) && (fd[1] >= 0), "Pipe creation");
}

void set_stdout_fd(int64_t pid, int new_fd) {

    int64_t args[] = {pid, (int64_t) new_fd};

    _int80(SYS_SET_STDOUT_FD, args);
}
            
void set_stdin_fd(int64_t pid, int new_fd)  {

    int64_t args[] = {pid, (int64_t) new_fd};

    _int80(SYS_SET_STDIN_FD, args);
}

void set_stdio(int fdin, int fdout) {

    int64_t args[] = {(int64_t) fdin, (int64_t) fdout};

    _int80(SYS_SET_STDIO, args);
}
            
void close(int fd)  {

    _int80(SYS_CLOSE, (int64_t *) &fd);
}

int consume_stdin()   {

    return (int) _int80(SYS_CONSUME_STDIN, NULL);
}

int get_stdout_fd(int64_t pid)  {

    return (int) _int80(SYS_GET_STDOUT_FD, &pid);
}

int get_stdin_fd(int64_t pid)  {

    return (int) _int80(SYS_SET_STDIN_FD, &pid);
}