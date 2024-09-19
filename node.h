//node.h

#ifndef NODE_H
#define NODE_H


  #include <stdlib.h>

  typedef struct node * node_t;

  node_t node_init(void * data);

  void node_add_next(node_t current_node, node_t next_node);

#endif
