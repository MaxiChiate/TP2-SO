#ifndef __GLOBALFD_H
#define __GLOBALFD_H

    typedef enum { 
        
        STDIN_FILENO = 0, 
        STDOUT_FILENO, 
        DEV_NULL, 
        
        STD_FD_COUNT
    
    } std_fd;

#endif