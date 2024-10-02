#include <bst.h>

typedef struct node {
    elemType value;
    struct node * right;
    struct node * left;
} tNode;

typedef tNode * tTree;

typedef struct bstCDT {
    tTree root;
    int height; /*Altura del árbol*/
    size_t size; /*Cantidad de elementos en el árbol*/
    int (*compare)(elemType, elemType);
} bstCDT;


bstADT newBst(int(*compare)(elemType, elemType)) {
    bstADT bst = calloc(1, sizeof(bstCDT));
    bst->height = -1;
    bst->size = 0;
    bst->compare = compare;
    return bst;
}

unsigned int size(bstADT bst) {
    return bst->size;
}

static tTree insertRec(tTree tree, elemType elem, bool * added, int * level, int (*compare)(elemType, elemType)) {
    int c;
    if (tree == NULL) {
        tNode * node = malloc(sizeof(tNode));
        node->value = elem;
        node->right = NULL;
        node->left = NULL;
        (*level)++;
        *added = true;
        return node;
    }
    if ((c = compare(elem, tree->value)) < 0) {
        (*level)++;
        tree->left = insertRec(tree->left, elem, added, level, compare);
        return tree;
    }
    if (c > 0) {
        (*level)++;
        tree->right = insertRec(tree->right, elem, added, level, compare);
        return tree;
    }
    return tree;
}

bool insert(bstADT bst, elemType elem) {

    if(height(bst) != -1 && sizeof(elem) != sizeof(bst->root))  {

        return false;
    }

    bool added = false;
    int level = -1;
    bst->root = insertRec(bst->root, elem, &added, &level, bst->compare);
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

static tTree discardRec(tTree tree, elemType elem, bool *removed, int *newHeight, int(*compare)(elemType, elemType)) {
    if (tree == NULL) {
        *newHeight = 0;
        return NULL;
    }

    int leftHeight = 0, rightHeight = 0;
    int c = compare(elem, tree->value);

    if (c < 0) {
        tree->left = discardRec(tree->left, elem, removed, &leftHeight, compare);
    } else if (c > 0) {
        tree->right = discardRec(tree->right, elem, removed, &rightHeight, compare);
    } else {
        *removed = true;

        // Caso 1: nodo sin hijos
        if (tree->left == NULL && tree->right == NULL) {
            free(tree);
            *newHeight = 0;
            return NULL;
        }

        // Caso 2: un hijo
        if (tree->left == NULL) {
            tTree temp = tree->right;
            free(tree);
            *newHeight = rightHeight + 1;
            return temp;
        } else if (tree->right == NULL) {
            tTree temp = tree->left;
            free(tree);
            *newHeight = leftHeight + 1;
            return temp;
        }

        // Caso 3: dos hijos
        tTree temp = findMin(tree->right); // Sucesor inorder
        tree->value = temp->value;
        tree->right = discardRec(tree->right, temp->value, removed, &rightHeight, compare);
    }

    // Calcular la nueva altura del nodo
    *newHeight = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    return tree;
}

bool discardEntry(bstADT bst, elemType elem) {

    if(bst != NULL && sizeof(elem) != sizeof(bst->root))  {

        return false;
    }

    bool removed = false;
    int newHeight = 0;
    bst->root = discardRec(bst->root, elem, &removed, &newHeight, bst->compare);

    if (removed) {
        bst->size--;
        bst->height = newHeight; // Actualizamos la altura
    }

    return removed;
}


unsigned int height(bstADT bst) {
    return bst->height;
}

static bool binarySearch(tTree tree, elemType elem, int (*compare)(elemType, elemType)) {
    int c;
    if (tree == NULL) {
        return 0;
    }
    if (!(c = compare(elem, tree->value))) {
        return 1;
    }
    if (c < 0) {
        return binarySearch(tree->left, elem, compare);
    }
    return binarySearch(tree->right, elem, compare);
}

bool belongs(bstADT bst, elemType elem) {
    return binarySearch(bst->root, elem, bst->compare);
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

