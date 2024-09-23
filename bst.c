#include "bstADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    return calloc(1, sizeof(bstCDT));
}

static unsigned int bstSize(tTree tree) {
    if (tree == NULL)
        return 0;
    return 1 + bstSize(tree->right) + bstSize(tree->left);
}

unsigned int size(bstADT bst) {
    return bstSize(bst->root);
}

static tTree insertRec(tTree tree, elemType elem, int * added, int * level) {
    int c;
    if (tree == NULL) {
        TNode * node = malloc(sizeof(TNode));
        node->value = elem;
        node->right = NULL;
        node->left = NULL;
        if (*level == 0) {
            (*level)++;
        }
        *added = 1;
        return node;
    }
    if ((c = compare(elem, tree->value)) < 0) {
        if (tree->right == NULL) {
            (*level)++;
        }
        tree->left = insertRec(tree->left, elem, added, level);
        return tree;
    }
    if (c > 0) {
        if (tree->left == NULL) {
            (*level)++;
        }
        tree->right = insertRec(tree->right, elem, added, level);
        return tree;
    }
    return tree;
}

unsigned int insert(bstADT bst, elemType elem) {
    int added = 0;
    bst->root = insertRec(bst->root, elem, &added, &(bst->height));
    bst->size += added;
    return added;
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
