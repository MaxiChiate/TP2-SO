#include <collections/KVTree.h>

typedef struct KVnode {
    key_t key;
    value_t value;
    struct KVnode * right;
    struct KVnode * left;
} tNode;

typedef tNode * tTree;

typedef struct KVTreeCDT {
    tTree root;
    int height; /*Altura del árbol*/
    uint32_t size; /*Cantidad de key en el árbol*/
    int (*compareKeys)(key_t, key_t);
    int (*compareValues)(value_t, value_t);
} KVTreeCDT;

KVTree newKVTree(int(*compareKeys)(key_t, key_t), int (*compareValues)(value_t, value_t)) {
    KVTree kvTree = mm_malloc(sizeof(KVTreeCDT));
    kvTree->height = -1;
    kvTree->size = 0;
    kvTree->compareKeys = compareKeys;
    kvTree->compareValues = compareValues;
    return kvTree;
}

unsigned int KVTreeSize(KVTree kvTree) {
    return kvTree->size;
}

bool isEmpty(KVTree kvTree) {
    return KVTreeSize(kvTree) == 0;
}

static tTree insertRec(tTree tree, key_t key, value_t value, bool * added, int * level, int(*compareKeys)(key_t, key_t)) {
    int c;
    if (tree == NULL) {
        tNode * node = mm_malloc(sizeof(tNode));
        node->key = key;
        node->value = value;
        node->right = NULL;
        node->left = NULL;
        (*level)++;
        *added = true;
        return node;
    }
    if ((c = compareKeys(key, tree->key)) < 0) {
        (*level)++;
        tree->left = insertRec(tree->left, key, value, added, level, compareKeys);
        return tree;
    }
    if (c > 0) {
        (*level)++;
        tree->right = insertRec(tree->right, key, value, added, level, compareKeys);
        return tree;
    }
    return tree;
}

bool insert(KVTree kvTree, key_t key, value_t value) {
    bool added = false;
    int level = -1;
    kvTree->root = insertRec(kvTree->root, key, value, &added, &level, kvTree->compareKeys);
    if (level > kvTree->height) {
        kvTree->height = level;
    }
    kvTree->size += added;
    return added;
}

static tTree findMin(tTree tree) {
    while (tree->left != NULL) {
        tree = tree->left;
    }
    return tree;
}

static tTree discardRec(tTree tree, key_t key, bool * removed, int * newHeight, int(*compareKeys)(key_t, key_t)) {
    if (tree == NULL) {
        *newHeight = 0;
        return NULL; // No se encontró la clave
    }

    int leftHeight = 0, rightHeight = 0;
    int c = compareKeys(key, tree->key);

    if (c < 0) {
        tree->left = discardRec(tree->left, key, removed, &leftHeight, compareKeys);
    } else if (c > 0) {
        tree->right = discardRec(tree->right, key, removed, &rightHeight, compareKeys);
    } else {
        *removed = true; // Se encontró el nodo a eliminar
        // Caso 1: nodo sin hijos (hoja)
        if (tree->left == NULL && tree->right == NULL) {
            mm_free(tree);
            *newHeight = 0;
            return NULL;
        }
        // Caso 2: nodo con un solo hijo
        if (tree->left == NULL) {
            tTree temp = tree->right;
            mm_free(tree);
            *newHeight = rightHeight + 1;
            return temp;
        } else if (tree->right == NULL) {
            tTree temp = tree->left;
            mm_free(tree);
            *newHeight = leftHeight + 1;
            return temp;
        }
        // Caso 3: nodo con dos hijos
        tTree temp = findMin(tree->right); // Sucesor en inorden
        tree->key = temp->key; // Copiamos la clave del sucesor
        tree->right = discardRec(tree->right, temp->key, removed, &rightHeight, compareKeys); // Eliminamos el sucesor
    }
    *newHeight = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    return tree;
}

bool discardEntry(KVTree kvTree, key_t key) {
    bool removed = false;
    int newHeight = 0;
    kvTree->root = discardRec(kvTree->root, key, &removed, &newHeight, kvTree->compareKeys);
    if (removed) {
        kvTree->size--;
        kvTree->height = newHeight;
    }
    return removed;
}

