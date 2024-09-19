//node.h

#ifndef NODE_H
#define NODE_H


  #include <stdlib.h>

  typedef struct node * node_t;

  node_t node_init(void * data);

  void node_add_next(node_t current_node, node_t next_node);

  void * node_get_data(node_t node);

  node_t node_get_next(node_t node);
#endif
