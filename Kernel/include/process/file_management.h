// #ifndef __file__management
// #define __file__management

// #include <bool.h>

// #define PIPE_BUFFER_SIZE 1024
// #define MAX_PIPES 256
// #define MAX_FDS 256
// #define MAX_FILES 256
// #define FILDES_AMOUNT 2


// /*
//  * Duplicates an existing object descriptor and returns its value to the calling process
//  */
// int dup(int old_fd_id);

// /*
//  * Redirects a process' input and output file descriptors
//  */
// int pipe(int fd[FILDES_AMOUNT]);

// /*
//  * Closes a file whose file descriptor is fd_id
//  */
// int close(int fd_id);

// /*
//  * Opens a file
//  */
// int open(const char * filename);

// #endif  //__file__management