unsigned int height(KVTree kvTree) {
    return kvTree->height;
}

static bool binarySearchKey(tTree tree, key_t key, int(*compareKeys)(key_t, key_t)) {
    int c;
    if (tree == NULL) {
        return false;
    }
    if (!(c = compareKeys(key, tree->key))) {
        return true;
    }
    if (c < 0) {
        return binarySearchKey(tree->left, key, compareKeys);
    }
    return binarySearchKey(tree->right, key, compareKeys);
}

static bool searchValue(tTree tree, value_t value, int(*compareValues)(value_t, value_t)) {
    if (tree == NULL) {
        return false;
    }
    if (!compareValues(tree->value, value)) {
        return true;
    }
    return searchValue(tree->right, value, compareValues) || searchValue(tree->left, value, compareValues);
}

bool containsKey(KVTree kvTree, key_t key) {
    return binarySearchKey(kvTree->root, key, kvTree->compareKeys);
}

bool containsValue(KVTree kvTree, value_t value) {
    return searchValue(kvTree->root, value, kvTree->compareValues);
}

bool replaceRec(tTree tree, key_t key, value_t newValue, int(*compareKeys)(key_t, key_t)) {
    int c;
    if (tree == NULL) {
        return false;
    }
    if (!(c = compareKeys(key, tree->key))) {
        tree->value = newValue;
        return true;
    }
    if (c < 0) {
        return replaceRec(tree->left, key, newValue, compareKeys);
    }
    return replaceRec(tree->right, key, newValue, compareKeys);
}

bool replace(KVTree kvTree, key_t key, value_t newValue) {
    return replaceRec(kvTree->root, key, newValue, kvTree->compareKeys);
}

static value_t getRec(tTree tree, key_t key, int(*compareKeys)(key_t, key_t)) {
    int c;
    if (tree == NULL) {
        return NULL;
    }
    if (!(c = compareKeys(key, tree->key))) {
        return tree->value;
    }
    if (c < 0) {
        return getRec(tree->left, key, compareKeys);
    }
    return getRec(tree->right, key, compareKeys);
}

value_t get(KVTree kvTree, key_t key) {
    return getRec(kvTree->root, key, kvTree->compareKeys);
}

static void keysToArrayRec(tTree tree, key_t * v, int * i) {
    if (tree == NULL)
        return;
    keysToArrayRec(tree->left, v, i);
    v[(*i)++] = tree->key;
    keysToArrayRec(tree->right, v, i);
}

static void valuesToArrayRec(tTree tree, value_t * v, int * i) {
    if (tree == NULL)
        return;
    valuesToArrayRec(tree->left, v, i);
    v[(*i)++] = tree->value;
    valuesToArrayRec(tree->right, v, i);
}

key_t * keySet(KVTree kvTree) {
    key_t * ans = mm_malloc(kvTree->size * sizeof(key_t));
    int idx = 0;

    keysToArrayRec(kvTree->root, ans, &idx);
    return ans;
}

value_t * values(KVTree kvTree) {
    value_t * ans = mm_malloc(kvTree->size * sizeof(value_t));
    int idx = 0;

    valuesToArrayRec(kvTree->root, ans, &idx);
    return ans;
}

static void freeKVTreeRec(tTree tree) {
    if (tree == NULL)
        return;
    freeKVTreeRec(tree->right);
    freeKVTreeRec(tree->left);
    mm_free(tree);
}

void freeKVTree(KVTree bst) {
    freeKVTreeRec(bst->root);
}

/*
static void printKVTreeRec(tTree tree) {
    if (tree == NULL || tree->right == NULL || tree->left == NULL)
        return;
    printf("Root: %5d\n", tree->key);
    printf("Left: %d\t", tree->left->key);
    printf("Right: %d\n", tree->right->key);
    printKVTreeRec(tree->left);
    printKVTreeRec(tree->right);
}

void printKVTree(KVTree bst) {
    printKVTreeRec(bst->root);
}
*/