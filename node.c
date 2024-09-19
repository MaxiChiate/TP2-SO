//node.c

#include "node.h"

//#include <stdio.h>

#define isNull(p)   ((p)==NULL)
#define notNull(p)  (!isNull(p))

//#define NODE_ADD_NEXT__INCOMPATIBLE_DATATYPE__ERROR "node_add_next() failed: Incompatible datatype between nodes"
//#define printError(str) (fprintf(stderr, (str)))

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

  if(isNull(current_node)) {
  
      return;    
  }


  //Must be same datatype, if @node_next is null, just assign it:
  
  if(notNull(next_node) && (sizeof(current_node->data[0]) != sizeof(next_node->data[0])))  {
    
//      printError(NODE_ADD_NEXT__INCOMPATIBLE_DATATYPE__ERROR);
      return;
  }


  current_node->next = next_node;

}


/*
int main()  {

  int number = 10;
  int * value1 = &number;

  node_t node1 = node_init(value1);

  printf("node1: %p, %p\n", node1->data, node1->next);
  
  int rebmun = 20;
  int * value2 = &rebmun;

  node_t node2 = node_init(value2);

  printf("node2: %p, %p\n", node2->data, node2->next);

  node_add_next(node1, node2);

  printf("node1: %p, %p\n", node1->data, node1->next);

  printf("node2: %p, %p\n", node1->next->data, node1->next->next);

}*/
