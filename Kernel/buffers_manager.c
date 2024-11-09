#include <buffers_manager.h>

#define IN_RANGE(x) ((x) >= FIRST_BUFFER && (x) < MAX_FDS)

#define STDOUT_BUFFER_DUMMY 0xFFFF
#define STDERR_BUFFER_DUMMY 0xEEEE

typedef enum {OPEN, CLOSED} state_t;

typedef struct buffer {
    char * start;
    state_t state;

} buffer;

static char buffers_heap[MAX_FDS * BUFFER_SIZE];

static int flags[MAX_FDS];

static buffer buffers[MAX_FDS];

static int last_index = FIRST_BUFFER;

static buffer create_buffer(state_t state, char * start) {

    buffer to_return = {
        .start = start,
        .state = state,
    };

    return to_return;
}


void init_file_system(int stdio_fd[2], int * stderr_fd) {

    for (int i = FIRST_BUFFER; i < MAX_FDS; i++) {
        
        buffers[i] = create_buffer(CLOSED, buffers_heap + (i * BUFFER_SIZE));
        flags[i] = NO;
    }

    pipe(stdio_fd);                  // OMG: Stdio  pipe, hi!
    *stderr_fd = open(R);            // OMG: Stderr, hi!
    last_index = FIRST_BUFFER;
}


int open(rw_flags_t flags_fd) {

    if (last_index >= MAX_FDS) {
        return -1;   
    }

    buffers[last_index].state = OPEN;

    if (last_index == STDOUT_FILENO)  {

        buffers[last_index].start = (char *) STDOUT_BUFFER_DUMMY;    
    }
    else if (last_index == STDERR)   {

        buffers[last_index].start = (char *) STDERR_BUFFER_DUMMY;
    }
    else    {

        buffers[last_index].start = buffers_heap + (last_index * BUFFER_SIZE);
    }

    flags[last_index] = flags_fd;

    return last_index++;
}

static inline rw_flags_t get_flags(int fd)  {

    return flags[fd];
}

static state_t get_state(int id) {
    return buffers[id].state;
}

static inline bool is_open(int fd) {
    return get_state(fd) == OPEN;
}

static inline bool is_closed(int fd) {
    return !is_open(fd);
}

void close(int fd) {
    
    if (!IN_RANGE(fd)) {
        return;
    }

    if (is_open(fd)) {
        
        char eof = EOF;
        write(fd, &eof, 1);

        buffers[fd].state = CLOSED;
        flags[fd] = NO;
    }

    if (last_index == 0) {
        return;
    }

    last_index--;
}

static void memcpy_write(char * dest, char * src, int size, int * bytes_written) {

    int i;
    for (i = 0; i < BUFFER_SIZE && i < size; i++) {
        dest[i] = src[i];
    }

    dest[i] = '\0';
    *bytes_written = i;

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

    if(!validate_flags(fd, f, 2) || !IN_RANGE(fd) || is_closed(fd)) {
        
        return -1;    
    }

    return kernel_write(fd, buf, size);
}

int kernel_write(int fd, char * buf, int size) {

    switch((int64_t) buf)  {

        case STDOUT_BUFFER_DUMMY: printTextDefault(buf, STDOUT_COLOR, BACKGROUND_COLOR); break;
        case STDERR_BUFFER_DUMMY: printTextDefault(buf, STDERR_COLOR, BACKGROUND_COLOR); break;

        default: break;
    }

    int bytes_written;

    memcpy_write(buffers[fd].start, buf, size, &bytes_written);

    return bytes_written;
}

static void memcpy_read(char * dest, char * src, int size, int * bytes_read) {

    int i;
    for (i = 0; i < BUFFER_SIZE && i < size && src[i] != '\0' && src[i] != EOF; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';

    *bytes_read = i;

}

int read(int fd, char * buf, int size) {
    
    rw_flags_t f[] = {RW, R};

    if (!validate_flags(fd, f, 2) || !IN_RANGE(fd) || is_closed(fd)) {
        return -1;
    }

    int bytes_read;

    memcpy_read(buf, buffers[fd].start, size, &bytes_read);

    return bytes_read;

}


int dup(int old_fd) {

    if (!IN_RANGE(old_fd)) {
        return -1;
    }

    int new_fd = open(flags[old_fd]);

    if (new_fd == -1) {
        return -1;
    }

    buffers[new_fd].start = buffers[old_fd].start;

    return new_fd;

}


int dup2(int old_fd, int new_fd) {

    if (!IN_RANGE(old_fd) || !IN_RANGE(new_fd)) {
        return -1;
    }

    if (old_fd == new_fd) {
        return new_fd;
    }

    if (is_closed(old_fd) || is_closed(new_fd)) {
        return -1;
    }

    buffers[new_fd].start = buffers[old_fd].start;
    flags[new_fd] = flags[old_fd];

    return new_fd;
}

int dup3(int old_fd, int new_fd, rw_flags_t new_flags)  {

    int to_return = dup2(old_fd, new_fd);
    
    if(to_return > 0) {
        flags[new_fd] = new_flags;
    }

    return to_return;
}

int pipe(int fds[2]) {

    fds[0] = open(R);
    fds[1] = open(W);
    
    buffers[fds[1]].start = buffers[fds[0]].start;

    return 0;
    
}
