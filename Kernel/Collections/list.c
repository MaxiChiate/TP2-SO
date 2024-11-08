

#include <Collections/list.h>

struct list {

  node_t head;
  uint32_t size;
  uint16_t data_size;

};


list_t list_init(uint16_t data_size)  {

  list_t to_return = (list_t) mm_malloc(sizeof(to_return[0]));

  if(!is_null(to_return)) {

    to_return->head = NULL;
    to_return->size = 0;
    to_return->data_size = data_size;
  }

  return to_return;
}



void list_add(list_t my_list, void * data) {

  node_t new_node = node_init(data, my_list->data_size);

  if(is_null(my_list->head))  {

      my_list->head = new_node;
  }
  else  {

      node_add_tail(my_list->head, new_node);
  }

  if(not_null(new_node))  {
    
    my_list->size++;
  }
}



uint32_t list_get_size(list_t my_list)  {

  return not_null(my_list) ? my_list->size : 0x00;
}



void * list_get_head(list_t my_list) {

  return not_null(my_list) ?
    node_get_data(my_list->head) : NULL;
}


bool list_is_empty(list_t my_list)  {

    return is_null(my_list) || list_get_size(my_list) == 0;
}

void list_delete_first(list_t my_list) {
  
  if (list_is_empty(my_list)) {
  
      return;
  }

  node_t to_free = my_list->head;
  my_list->head = node_get_next(my_list->head);
  node_free(to_free);
  my_list->size--;
}

void list_free(list_t my_list) {
    
    while(!list_is_empty(my_list))  {

        list_delete_first(my_list);
    }

    mm_free(my_list); 
}