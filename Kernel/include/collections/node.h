//node.h

#ifndef NODE_H
#define NODE_H

  #include <memory_manager.h>

  #define is_null(p)   ((p)==NULL)
  #define not_null(p)  (!is_null(p))

  typedef struct node * node_t;

  node_t node_init(void * data);

  void node_add_next(node_t current_node, node_t next_node);

  void node_add_tail(node_t first_node, node_t new_node);

  void * node_get_data(node_t node);

  node_t node_get_next(node_t node);

  void node_free(node_t node);

#endif
