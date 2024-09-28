#include "KVTree.h"

typedef struct node {
    key_t key;
    value_t value;
    struct node * right;
    struct node * left;
} tNode;

typedef tNode * tTree;

typedef struct KVTreeCDT {
    tTree root;
    int height; /*Altura del árbol*/
    size_t size; /*Cantidad de key en el árbol*/
} KVTreeCDT;

KVTree newKVTree(void) {
    return calloc(1, sizeof(KVTreeCDT));
}

static unsigned int KVTreeSize(tTree tree) {
    if (tree == NULL)
        return 0;
    return 1 + KVTreeSize(tree->right) + KVTreeSize(tree->left);
}

unsigned int size(KVTree bst) {
    return KVTreeSize(bst->root);
}

bool isEmpty(KVTree bst) {
    return size(bst) == 0;
}

static tTree insertRec(tTree tree, key_t key, value_t value, bool * added, int * level) {
    int c;
    if (tree == NULL) {
        tNode * node = malloc(sizeof(tNode));
        node->key = key;
        node->value = value;
        node->right = NULL;
        node->left = NULL;
        if (*level == 0) {
            (*level)++;
        }
        *added = true;
        return node;
    }
    if ((c = compareKeys(key, tree->key)) < 0) {
        if (tree->right == NULL) {
            (*level)++;
        }
        tree->left = insertRec(tree->left, key, value, added, level);
        return tree;
    }
    if (c > 0) {
        if (tree->left == NULL) {
            (*level)++;
        }
        tree->right = insertRec(tree->right, key, value, added, level);
        return tree;
    }
    return tree;
}

bool insert(KVTree bst, key_t key, value_t value) {
    if (containsKey(bst, key)) {
        fprintf(stderr, "key is already mapped to a value");
        exit(1);
    }
    bool added = false;
    bst->root = insertRec(bst->root, key, value, &added, &(bst->height));
    bst->size += added;
    return added;
}

static tTree findMin(tTree tree) {
    while (tree->left != NULL) {
        tree = tree->left;
    }
    return tree;
}

static tTree discardRec(tTree tree, key_t key, bool * removed) {
    if (tree == NULL) {
        return NULL; // No se encontró la clave
    }

    int c = compareKeys(key, tree->key);
    if (c < 0) {
        tree->left = discardRec(tree->left, key, removed);
    } else if (c > 0) {
        tree->right = discardRec(tree->right, key, removed);
    } else {
        *removed = true; // Se encontró el nodo a eliminar
        // Caso 1: nodo sin hijos (hoja)
        if (tree->left == NULL && tree->right == NULL) {
            free(tree);
            return NULL;
        }
        // Caso 2: nodo con un solo hijo
        if (tree->left == NULL) {
            tTree temp = tree->right;
            free(tree);
            return temp;
        } else if (tree->right == NULL) {
            tTree temp = tree->left;
            free(tree);
            return temp;
        }
        // Caso 3: nodo con dos hijos
        tTree temp = findMin(tree->right); // Sucesor en inorden
        tree->key = temp->key; // Copiamos la clave del sucesor
        tree->right = discardRec(tree->right, temp->key, removed); // Eliminamos el sucesor
    }
    return tree;
}

bool discardEntry(KVTree bst, key_t key) {
    bool removed = false;
    bst->root = discardRec(bst->root, key, &removed);
    if (removed) {
        bst->size--; // Si se eliminó, decrementamos el tamaño
    }
    return removed;
}

unsigned int height(KVTree bst) {
    return bst->height;
}

static bool binarySearchKey(tTree tree, key_t key) {
    int c;
    if (tree == NULL) {
        return false;
    }
    if (!(c = compareKeys(key, tree->key))) {
        return true;
    }
    if (c < 0) {
        return binarySearchKey(tree->left, key);
    }
    return binarySearchKey(tree->right, key);
}

static bool searchValue(tTree tree, value_t value) {
    if (tree == NULL) {
        return false;
    }
    if (!compareValues(tree->value, value)) {
        return true;
    }
    return searchValue(tree->right, value) || searchValue(tree->left, value);
}

bool containsKey(KVTree bst, key_t key) {
    return binarySearchKey(bst->root, key);
}

bool containsValue(KVTree bst, value_t value) {
    return searchValue(bst->root, value);
}

bool replaceRec(tTree tree, key_t key, value_t newValue) {
    int c;
    if (tree == NULL) {
        return false;
    }
    if (!(c = compareKeys(key, tree->key))) {
        tree->value = newValue;
        return true;
    }
    if (c < 0) {
        return replaceRec(tree->left, key, newValue);
    }
    return replaceRec(tree->right, key, newValue);
}

bool replace(KVTree bst, key_t key, value_t newValue) {
    return replaceRec(bst->root, key, newValue);
}

static value_t getRec(tTree tree, key_t key) {
    int c;
    if (tree == NULL) {
        return NULL;
    }
    if (!(c = compareKeys(key, tree->key))) {
        return tree->value;
    }
    if (c < 0) {
        return getRec(tree->left, key);
    }
    return getRec(tree->right, key);
}

value_t get(KVTree bst, key_t key) {
    return getRec(bst->root, key);
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

key_t * keySet(KVTree bst) {
    key_t * ans = malloc(bst->size * sizeof(key_t));
    int idx = 0;

    keysToArrayRec(bst->root, ans, &idx);
    return ans;
}

value_t * values(KVTree bst) {
    value_t * ans = malloc(bst->size * sizeof(value_t));
    int idx = 0;

    valuesToArrayRec(bst->root, ans, &idx);
    return ans;
}

static void freeKVTreeRec(tTree tree) {
    if (tree == NULL)
        return;
    freeKVTreeRec(tree->right);
    freeKVTreeRec(tree->left);
    free(tree);
}

void freeKVTree(KVTree bst) {
    freeKVTreeRec(bst->root);
}

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
