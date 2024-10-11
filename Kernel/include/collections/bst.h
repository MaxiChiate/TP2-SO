#ifndef __bstADT_h
#define __bstADT_h

#include <memory_manager.h>
#include <bool.h>

typedef struct bstCDT * bstADT;

typedef void * elemType;

/*
 * Reserva memoria para un nuevo árbol binario
 */
bstADT newBst(int(*compare)(elemType, elemType));

/*
 * Retorna el tamaño del árbol binario
 */
unsigned int bstSize(bstADT bst);

/*
 * Inserta un elemento en el árbol binario
 */
bool bstInsert(bstADT bst, elemType eleme);

/*
 * Borra un elemento en el árbol binario
 */
bool bstDiscardEntry(bstADT bst, elemType elem);

/*
 * Retorna la altura del árbol binario
 */
unsigned int bstHeight(bstADT bst);

/*
 * Retorna 1 si el elemento pertenece al árbol, 0 si no
 */
bool belongs(bstADT bst, elemType elem);

/*
 * Retorna un vector con los elementos del árbol en orden
 */
elemType * inOrder(bstADT bst);

/*
 * Libera la memoria reservada para el árabol
 */
void freeBst(bstADT bst);


#endif
