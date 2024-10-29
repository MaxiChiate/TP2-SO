//list.h

#ifndef LIST_H
#define LIST_H

  #include <memory_manager.h>
  #include <collections/node.h>

  typedef struct list * list_t;

  list_t list_init();

  void list_add(list_t my_list, void * data);

  size_t list_get_size(list_t my_list);

  void * list_get_head(list_t my_list);

  void delete_first(list_t my_list);

  void list_mm_free(list_t my_list);
#endif
