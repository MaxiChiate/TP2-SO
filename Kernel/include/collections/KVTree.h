#ifndef __KVTree_h
#define __KVTree_h

#include <memory_manager.h>
#include <bool.h>


typedef struct KVTreeCDT * KVTree;

typedef int key_t;
typedef char * value_t;

/*
 * Reserva memoria para un nuevo árbol binario
 */
KVTree newKVTree(int(*compareKeys)(key_t, key_t), int (*compareValues)(value_t, value_t));

/*
 * Retorna el tamaño del árbol binario
 */
unsigned int KVTreeSize(KVTree bst);

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

// /*
//  * Imprime todos los keyentos del árbol
//  */
// void printKVTree(KVTree bst);

#endif
