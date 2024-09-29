#ifndef __bstADT_h
#define __bstADT_h

#include <stdio.h>
#include <stdlib.h>
#include "bool.h"

typedef struct bstCDT * bstADT;

typedef void * elemType;

/*
 * Reserva memoria para un nuevo árbol binario
 */
bstADT newBst(int(*compare)(elemType, elemType));

/*
 * Retorna el tamaño del árbol binario
 */
unsigned int size(bstADT bst);

/*
 * Inserta un elemento en el árbol binario
 */
bool insert(bstADT bst, elemType eleme);

/*
 * Borra un elemento en el árbol binario
 */
bool discardEntry(bstADT bst, elemType elem);

/*
 * Retorna la altura del árbol binario
 */
unsigned int height(bstADT bst);

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
