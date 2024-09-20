//memory_manager.h

#ifndef MM_MANAGER_H
#define MM_MANAGER_H

  void mm_init(void * that_start, int that_size);

  void * malloc(size_t size);

  void free(void * p);

#endif
