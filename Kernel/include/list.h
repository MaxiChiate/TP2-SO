//list.h

#ifndef LIST_H
#define LIST_H

  #include "node.h"

  typedef struct list * list_t;  

  list_t list_init();

  size_t list_get_size(list_t my_list);

  void * list_get_head(list_t my_list);

  void list_free(list_t my_list);

#endif
