//heaplib.h

#ifndef __HEAP_H
#define __HEAP_H

    #include <userlib.h>
    #include <stdint.h>

    void * malloc(uint32_t size);
    void free(void * ptr);

#endif