//heaplib.h

#ifndef __HEAP_H
#define __HEAP_H

    #include <userlib.h>
    #include <stdint.h>

    typedef struct mem_struct{
    
        uint32_t total_space;
        uint32_t used_space;

    } mem_struct;

    typedef struct mem_struct * mem_t;

    void * malloc(uint32_t size);
    void free(void * ptr);
    mem_t mem_data();

#endif