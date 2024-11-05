#ifndef BUDDY_H
#define BUDDY_H

#include <stdint.h>
#include <buddy_library.h>
// #include <stddef.h>
#include <defs.h>

#define MAX_ORDER 10
#define MIN_CHUNK_ORDER 3
#define HEAP_SIZE (1 << MAX_ORDER) /*2^MAX_ORDER*/
#define MAX_NODES ((1 << (MAX_ORDER - MIN_CHUNK_ORDER + 1)) - 1) /*255*/

typedef enum {GREEN, YELLOW, RED, BLUE} block_state;

/*
 * GREEN:  free
 * YELLOW: partially free
 * RED:    alloced
 * BLUE:   left and right are alloced but current isn't
 */

typedef struct buddy_block_descriptor * buddy_block_descriptor_t;

typedef struct buddy_tree * buddy_tree_t;

void init_buddy();

void * buddy_alloc(size_t size);

void buddy_free(void * ptr);

void print_rec(buddy_block_descriptor_t block);

void print_buddy();

size_t get_total_assignable_space();


#endif //BUDDY_H