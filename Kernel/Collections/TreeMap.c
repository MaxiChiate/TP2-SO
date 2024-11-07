#include <collections/TreeMap.h>

#define AMOUNT_TREES 10

typedef struct TreeMapCDT {
    KVTree trees[AMOUNT_TREES];
    uint32_t size;
    int (*compareKeys)(key_t, key_t);
    int (*compareValues)(value_t, value_t);
} TreeMapCDT;

TreeMap newTreeMap(int (*compareKeys)(key_t, key_t), int (*compareValues)(value_t, value_t)) {
    TreeMap treeMap = mm_malloc(sizeof(TreeMapCDT));
    treeMap->size = 0;
    for (int i = 0; i < AMOUNT_TREES; i++) {
        treeMap->trees[i] = newKVTree(compareKeys, compareValues);
    }
    return treeMap;
}

bool mapIsEmpty(TreeMap treeMap) {
    bool emptyTree = true;
    for (int i = 0; i < AMOUNT_TREES; i++) {
        if (!isEmpty(treeMap->trees[i])) {
            emptyTree = false;
        }
    }
    return emptyTree;
}

int mapSize(TreeMap treeMap) {
    return treeMap->size;
}

bool put(TreeMap treeMap, key_t key, value_t value) {
    int idx = hash(key);
    bool inserted = insert(treeMap->trees[idx], key, value);
    if (inserted) {
        treeMap->size++;
    }
    return inserted;
}

bool discard(TreeMap treeMap, key_t key) {
    int idx = hash(key);
    bool discarded = discardEntry(treeMap->trees[idx], key);
    if (discarded) {
        treeMap->size--;
    }
    return discarded;
}

bool containsK(TreeMap treeMap, key_t key) {
    int idx = hash(key);
    return containsKey(treeMap->trees[idx], key);
}

bool containsV(TreeMap treeMap, value_t value) {
    bool toReturn = false;
    for (int i = 0; i < AMOUNT_TREES; i++) {
        if (containsValue(treeMap->trees[i], value)) {
            toReturn = true;
        }
    }
    return toReturn;
}

value_t getValue(TreeMap treeMap, key_t key) {
    int idx = hash(key);
    return get(treeMap->trees[idx], key);
}

bool replaceValue(TreeMap treeMap, key_t key, value_t newValue) {
    int idx = hash(key);
    return replace(treeMap->trees[idx], key, newValue);
}

key_t * keys(TreeMap treeMap) {
    int toReturnIdx = 0;
    key_t * toReturn = mm_malloc(sizeof(key_t) * treeMap->size);
    for (int i = 0; i < AMOUNT_TREES; i++) {
        key_t * partialKeys = keySet(treeMap->trees[i]);
        int partialSize = KVTreeSize(treeMap->trees[i]);
        for (int j = 0; j < partialSize; j++) {
            toReturn[toReturnIdx++] = partialKeys[j];
        }
    }
    return toReturn;
}

value_t * valueSet(TreeMap treeMap) {
    int toReturnIdx = 0;
    value_t * toReturn = mm_malloc(sizeof(key_t) * treeMap->size);
    for (int i = 0; i < AMOUNT_TREES; i++) {
        value_t * partialValues = values(treeMap->trees[i]);
        int partialSize = KVTreeSize(treeMap->trees[i]);
        for (int j = 0; j < partialSize; ) {
            toReturn[toReturnIdx++] = partialValues[j++];
        }
    }
    return toReturn;
}

void freeTreeMap(TreeMap treeMap) {
    for (int i = 0; i < AMOUNT_TREES; i++) {
        freeKVTree(treeMap->trees[i]);
    }
}
