#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <stringPrinter.h>
#include <buffer.h>

//memory_manager.h

#ifndef MM_MANAGER_H
#define MM_MANAGER_H

  #define K 1024
  #define BLOCK_SIZE  2*K

  void mm_init(void * start_given, int size_requested);

  void * mm_malloc(size_t size);

  void mm_free(void * p);

#endif
