#ifndef __KVTree_h
#define __KVTree_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct KVTreeCDT * KVTree;

typedef int key_t;
typedef char * value_t;

/*
 * Retorna 0 si e1 es igual a e2, negativo si e1 es menor que e2 o positivo si e1 es mayor que e2
 */

static int compareKeys(key_t key1, key_t key2) {
    return key1 - key2;
}

static int compareValues(value_t value1, value_t value2) {
    return strcmp(value1, value2);
}


/*
 * Reserva memoria para un nuevo árbol binario
 */
KVTree newKVTree();

/*
 * Retorna el tamaño del árbol binario
 */
unsigned int size(KVTree bst);

/*
 * Retorna true si árbol está vacío, false si no
 */
bool isEmpty(KVTree bst);

/*
 * Inserta un keyento en el árbol binario
 */
bool insert(KVTree bst, key_t key, value_t value);

/*
 * Borra un keyento del árbol binario
 */
bool discardEntry(KVTree bst, key_t key);

/*
 * Retorna la altura del árbol binario
 */
unsigned int height(KVTree bst);

/*
 * Retorna 1 si  key pertenece al árbol, 0 si no
 */
bool containsKey(KVTree bst, key_t key);

/*
 * Retorna 1 si value pertenece al árbol, 0 si no
 */
bool containsValue(KVTree bst, value_t value);


/*
 * Retorna el value asociado a key
 */
value_t get(KVTree bst, key_t key);

/*
 * Reemplaza el value asociado a key por newValue
 */
bool replace(KVTree bst, key_t key, value_t newValue);

/*
 * Retorna un vector con las keys del árbol en orden
 */
key_t * keySet(KVTree bst);


/*
 * Retorna un vector con los values del árbol
 */
 value_t * values(KVTree bst);

/*
 * Libera la memoria reservada para el árabol
 */
void freeKVTree(KVTree bst);

/*
 * Imprime todos los keyentos del árbol
 */
void printKVTree(KVTree bst);

#endif
