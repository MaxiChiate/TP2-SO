#ifndef __TreeMap_h
#define __TreeMap_h

#include <memory_manager.h>
#include <collections/KVTree.h>

typedef struct TreeMapCDT * TreeMap;

static int hash(key_t key) {
    return key % 10;
}

/*
 * Reserva memoria para un nuevo árbol binario
 */
TreeMap newTreeMap(int (*compareKeys)(key_t, key_t), int (*compareValues)(value_t, value_t));

/*
 * Retorna true si el mapa está vacío, false si no
 */
bool mapIsEmpty(TreeMap treeMap);

/*
 * Retorna el tamaño del mapa
 */
int mapSize(TreeMap treeMap);

/*
 * Inserta un key en el árbol binario
 */
bool put(TreeMap treeMap, key_t key, value_t value);

/*
 * Borra un key del árbol binario
 */
bool discard(TreeMap treeMap, key_t key);

/*
 * Retorna 1 si key pertenece al árbol, 0 si no
 */
bool containsK(TreeMap treeMap, key_t key);

/*
 * Retorna 1 si value pertenece al árbol, 0 si no
 */
bool containsV(TreeMap treeMap, value_t value);

/*
 * Retorna el value asociado a key
 */
value_t getValue(TreeMap treeMap, key_t key);

/*
 * Reemplaza el value asociado a key por newValue
 */
bool replaceValue(TreeMap treeMap, key_t key, value_t newValue);

/*
 * Retorna un vector con las keys del árbol en orden
 */
key_t * keys(TreeMap treeMap);

/*
 * Retorna un vector con los values del árbol
 */
 value_t * valueSet(TreeMap treeMap);

/*
 * Libera la memoria reservada para el árabol
 */
void freeTreeMap(TreeMap treeMap);


#endif
