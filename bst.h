#ifndef __bstADT_h
#define __bstADT_h

typedef struct bstCDT * bstADT;

typedef int elemType;

/*
 * Reserva memoria para un nuevo árbol binario
 */
bstADT newBst(void);

/*
 * Retorna el tamaño del árbol binario
 */
unsigned int size(bstADT bst);

/*
 * Inserta un elemento en el árbol binario
 */
unsigned int insert(bstADT bst, elemType eleme);

/*
 * Retorna la altura del árbol binario
 */
unsigned int height(bstADT bst);

/*
 * Retorna 1 si el elemento pertenece al árbol, 0 si no
 */
unsigned int belongs(bstADT bst, elemType elem);

/*
 * Retorna un vector con los elementos del árbol en orden
 */
elemType * inOrder(bstADT bst);

/*
 * Libera la memoria reservada para el árabol
 */
void freeBst(bstADT bst);

/*
 * Imprime todos los elementos del árbol
 */
void printBst(bstADT bst);

#endif
