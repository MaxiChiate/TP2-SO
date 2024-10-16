//lsit.c

#include <collections/list.h>


struct list {

  node_t head;
  size_t size;

};


list_t list_init()  {

  list_t to_return = mm_malloc(sizeof(to_return[0]));

  to_return->head = NULL;
  to_return->size = 0;

  return to_return;
}



void list_add(list_t my_list, void * data) {

  node_t new_node = node_init(data);

  if(is_null(my_list->head))  {
  
      my_list->head = new_node;    
  }  
  else  {

      node_add_tail(my_list->head, new_node);
  }
}



size_t list_get_size(list_t my_list)  {

  return not_null(my_list) ? my_list->size : 0x00;
}



void * list_get_head(list_t my_list) {

  return not_null(my_list) ? 
    node_get_data(my_list->head) : NULL;
}



void list_mm_free(list_t my_list)  {


    node_t n = my_list->head; 
    
    while((n = node_get_next(n)) != NULL) {
        
          node_free(n);   
      }

    mm_free(my_list);
}
