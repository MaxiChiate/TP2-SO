//memory_manager.h

#ifndef MM_MANAGER_H
#define MM_MANAGER_H

  #include <stdint.h>
  #include <bool.h>
  #include <defs.h>

  #define K 1024
  #define MAX_HEAP_DIM 10*K

  void mm_init(void * start_given, int size_requested);

  void * mm_malloc(uint32_t size);

  void mm_free(void * p);

#endif

