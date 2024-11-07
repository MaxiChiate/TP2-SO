#include <memory_manager.h>

typedef struct buddy_block_descriptor {
	int order; /*Potencia de 2 que representa el tamaño del bloque*/
    block_state state;
	struct buddy_block_descriptor * left;
    struct buddy_block_descriptor * right;
    void * chunk;
} buddy_block_descriptor;

typedef struct buddy_tree {
    buddy_block_descriptor_t head;
    size_t total_assignable_space;
    size_t descriptor_counter; /*Índice de la lista. Solo sirve para inicializar*/
} buddy_tree;

#define IN_RANGE(x) (x >= tree->head->chunk && x < tree->head->chunk + (1 << tree->head->order))

// Memoria preasignada para el sistema
static char heap[HEAP_SIZE];

// Memoria preasignada para los bloques
static buddy_block_descriptor blocks_heap[MAX_NODES];

static buddy_tree tree_struct;
static buddy_tree_t tree = &tree_struct;

static void init_block(buddy_block_descriptor_t block, int order){

    block->order = order;
    block->state = B_GREEN;
    block->left = NULL;
    block->right = NULL;

}



static buddy_block_descriptor_t create_new_block_descriptor(int order) {
    
    buddy_block_descriptor_t new_block_descriptor = &blocks_heap[tree->descriptor_counter++];
    
    init_block(new_block_descriptor, order);

    return new_block_descriptor;

}

//Hace un arbol del orden mínimo solicitado, se llamará en el init_buddy para crear el arbol de descriptores
static void split_block(buddy_block_descriptor_t block, int order) {

    if (block->order <= order) {
        return;
    }

    buddy_block_descriptor_t left_block =  create_new_block_descriptor(block->order - 1);
    buddy_block_descriptor_t right_block = create_new_block_descriptor(block->order - 1);

    left_block->chunk = (void *)block->chunk;
    right_block->chunk = (void *)block->chunk + ((1 << (block->order - 1)));

    block->left = left_block;
    block->right = right_block;

    split_block(left_block, order);
    split_block(right_block, order);

}

static buddy_block_descriptor_t find_rec(buddy_block_descriptor_t current, int order) {

    buddy_block_descriptor_t to_return = NULL;

    // Si es null retorna null
    if (current == NULL) {
        return NULL;
    }
    // Si este bloque esta libre y es del orden indicado se retorna este bloque
    if (current->order == order && current->state == B_GREEN) {
        current->state = B_RED;
        return current;
    }

    // Si left es GREEN llamo recursivamente con left
    if (current->left != NULL && current->left->state == B_GREEN) {
        to_return = find_rec(current->left, order);
    }

    // Si left es YELLOW
    else if (current->left != NULL && current->left->state == B_YELLOW) {

        // Si left es del orden solicitado llamo recursivamente con right 
        if (current->left->order == order && current->right != NULL) {
            to_return = find_rec(current->right, order);

        }

        // Si left->order > order llamo recursivamente con left
        else if (current->left->order > order) {
            to_return = find_rec(current->left, order);

            // Si no pudo en el sub arbol left, va por el sub arbol right
            if (to_return == NULL) {
                to_return = find_rec(current->right, order);
            }
        }
    }

    // Si left es RED o BLUE llamo recursivamente con right
    else if (current->left != NULL && (current->left->state == B_RED || current->left->state == B_BLUE)) {
        to_return = find_rec(current->right, order);
    }

    // Si no es una hoja    
    if(current->left!=NULL && current->right!=NULL){
        // Pone en amarillo todo el recorrido
        current->state = B_YELLOW;
        //Si ambos hijos son RED, el current se pone en BLUE
        if (current->left->state == B_RED && current->right->state == B_RED) {
            current->state = B_BLUE;
        }
        else if (current->left->state == B_BLUE && current->right->state == B_BLUE) {
            current->state = B_BLUE;
        }
    }
    
    return to_return;

}

// Encontrar el bloque adecuado en la lista
static buddy_block_descriptor_t find_suitable_block(int order) {

    return find_rec(tree->head, order);

}

void * buddy_alloc(size_t size) {

    int order = get_order(size);
    if (order > MAX_ORDER) {
        return NULL;
    }

    if (tree->total_assignable_space < size) {
        return NULL;
    }

    buddy_block_descriptor_t block = find_suitable_block(order);
    if (block == NULL) {
        return NULL;
    }

    tree->total_assignable_space -= (1 << order);

    return block->chunk;
}

static void free_rec(buddy_block_descriptor_t block, void * ptr) {

    if(block == NULL){
        return;
    }

    //Si el bloque encontrado no esta libre, se libera y se pone en GREEN
    if (block->state == B_RED) {
        block->state = B_GREEN;
        tree->total_assignable_space += (1 << block->order);
        return;
    }

    //Si el puntero es menor a la mitad del bloque, va al subarbol izquierdo
    if (ptr < (block->chunk + (1 << (block->order - 1)))) {
        free_rec(block->left, ptr);
    } 
    //Si el puntero es mayor a la mitad del bloque, va al subarbol derecho
    else {
        free_rec(block->right, ptr);
    }

    //Si ambos hijos son GREEN, el current se pone en GREEN
    if (block->left!= NULL && block->left->state == B_GREEN && block->right->state == B_GREEN) {
        block->state = B_GREEN;
    }
    //Si los dos hijos no son GREEN, se pone en YELLOW
    else if(block->left!=NULL){
        block->state = B_YELLOW;
    }
}

void buddy_free(void *ptr) {

    if (!IN_RANGE(ptr)) {
        return;
    }

    free_rec(tree->head, ptr);

    return;

}

//Aloca el head en blocks_heap y llama recursivamente a split_block
void init_buddy() {

    tree->descriptor_counter = 0;
    tree->total_assignable_space = HEAP_SIZE;
    tree->head = create_new_block_descriptor(MAX_ORDER);
	tree->head->chunk = (void *) heap;
    split_block(tree->head, MIN_CHUNK_ORDER);

}

size_t get_total_assignable_space(){
    return tree->total_assignable_space;
}