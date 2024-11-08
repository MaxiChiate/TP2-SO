//list.h

#ifndef LIST_H
#define LIST_H

  #include <memory_manager.h>
  #include <collections/node.h>
  #include <bool.h>

  typedef struct list * list_t;

  list_t list_init(uint16_t data_size);

  void list_add(list_t my_list, void * data);

  uint32_t list_get_size(list_t my_list);

  void * list_get_head(list_t my_list);

  void list_delete_first(list_t my_list);

  bool list_is_empty(list_t my_list);

  void list_free(list_t my_list);
#endif
