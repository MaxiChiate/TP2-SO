#ifndef __file__management
#define __file__management

#define PIPE_BUFFER_SIZE 1024
#define MAX_PIPES 256
#define MAX_FDS 256
#define MAX_FILES 256

/*
 * Duplicates an existing object descriptor and returns its value to the calling process
 */
int dup(file_descriptor_t old_fd);

/*
 * Redirects a process' input and output file descriptors
 */
int pipe(int fd[FILDES_AMOUNT]);

/*
 * Closes a file descriptor
 */
int close(file_descriptor_t fd);

/*
 * Opens a file
 */
int open(const char * filename);

#endif  //__file__management