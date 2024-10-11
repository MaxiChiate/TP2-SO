//node.c

#include <node.h>

struct node {

  struct node * next;
  void * data;

};



node_t node_init(void * data) {

  node_t to_return = malloc(sizeof(to_return[0]));

  to_return->data = data;
  to_return->next = NULL;

  return to_return;
}



void node_add_next(node_t current_node, node_t next_node) {

  if(is_null(current_node)) {
  
      return;    
  }


  //Must be same datatype, if @node_next is null, just assign it:
  
  if(not_null(next_node) && (sizeof(current_node->data[0]) != sizeof(next_node->data[0])))  {
    
//      printError(NODE_ADD_NEXT__INCOMPATIBLE_DATATYPE__ERROR);
      return;
  }


  current_node->next = next_node;

}



void node_add_tail(node_t first_node, node_t new_node) {

  if(is_null(first_node))  {

      return;
  }

  node_t current_node = first_node;
  node_t next_node;

  while(not_null(next_node = node_get_next(current_node))) {

      current_node = next_node;
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

    free(node);
}


