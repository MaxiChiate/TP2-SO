//heap.c

#include <heaplib.h>

void * malloc(uint32_t size) {

    return (void *) _int80(SYS_MM_MALLOC, (int64_t *) &size);
}

void free(void * ptr) {

    _int80(SYS_MM_FREE, (int64_t *) &ptr);
}

mem_t mem_data(){
    return (mem_t) _int80(SYS_MM_MEM,NULL);
}