#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <stringPrinter.h>
#include <buffer.h>
#include <bool.h>

//memory_manager.h

#ifndef MM_MANAGER_H
#define MM_MANAGER_H

  #define K 1024
  #define MAX_HEAP_DIM 10*K

  void mm_init(void * start_given, int size_requested);

  void * mm_malloc(size_t size);

  void mm_free(void * p);

#endif

