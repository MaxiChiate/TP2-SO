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

void memcpy(void *dest, void *src, uint32_t n) {

    char *d = (char *)dest;
    const char *s = (char *)src;

    for (uint64_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

void * dum_realloc(void *ptr,uint32_t old_size, uint32_t new_size) {

    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    if (ptr == NULL) {
        return malloc(new_size);
    }

    void *new_ptr = malloc(new_size);
    if (new_ptr== NULL) {
        return NULL;
    }

    memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);

    free(ptr);

    return new_ptr;
}