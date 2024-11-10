#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <buffer.h>
#include <bool.h>
#include <buddy_library.h>

//memory_manager.h

#ifndef MM_MANAGER_H
#define MM_MANAGER_H

#define MAX_ORDER 20
#define MIN_CHUNK_ORDER 6
#define BLOCK_SIZE (1 << MIN_CHUNK_ORDER)
#define BLOCKS_HEAP_SIZE (HEAP_SIZE/ BLOCK_SIZE) /*Cantidad de bloques*/
#define HEAP_SIZE (1 << MAX_ORDER) /*2^MAX_ORDER*/
#define MAX_NODES ((1 << (MAX_ORDER - MIN_CHUNK_ORDER + 1)) - 1) /*255*/

typedef enum {B_GREEN, B_YELLOW, B_RED, B_BLUE} block_state;
/*
 * GREEN:  free
 * YELLOW: partially free
 * RED:    alloced
 * BLUE:   left and right are alloced but current isn't
 */

typedef struct mem_struct{
    
    size_t total_space;
    size_t used_space;

} mem_struct;

typedef struct mem_struct * mem_t;

typedef struct buddy_block_descriptor * buddy_block_descriptor_t;

typedef struct buddy_tree * buddy_tree_t;

void mm_init(void * start_given);

void * mm_malloc(size_t size);

void mm_free(void * p);

mem_t mm_mem();

#endif

