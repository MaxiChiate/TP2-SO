//node.c

#include <collections/node.h>

struct node {

  struct node * next;
  void * data;
  uint16_t data_size;

};



node_t node_init(void * data, uint16_t data_size) {

  node_t to_return = (node_t) mm_malloc(sizeof(to_return[0]));

  if(not_null(to_return)) {

    void * data_copy = mm_malloc(data_size);

    if(not_null(data_copy)) {

      to_return->data = memcpy(data_copy, data, data_size);
      to_return->next = NULL;
      to_return->data_size = data_size;
    }
    else  {

      node_free(to_return);
    }
  }

  return to_return;
}



void node_add_next(node_t current_node, node_t next_node) {

  if(is_null(current_node)) {

      return;
  }

  //Must be same datatype, if @node_next is null, just assign it:

  if(not_null(next_node) && (current_node->data_size != next_node->data_size))  {

      return;
  }

  current_node->next = next_node;
}



void node_add_tail(node_t first_node, node_t new_node)  {

    if (is_null(first_node))  {
  
        return;
    }

    node_t current_node = first_node;
    
    while (not_null(current_node->next))  {
  
        current_node = current_node->next;
    }

    node_add_next(current_node, new_node);
}



void * node_get_data(node_t node) {

  return not_null(node)? node->data : NULL;
}



node_t node_get_next(node_t node) {

  return not_null(node)? node->next : NULL;
}



void node_free(node_t node)  {
  
  if(not_null(node))  {

      mm_free(node->data);
      mm_free(node);
  }
}

