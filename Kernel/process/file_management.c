#include <file_management.h>

typedef struct pipe {
    char buffer[PIPE_BUFFER_SIZE];
    int read_pos;
    int write_pos;
    bool is_open;
} pipe_t;

pipe_t pipes_table[MAX_PIPES];

typedef struct file_descriptor {
    int value;
} file_descriptor_t

static file_descriptor_t fds[MAX_FDS];

typedef struct file {
    const char * filename;
    bool is_open;
    file_descriptor_t fd;
} file_t;

file_t file_table[MAX_FILES];


// Inicializa fds y file_table
static void init_file_system() {
    for (int i = 0; i < MAX_FDS; i++) {
        fds[i].is_open = false;
        fds[i].file_pointer = 0;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        file_table[i].filename = NULL;
        file_table[i].is_open = false;
        file_table[i].fd.is_open = false;
        file_table[i].fd.file_pointer = 0;
    }
}


// fildes2 = dup(fildes)
int dup(file_descriptor_t old_fd) {
    if (!VALID_FD(old_fd) || !fds[old_fd].is_open) {
        return -1;
    }

    for (int new_fd = 0; new_fd < MAX_FDS; new_fd++) {
        if (!fds[new_fd].is_open) {
            fds[new_fd] = fds[old_fd];
            fds[new_fd].is_open = true;
            return new_fd;
        }
    }
    // No hay descriptores libres
    return -1;
}

static int find_free_pipe_index() {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (!pipes_table[i].is_open) {
            return i;
        }
    }
    return -1;
}


int pipe(int fd[FILDES_AMOUNT]) {

    for (int i = 0; i < MAX_FDS; i++) {
        if (!pipes_table[i].is_open) {
            pipes[i].read_pos = 0;
            pipes[i].write_pos = 0;
            pipes[i].is_open = true;
        }

        // Devuelve dos descriptores, uno para lectura y otro para escritura.
        fd[0] = i;
        fd[1] = i + 1;

        return 0;
    }

    return -1;
}



int open(const char * filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].is_open && file_table[i].filename == filename) {
            return i; // Retorna el descriptor existente si ya está abierto
        }
    }

    int fd_index = -1;
    bool found = false:
    for (int i = 0; i < MAX_FDS && !found; i++) {
        if (!fds[i].is_open) {
            fd_index = fds[i].value;
            found = true;
        }
    }

    if (fd_index == -1) {
        return -1;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!file_table[i].is_open) {
            file_table[i].filename = filename;
            file_table[i].is_open = true;
            file_table[i].fd = fds[fd_index];
            file_table[i].fd.value = fd_index;
            return fd_index;
        }
    }
    return -1;
}



int close(int fd_index) {

    //int fd_index = get_index_by_fd(fd);
    if (!VALID_FD(fd_index) || !fds[fd_index].is_open) {
        return -1;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].is_open && file_table[i].fd.value == fd_index) {
            file_table[i].is_open = false;
            file_table[i].filename = NULL;
        }
    }

    fds[fd_index].is_open = false;
    fds[fd_index].file_pointer = 0;

    return 1;
}
