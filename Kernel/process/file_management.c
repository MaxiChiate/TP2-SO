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
    bool is_open;
} file_descriptor_t

static file_descriptor_t fds[MAX_FDS];

// Un fd con valor i está en fds[i]. Tal vez sea mejor hacer un typedef int file_descriptor_t.

typedef struct file {
    const char * filename;
    bool is_open;
    file_descriptor_t fd;
} file_t;

file_t file_table[MAX_FILES];


// Inicializa fds y file_table
static void init_file_system() {
    for (int i = 0; i < MAX_FDS; i++) {
        fds[i].value = i;
        fds[i].is_open = false;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        file_table[i].filename = NULL;
        file_table[i].is_open = false;
        file_table[i].fd.value = -1;
    }
}



// fildes2 = dup(fildes)
int dup(int old_fd_id) {
    if (!VALID_FD(old_fd_id) || !fds[old_fd_id].is_open) {
        return -1;
    }

    int new_fd_index = -1;

    bool found_fd_index = false;
    for (int i = 0; i < MAX_FDS && !found_fd_index; i++) {
        if (!fds[i].is_open) {
            new_fd_index = i;
            found_fd_index = true;
        }
    }

    if (new_fd_index == -1) {
        return -1;
    }

    fds[new_fd_index].value = new_fd_index;
    fds[new_fd_index].is_open = true;

    bool found_file = false;
    for (int i = 0; i < MAX_FILES && !found_file; i++) {
        if (file_table[i].is_open && file_table[i].fd.value == old_fd_id) {
            file_table[i].fd = fds[new_fd_index];
            found_file = true;
        }
    }

    return fds[new_fd_index].value; //fds[new_fd_index].value = new_fd_index 
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


static int strlen(const char * str) {
    if (str == NULL) {
        return -1;
    }
    int length = 0;
    while (str[length++] != '\0');
    return length;
}

static bool strequals(const char * str1, const char * str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 != len2) {
        return false;
    } 

    for (int i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}


int open(const char * filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].is_open && strequals(file_table[i].filename == filename)) {
            return file_table[i].fd.value;
        }
    }

    int file_index = -1;
    bool found_file_index = false:
    for (int i = 0; i < MAX_FILES && !found; i++) {
        if (!file_table[i].is_open) {
            file_index = i;
            found_file_index = true;
        }
    }

    if (file_index == -1) {
        return -1;
    }

    int fd_index = -1;

    bool found_fd_index = false; 
    for (int i = 0; i < MAX_FDS && !found_fd_index; i++) {
        if (!fds[i].is_open) {
            fd_index = i;
            found_fd_index = true;
        }
    }

    if (fd_index == -1) {
        return -1;
    }

    file_table[file_index].filename = filename; 
    file_table[file_index].is_open = true;
    file_table[file_index].fd = fds[fd_index];

    fds[fd_index].is_open = true;

    return fds[fd_index].value;
}



int close(int fd_id) {

    if (!VALID_FD(fd_id) || !fds[fd_id].is_open) {
        return -1;
    }

    bool found_file = false;
    for (int i = 0; i < MAX_FILES && !found_file; i++) {
        if (file_table[i].is_open && file_table[i].fd.value == fd_id) {
            file_table[i].is_open = false;
            file_table[i].filename = NULL;
            found_file = true;
        }
    }

    fds[fd_id].is_open = false;

    return 1;
}
