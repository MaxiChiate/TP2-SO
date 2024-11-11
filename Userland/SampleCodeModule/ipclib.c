#include <ipclib.h>

void pipe(int fd[2]) {

    _int80(SYS_PIPE, (int64_t *) fd);
}

int dup(int fd)  {

    return (int) _int80(SYS_DUP, (int64_t *) &fd);    
}

void dup2(int old_fd, int new_fd) {

    int64_t args[] = {(int64_t) old_fd, (int64_t) new_fd};
    _int80(SYS_DUP2, args);
}

int dup3(int old_fd, int new_fd, rw_flags_t new_flags)  {
    
    int64_t args[] = {(int64_t) old_fd, (int64_t) (int64_t) args};
    return (int) _int80(SYS_DUP3, args);
}
