#include <memory_manager.h>

#define OUT_OF_RANGE_EXCLUSIVE(x,a,b) ((x) < (a) || (x) > (b))

#define IN_RANGE(x, y, z) (x >= y && x < z)

typedef struct {

	bool free;
	void * initial_address;
	int id;
	int link;

} blocks_heapblock_t;


static char * heap;
static mem_struct mem;

static mem_t mem_data;

// index del último bloque reservado
static int last_alloced;

static blocks_heapblock_t blocks_heap[BLOCKS_HEAP_SIZE];

static blocks_heapblock_t create_blocks_heapblock(void * initial_address, int id, int link) {
    
    blocks_heapblock_t blocks_heapblock = {
        .free = true,
        .initial_address = initial_address,
        .id = id,
        .link = -1
    };

    return blocks_heapblock;
}


void mm_init(void * start_given) {
    
    last_alloced = 0;
    mem_data = &mem;
    mem_data->total_space= HEAP_SIZE;
    mem_data->used_space = 0;

	heap = (char *) start_given;

// Inicializo bloques "sin uso", sirven para crear bloques libres al hacer malloc.
	for(int i = 0; i < BLOCKS_HEAP_SIZE; i++)	{
		blocks_heap[i] = create_blocks_heapblock(heap + i * BLOCK_SIZE, i, -1);
	}
}

static inline void free_block(int index)	{

	blocks_heap[index].free = true;
    blocks_heap[index].link = -1;
    mem_data->used_space -= BLOCK_SIZE;
}

static inline void lock_block(int index,int link_index)	{

	blocks_heap[index].link = link_index;
    blocks_heap[index].free=false;
    mem_data->used_space += BLOCK_SIZE;
}

static inline bool is_free(int index)	{

	return blocks_heap[index].free;
}

static inline bool is_locked(int index)	{
	
	return !is_free(index);
}

static inline void * end_address(int index)	{

	return (void *) (blocks_heap[index].initial_address + blocks_heap[index].id * BLOCKS_HEAP_SIZE);
}

static size_t find_blocks_to_alloc(size_t dim) {
    
    size_t to_return = dim/BLOCK_SIZE;

    if(dim%BLOCK_SIZE!=0){
        to_return+=1;
    }

    return to_return;
}

// Retorna la posición donde se comenzará a reservar el nuevo bloque
static int find_block_space(int blocks){

    int free_streak= 0;
    
    for(int i=last_alloced; i < BLOCKS_HEAP_SIZE;i++){

        if(blocks_heap[i].free){
            free_streak+=1;
            if(free_streak>=blocks){
                return i-blocks+1;
            }
        }
        else{
            free_streak=0;
        }
    }
    free_streak=0;
    for(int i=0; i<last_alloced; i++){
        
        if(blocks_heap[i].free){
            free_streak+=1;
            if(free_streak>=blocks){
                return i-blocks+1;
            }
        }
        else{
            free_streak=0;
        }
    }

    return -1;

}


void * mm_malloc(size_t dim)	{

    size_t blocks_to_alloc = find_blocks_to_alloc(dim);
    // Si no me quedan bloques retorno NULL
    int to_alloc=find_block_space(blocks_to_alloc);

    if (to_alloc==-1) {
        return NULL;
    }

    void * to_return = blocks_heap[to_alloc].initial_address;
    for (int i = to_alloc; i < to_alloc + blocks_to_alloc; i++) {
        
        lock_block(i,to_alloc);
    }

    last_alloced = to_alloc + blocks_to_alloc;
    return to_return;

}	

static int get_index(void * ptr) {

    for (int i = 0; i < BLOCKS_HEAP_SIZE; i++) {

        if (IN_RANGE(ptr, blocks_heap[i].initial_address, blocks_heap[i].initial_address + BLOCK_SIZE)) {
            return i;
        }

    }

    return -1;
}

void mm_free(void * p)	{

    //Si no es una dirección válida retorno

	if(!IN_RANGE(p, (void *)heap, (void *)heap + HEAP_SIZE))	{
		return;
	}
	int index = get_index(p);
    if (index == -1) return;

    int i = index;
    while (i < BLOCKS_HEAP_SIZE && blocks_heap[i].link == blocks_heap[index].id) {
        free_block(i++);
    }


}

mem_t mm_mem(){
    return mem_data;
}