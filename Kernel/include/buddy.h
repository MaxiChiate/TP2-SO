#ifndef BUDDY_H
#define BUDDY_H

#include <stdint.h>
#include <bool.h>
#include <stddef.h>

#define MAX_ORDER 10
#define HEAP_SIZE (1 << MAX_ORDER) /*2^MAX_ORDER*/

typedef struct buddy_block * buddy_block_t;

void init_buddy();

void * buddy_alloc(int size);

void buddy_free(void * ptr);

void print_rec(buddy_block_t block);

void print_free_list();

#endif //BUDDY_H
