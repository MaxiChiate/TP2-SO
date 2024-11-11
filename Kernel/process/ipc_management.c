#include <process/ipc_management.h>

#define IN_RANGE(x) ((x) >= 0 && (x) < MAX_FDS)

typedef enum {OPEN, CLOSED} state_t;

typedef struct fd {

    buffer_t buffer;
    state_t state;

} fd;

static int flags[MAX_FDS];
static fd descriptors[MAX_FDS];
static int next_index;
static int currently_closed;

static rw_flags_t get_flags(int fd);
static state_t get_state(int id);
static bool is_open(int fd);
static bool is_closed(int fd);
static fd create_descriptor(state_t state);
static int next();
static inline bool validate_flags(int fd, const rw_flags_t * f, int flags_amount);


void init_ipc() {

    for (int i = 0; i < MAX_FDS; i++) {

        descriptors[i] = create_descriptor(CLOSED);
        flags[i] = NO;
    }

    kernel_pipe(STDIN_FILENO, STDOUT_FILENO, buffer_init(STD_BUFFER_SIZE, true));

    next_index = FIRST_BUFFER;
    currently_closed = MAX_FDS - STD_FD_COUNT;
}


int kernel_open(rw_flags_t flags_fd, int index, buffer_t buffer)  {

    if (!IN_RANGE(index) || is_open(index)) {

        return -1;   
    }

    descriptors[index].state = OPEN;

    descriptors[index].buffer = buffer;

    flags[index] = flags_fd;

    currently_closed--; 

    return index;
}



int open(rw_flags_t flags_fd) {

    int my_index = next();
    buffer_t buffer = buffer_init(BUFFER_SIZE, false);

    return kernel_open(flags_fd, my_index, buffer);
}



void close(int fd) {
    
    if (IN_RANGE(fd) && is_open(fd)) {

        if(buffer_unref(descriptors[fd].buffer))    {
        // Si alguien mas lo referencia le mando un EOF
            char eof = EOF;
            write(fd, &eof, 1);
        }

        descriptors[fd].state = CLOSED;
        flags[fd] = NO;
    
        currently_closed++;   
    }
}



int write(int fd, char * buf, int size) {

    rw_flags_t f[] = {RW, W};

    if(!IN_RANGE(fd) || is_closed(fd) || !validate_flags(fd, f, 2)) {
        
        return -1;    
    }

//Por seguridad, solo se imprime con permisos de escritura (kernel_write no lo valida):

    if(buffer_is_standard(descriptors[fd].buffer))  {

        printTextDefault2(buf, STDOUT_COLOR, BACKGROUND_COLOR, size);

    // Ya se "escribio" en pantalla, no hay necesidad de escribirlo en buffer.
        return size;
    }

    return kernel_write(fd, buf, size);
}

int kernel_write(int fd, char * buf, int size) {

    if(buf == NULL) {

        return -1;
    }

    return buffer_write(descriptors[fd].buffer, buf, size);
}



int read(int fd, char * buf, int size)  {

    rw_flags_t f[] = {RW, R};

    if (!IN_RANGE(fd) || is_closed(fd) || buf == NULL || !validate_flags(fd, f, 2)) {

        return -1;
    }

    return buffer_read(descriptors[fd].buffer, buf, size);
}

int dup(int fd) {

    return 0;
}

int dup2(int fd1, int fd2) {

    return 0;
}

int dup3(int old_fd, int new_fd, rw_flags_t new_flags)  {

    return 0;
}


void kernel_pipe(int fd1, int fd2, buffer_t buffer)   {

    if(IN_RANGE(fd1) && IN_RANGE(fd2) && buffer != NULL)    {

        kernel_open(R, fd1, buffer);
        kernel_open(W, fd2, buffer);

    // +1 ref, ya tenía un ref por el init
        buffer_ref(buffer);
    }
}

void pipe(int fds[2])    {

    if((fds != NULL) && ((fds[0] = next()) >= 0) && ((fds[1] = next()) >= 0))    {

        kernel_pipe(fds[0], fds[1], buffer_init(BUFFER_SIZE, false));
    }
}


static inline rw_flags_t get_flags(int fd)  {

    return flags[fd];
}

static state_t get_state(int id) {

    return descriptors[id].state;
}

static inline bool is_open(int fd) {

    return get_state(fd) == OPEN;
}

static inline bool is_closed(int fd) {

    return !is_open(fd);
}

static inline fd create_descriptor(state_t state) {

    return (fd) {
        
        .buffer = NULL,
        .state = state
    };
}

static int next()  {

    if( currently_closed > 0) {

        while(get_state(next_index) != CLOSED)  {

            next_index = FIRST_BUFFER + ((next_index + 1 - FIRST_BUFFER) % (MAX_FDS - FIRST_BUFFER));
        }

        return next_index;
    }
    
    return -1;
}

static bool validate_flags(int fd, const rw_flags_t * f, int flags_amount)    {

    for(int i =0; i < flags_amount; i++) {

        if(get_flags(fd) == f[i])   {
            
            return true;
        }
    }

    return false;
}



// int dup(int old_fd) {

//     if (!IN_RANGE(old_fd)) {
//         return -1;
//     }

//     int new_fd = open(flags[old_fd]);

//     if (new_fd > 0) {

//         descriptors[new_fd].buffer = descriptors[old_fd].buffer;    
//     }

//     return new_fd;

// }


// int dup2(int old_fd, int new_fd) {

//     if (!IN_RANGE(old_fd) || !IN_RANGE(new_fd)) {
//         return -1;
//     }

//     if (old_fd == new_fd) {
//         return new_fd;
//     }

//     if (is_closed(old_fd) || is_closed(new_fd)) {
//         return -1;
//     }

//     descriptors[new_fd].buffer = descriptors[old_fd].buffer;
//     flags[new_fd] = flags[old_fd];

//     return new_fd;
// }

// int dup3(int old_fd, int new_fd, rw_flags_t new_flags)  {

//     int to_return = dup2(old_fd, new_fd);
    
//     if(to_return > 0) {
//         flags[new_fd] = new_flags;
//     }

//     return to_return;
// }

