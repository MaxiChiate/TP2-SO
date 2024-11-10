#include <process/ipc_management.h>

#define IN_RANGE(x) ((x) >= 0 && (x) < MAX_FDS)

typedef enum {OPEN, CLOSED} state_t;

typedef struct {

    char * data;         
    int current_index;  
    bool std;   // Pensado para una posible futura impl de dup...
} buffer_t;


typedef struct fd {

    buffer_t buffer;
    state_t state;
} fd;

static char buffers[TOTAL_BUFFER_SIZE];
static int flags[MAX_FDS];
static fd descriptors[MAX_FDS];
static int next_index = FIRST_BUFFER;
static int currently_closed;

static char * calculate_buffer_start(int index);
static rw_flags_t get_flags(int fd);
static state_t get_state(int id);
static bool is_open(int fd);
static bool is_closed(int fd);
static fd create_descriptor(state_t state, int index);
static void memcpy_read(char *  dest, buffer_t *  src, int size, int *  bytes_read);
static void memcpy_write(buffer_t * dest, char * src, int size, int * bytes_written);
static int next();


void init_ipc() {

    for (int i = FIRST_BUFFER; i < MAX_FDS; i++) {

        descriptors[i] = create_descriptor(CLOSED, i);
        flags[i] = NO;
    }

    kernel_pipe(STDIN_FILENO, STDOUT_FILENO);

    descriptors[STDIN_FILENO].buffer.std = descriptors[STDOUT_FILENO].buffer.std = true;

    next_index = FIRST_BUFFER;
    currently_closed = MAX_FDS - FIRST_BUFFER;
}


int kernel_open(rw_flags_t flags_fd, int index, char * buffer)  {

    if (!IN_RANGE(index)) {

        return -1;   
    }

    descriptors[index].state = OPEN;

    descriptors[index].buffer.data = buffer;

    flags[index] = flags_fd;

    currently_closed--; 

    return index;
}

int open(rw_flags_t flags_fd) {

    int my_index = next();
    char * buffer = calculate_buffer_start(my_index);

    return kernel_open(flags_fd, my_index, buffer);
}

void close(int fd) {
    
    if (!IN_RANGE(fd)) {

        return;
    }

    if (is_open(fd)) {
        
        char eof = EOF;
        write(fd, &eof, 1);

        descriptors[fd].state = CLOSED;
        flags[fd] = NO;
    }

    currently_closed++;
}


static inline bool validate_flags(int fd, const rw_flags_t * f, int flags_amount)    {

    for(int i =0; i < flags_amount; i++) {

        if(flags[fd] == f[i])   {
            
            return true;
        }
    }

    return false;
}


int write(int fd, char * buf, int size) {

    rw_flags_t f[] = {RW, W};

    if(!IN_RANGE(fd) || is_closed(fd) || !validate_flags(fd, f, 2)) {
        
        return -1;    
    }

    return kernel_write(fd, buf, size);
}

int kernel_write(int fd, char * buf, int size) {

    if(buf == NULL) {

        return 0;
    }

    if(descriptors[fd].buffer.std)  {

        printTextDefault2(buf, STDOUT_COLOR, BACKGROUND_COLOR, size);
    }

    int bytes_written;

    memcpy_write(&descriptors[fd].buffer, buf, size, &bytes_written);

    return bytes_written;
}



int read(int fd, char * buf, int size) {
    
    rw_flags_t f[] = {RW, R};

    if (!IN_RANGE(fd) || is_closed(fd) || !validate_flags(fd, f, 2)) {
        return -1;
    }

    int bytes_read;

    memcpy_read(buf, &descriptors[fd].buffer, size, &bytes_read);

    return bytes_read;

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

void kernel_pipe(int fd1, int fd2)   {

    if(IN_RANGE(fd1) && IN_RANGE(fd2))    {

        kernel_open(R, fd1, calculate_buffer_start(fd1));
        kernel_open(W, fd2, calculate_buffer_start(fd1));
    }
}

void pipe(int fds[2])    {

    fds[0] = open(R);
    fds[1] = open(W);
    
    descriptors[fds[1]].buffer = descriptors[fds[0]].buffer;    
}


static inline char * calculate_buffer_start(int index)   {

    return buffers + (index * BUFFER_SIZE);
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

static fd create_descriptor(state_t state, int index) {

    fd to_return = {

        .buffer = { 
            
            .data = calculate_buffer_start(index), 
            .current_index = 0,
            .std = false
        },
        .state = state,
    };

    return to_return;
}

static void memcpy_read(char *  dest, buffer_t *  src, int size, int *  bytes_read)   {

    int i = 0;

    while(i < size && src->current_index < BUFFER_SIZE && src->data[src->current_index] != '\0' && src->data[src->current_index] != EOF) {

        dest[i++] = src->data[src->current_index++];
    }

    dest[i] = '\0';
    * bytes_read = i;
}

static void memcpy_write(buffer_t * dest, char * src, int size, int * bytes_written)   {

    int i;

    for (i = 0; i < size && dest->current_index < BUFFER_SIZE; i++) {

        dest->data[dest->current_index++] = src[i];
    }

    *bytes_written = i;
}


static int next()  {

    if( currently_closed > 0) {

        while(descriptors[next_index].state != CLOSED)  {

            next_index = FIRST_BUFFER + ((next_index + 1 - FIRST_BUFFER) % (MAX_FDS - FIRST_BUFFER));
        }

        return next_index;
    }
    
    return -1;
}
