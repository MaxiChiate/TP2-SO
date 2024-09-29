#include "bst.h"

typedef struct node {
    elemType value;
    struct node * right;
    struct node * left;
} TNode;

typedef TNode * tTree;

typedef struct bstCDT {
    tTree root;
    int height; /*Altura del árbol*/
    size_t size; /*Cantidad de elementos en el árbol*/
} bstCDT;

/*
 * Retora 0 si e1 es igual a e2, negativo si e1 es menor que e2 o positivo si e1 es mayor que e2
 */
static int compare(elemType e1, elemType e2) {
    return e1 - e2;
}


bstADT newBst(void) {
    bstADT bst = calloc(1, sizeof(bstCDT));
    bst->height = -1;
    bst->size = 0;
    return bst;
}

static unsigned int bstSize(tTree tree) {
    if (tree == NULL)
        return 0;
    return 1 + bstSize(tree->right) + bstSize(tree->left);
}

unsigned int size(bstADT bst) {
    return bstSize(bst->root);
}

// static int calculateHeight(tTree tree) {
//     if (tree == NULL) {
//         return -1;
//     }
//     int left = calculateHeight(tree->left);
//     int right = calculateHeight(tree->right);

//     return left > right ? left : right + 1;
// }

static tTree insertRec(tTree tree, elemType elem, int * added, int * level) {
    int c;
    if (tree == NULL) {
        TNode * node = malloc(sizeof(TNode));
        node->value = elem;
        node->right = NULL;
        node->left = NULL;
        *added = 1;
        return node;
    }
    if ((c = compare(elem, tree->value)) < 0) {
        tree->left = insertRec(tree->left, elem, added, ++level);
        return tree;
    }
    if (c > 0) {
        tree->right = insertRec(tree->right, elem, added, ++level);
        return tree;
    }
    return tree;
}

unsigned int insert(bstADT bst, elemType elem) {
    int added = 0;
    int level = 0;
    bst->root = insertRec(bst->root, elem, &added, &level);
    if (level > bst->height) {
        bst->height = level;
    }
    bst->size += added;
    return added;
}

static tTree findMin(tTree tree) {
    while (tree->left != NULL) {
        tree = tree->left;
    }
    return tree;
}

static tTree discardRec(tTree tree, elemType elem, bool * removed, int * level) {
    if (tree == NULL) {
        return NULL; // No se encontró la clave
    }

    int c = compare(elem, tree->value);
    if (c < 0) {
        tree->left = discardRec(tree->left, elem, removed, ++level);
    } else if (c > 0) {
        tree->right = discardRec(tree->right, elem, removed, ++level);
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
        tTree temp = findMin(tree->right); // Sucesor inorder
        tree->value = temp->value; // Copio la clave del sucesor
        tree->right = discardRec(tree->right, temp->value, removed, ++level); // Eliminamos el sucesor
    }
    return tree;
}

bool discardEntry(bstADT bst, elemType elem) {
    bool removed = false;
    int level = 0;
    bst->root = discardRec(bst->root, elem, &removed, &level);
    // bst->height = calculateHeight(bst->root);
    bst->size -= removed;
    return removed;
}

unsigned int height(bstADT bst) {
    return bst->height;
}

static bool binarySearch(tTree tree, elemType elem) {
    int c;
    if (tree == NULL) {
        return 0;
    }
    if (!(c = compare(elem, tree->value))) {
        return 1;
    }
    if (c < 0) {
        return binarySearch(tree->left, elem);
    }
    return binarySearch(tree->right, elem);
}

unsigned int belongs(bstADT bst, elemType elem) {
    return binarySearch(bst->root, elem);
}

static void treeToArrayRec(tTree tree, elemType * v, int * i) {
    if (tree == NULL)
        return;
    treeToArrayRec(tree->left, v, i);
    v[(*i)++] = tree->value;
    treeToArrayRec(tree->right, v, i);
}

elemType * inOrder(bstADT bst) {
    elemType * ans = malloc(bst->size * sizeof(elemType));
    int idx = 0;

    treeToArrayRec(bst->root, ans, &idx);
    return ans;
}

static void freeBstRec(tTree tree) {
    if (tree == NULL)
        return;
    freeBstRec(tree->right);
    freeBstRec(tree->left);
    free(tree);
}

void freeBst(bstADT bst) {
    freeBstRec(bst->root);
}

static void printBstRec(tTree tree) {
    if (tree == NULL || tree->right == NULL || tree->left == NULL)
        return;
    printf("Root: %5d\n", tree->value);
    printf("Left: %d\t", tree->left->value);
    printf("Right: %d\n", tree->right->value);
    printBstRec(tree->left);
    printBstRec(tree->right);
}

void printBst(bstADT bst) {
    printBstRec(bst->root);
}

int main() {
    bstADT tree = newBst();

    printf("Height: %d\n", height(tree));

    insert(tree, 5);
    insert(tree, 4);
    insert(tree, 6);
    insert(tree, 8);
    insert(tree, 10);

    //           5
    //      4        6
    //                   8
    //                     10

    printf("Height: %d\n", height(tree));

    // discardEntry(tree, 5);
    // discardEntry(tree, 4);
    // discardEntry(tree, 6);
    // discardEntry(tree, 8);
    // discardEntry(tree, 10);

    // printf("Height: %d\n", height(tree));

    freeBst(tree);

    return 0;
}
