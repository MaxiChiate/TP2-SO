#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <stringPrinter.h>
#include <buffer.h>

//memory_manager.h

#ifndef MM_MANAGER_H
#define MM_MANAGER_H

  #define BLOCK_SIZE  10

  void mm_init(void * that_start, int that_size);

  void * mm_malloc(size_t size);

  void mm_free(void * p);

#endif
