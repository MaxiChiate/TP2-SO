#include <buddy.h>

typedef struct buddy_block {
	int order; /*Potencia de 2 que representa el tamaño del bloque*/
	bool is_free;
	struct buddy_block * next;
} buddy_block;

// 1024
// 512 256 128 64
// 960 + 64

// Arreglo de punteros a buddy_block_t. Cada posición representa bloques de tamaño específico en potencias de 2.
buddy_block_t free_list[MAX_ORDER + 1];
char heap[HEAP_SIZE]; /*Memoria preasignada para el sistema*/
static int total_assignable_space = HEAP_SIZE;

void init_buddy() {

	for (int i = 0; i <= MAX_ORDER; i++) {
		free_list[i] = NULL;
	}

	// Bloque que abarca toda la memoria
	buddy_block_t initial_block = (buddy_block_t)heap;
	initial_block->order = MAX_ORDER;
	initial_block->is_free = true;
	initial_block->next = NULL;

	free_list[MAX_ORDER] = initial_block;
}

// Dividir un block en dos buddies
static buddy_block_t split_block(buddy_block_t block) {

	int new_order = block->order - 1;
	buddy_block_t buddy = (buddy_block_t)((char *)block + (1 << new_order));

	buddy->order = new_order;
	buddy->is_free = true;
	buddy->next = NULL;

	block->order = new_order;
	block->is_free = true;
	block->next = buddy;

	return buddy;
}

// Encontrar el bloque adecuado en la lista
static buddy_block_t find_suitable_block(int order) {

	for (int i = order; i <= MAX_ORDER; i++) {
		if (free_list[i] != NULL) {
			return free_list[i];
		}
	}
	return NULL;
}

static int ceil_custom(double x) {
    int integer_part = (int)x;
    
    // Si x ya es un número entero, simplemente regresamos la parte entera.
    if (x == (double)integer_part) {
        return integer_part;
    }

    // Si x es positivo y tiene parte decimal, redondeamos al entero siguiente.
    return integer_part + 1;
}


static int get_order(int x) {
    if (x <= 0) {
        return -1; // Indica un error para valores no positivos
    }

    int integer_part = 0;
    double fraction_part = 0.0;

    // Calcular la parte entera del logaritmo
    while (x >= 2.0) {
        x /= 2.0;
        integer_part++;
    }
    while (x < 1.0) {
        x *= 2.0;
        integer_part--;
    }

    // Calcular la parte fraccionaria usando un método de aproximación
    double fraction = 0.5;
    while (x != 1.0 && fraction > 1e-10) {  // Precisión ajustable
        x *= x;
        if (x >= 2.0) {
            fraction_part += fraction;
            x /= 2.0;
        }
        fraction /= 2.0;
    }

    return ceil_custom(integer_part + fraction_part);
}

void * buddy_alloc(int size) {
    int order = get_order(size); /* Ceiling. Redondea para arriba */
    if (order > MAX_ORDER) {
        return NULL;
    }

    if (total_assignable_space < size) {
            return NULL;
    }

    buddy_block_t block = find_suitable_block(order);
    if (block == NULL) {
        return NULL;
    }

    block->is_free = false;

    // Mantener el bloque en free_list
    free_list[block->order] = block->next;

    int current = block->order;
    while (current > order) {
        buddy_block_t buddy = split_block(block);
        free_list[current - 1] = buddy;
        current--;
    }

    total_assignable_space -= (1 << order);

    return (void *)block;
}

void buddy_free(void *ptr) {
    // Verificar si el puntero es NULL
    if (ptr == NULL) {
        return;
    }

    buddy_block_t block = (buddy_block_t)ptr;

    // Verificar si el bloque es un bloque válido
    if (block->is_free) {
        return; // El bloque ya está libre, no se debe liberar de nuevo
    }

    // Marcar el bloque como libre
    block->is_free = true;

    // Calcular el tamaño del bloque
    size_t block_size = 1 << block->order;
    total_assignable_space += block_size;  // Aumentar el espacio total asignable

    int current_order = block->order;

    // Intentar fusionar con su buddy
    while (current_order < MAX_ORDER) {
        size_t buddy_offset = 1 << current_order;
        buddy_block_t buddy = (buddy_block_t)((uintptr_t)block ^ buddy_offset); // Calcular la dirección del buddy

        // Verificar si el buddy está dentro del rango de la memoria asignada
        if (buddy < (buddy_block_t)heap || buddy >= (buddy_block_t)(heap + HEAP_SIZE)) {
            break;  // El buddy está fuera de los límites, no se puede fusionar
        }

        // Verificar si el buddy está libre
        if (buddy->is_free && buddy->order == current_order) {
            // Actualizar la dirección del bloque a la dirección mínima
            if (buddy < block) {
                block = buddy;  // Siempre usar el bloque de menor dirección
            }

            // Fusionar: incrementar el orden
            current_order++;
        } else {
            break;  // No se puede fusionar más, salimos del bucle
        }
    }

    // Actualizar la estructura del bloque resultante
    block->order = current_order;  // Nuevo orden del bloque fusionado
    block->next = free_list[current_order];  // Enlazar el bloque en la lista
    free_list[current_order] = block;  // Añadir a la lista de bloques libres
}