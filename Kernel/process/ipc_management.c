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
        flags[i] = NONE;
    }

    next_index = FIRST_BUFFER;
    currently_closed = MAX_FDS - STD_FD_COUNT;

    kernel_pipe(STDIN_FILENO, STDOUT_FILENO, buffer_init(STD_BUFFER_SIZE));
    kernel_open(W, DEV_NULL, NULL);
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
    buffer_t buffer = buffer_init(BUFFER_SIZE);

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
        flags[fd] =NONE;
    
        currently_closed++;   
    }
}



int write(int fd, char * buf, int size) {

    rw_flags_t f[] = {RW, W};

    return validate_flags(fd, f, 2) ? kernel_write(fd, buf, size) : -1;
}


int kernel_write(int fd, char * buf, int size) {

    if (!IN_RANGE(fd) || is_closed(fd) || buf == NULL || size < 0)  {

        return -1;
    }

// No hay duplicates, globalemente STDOUT siempre imprime.
// Los procesos para imprimir en panatalla no saben el fd global de stdout, usan su propia "tabla"

    if(fd == STDOUT_FILENO)  {  

        printTextDefault2(buf, STDOUT_COLOR, BACKGROUND_COLOR, size);

    // Ya se "escribio" en pantalla, no hay necesidad de escribirlo en buffer.
        return size;
    }

    if(fd == DEV_NULL)  {

        return size;
    }

    return buffer_write(descriptors[fd].buffer, buf, size);
}



int read(int fd, char * buf, int size)  {

    rw_flags_t f[] = {RW, R};

    if (!IN_RANGE(fd) || is_closed(fd) || buf == NULL 
        || !validate_flags(fd, f, 2) || fd == DEV_NULL || size < 0) {

        return EOF;
    }

    return buffer_read(descriptors[fd].buffer, buf, size);
}


int read_all(int fd, char * buf)    {

    return buffer_read_all(descriptors[fd].buffer, buf);
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

        kernel_pipe(fds[0], fds[1], buffer_init(BUFFER_SIZE));
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
