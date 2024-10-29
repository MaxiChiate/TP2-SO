

#include <Collections/list.h>

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

  my_list->size++;
}



size_t list_get_size(list_t my_list)  {

  return not_null(my_list) ? my_list->size : 0x00;
}



void * list_get_head(list_t my_list) {

  return not_null(my_list) ?
    node_get_data(my_list->head) : NULL;
}


void delete_first(list_t my_list) {
    if (is_null(my_list) || list_get_size(my_list) == 0) {
        return;
    }
    node_t to_free = my_list->head;
my_list->head = node_get_next(my_list->head);
mm_free(to_free);
my_list->size--;
}

void list_mm_free(list_t my_list) {
    if (is_null(my_list)) {
        return; // AsegÃºrate de que la lista no sea nula
    }

    node_t current = my_list->head;
    node_t next;

    while (current != NULL) {
        next = node_get_next(current); // Guarda el siguiente nodo
        node_free(current); // Libera el nodo actual
        current = next; // Avanza al siguiente nodo
    }

    mm_free(my_list); // Finalmente libera la lista
}