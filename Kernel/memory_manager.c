#include <memory_manager.h>

#define OUT_OF_RANGE_EXCLUSIVE(x,a,b) ((x) < (a) || (x) > (b))

typedef struct {

	bool free;
	void * initial_address;
	uint16_t dim;

} heapblock_t;


// init_mm(0, 10)
// 	[{free, 0, 10}, {inUse, NULL, 0}, ...]
//	mm_malloc(5);
//	[{inUse, 0, 5}, {free, 5, 5}, ...]
//	mm_malloc(2);
//	[{inUse, 0, 5}, {inUse, 5, 2}, {free, 7, 3}, ...]
//	mm_free(0);
//	[{free, 0, 5}, {inUse, 5, 2}, {free, 7, 3}}, ...]
//	mm_malloc(3);
//	[{inUse, 0, 3}, {inUse, 5, 2}, {free, 7, 3}, {free, 10, 2}, ...]


static void * start;
static void * end;
static heapblock_t heap[MAX_HEAP_DIM];

static int size;
static int empty_blocks;

void mm_init(void * start_given, int size_requested)	{
  
	size = (MAX_HEAP_DIM < size_requested) ? MAX_HEAP_DIM : size_requested;

	start = (void *) start_given;
	end = (void *) (start + size_requested - 1);
	size = size_requested;	
	empty_blocks = size_requested - 1;

	heapblock_t genesis_block = {
			
		.free = true,
		.initial_address = start,
		.dim = size
	};

	heapblock_t empty_block = {

		.free = false,
		.initial_address = NULL,
		.dim = 0
	};

	heap[0] = genesis_block;

// Inicializo bloques "sin uso", sirven para crear bloques libres al hacer malloc.

	for(int i=1; i<size; i++)	{

		heap[i] = empty_block;
	}
}



static inline void free_block(int index)	{

	heap[index].free = true;
}

static inline void lock_block(int index)	{

	heap[index].free = false;
}

static inline bool is_free(int index)	{

	return heap[index].free;
}

static inline bool is_locked(int index)	{
	
	return !is_free(index);
}

static inline void * end_address(int index)	{

	return (void *) (heap[index].initial_address + heap[index].dim - 1);
}

static inline int16_t get_address_index(const void * address) {

	uint16_t i = 0;

	while(i<size)	{

		if((address - heap[i].initial_address) < heap[i].dim)	{
			
			return i;
		}
		else	{

			i++;
		}
	}

	return -1;
}

static bool transfer_freespace(int current_index, int dim_requested)	{

	if( heap[current_index].dim == dim_requested)	{
		
		return true;
	}

	for(int j=current_index+1; j<size; j++)	{	

		int freeblock_dim = heap[current_index].dim - dim_requested;

	// Block "sin uso":

		if(is_locked(j) && heap[j].initial_address == NULL)	{ 

			free_block(j);
			heap[j].dim = freeblock_dim;
			heap[j].initial_address = heap[current_index].initial_address + dim_requested;

			return true;
		}

	}	

	return false;
}


void * mm_malloc(size_t dim)	{

	for(int i=0; i<size; i++)	{

		if (is_free(i) && heap[i].dim >= dim)	{	// Encontró un bloque libre

			if(empty_blocks == 0)	{	// Sin bloques vacíos no hace la logica de transferir bloques libres

				lock_block(i);
				return heap[i].initial_address;
			}

		// Primero se "transfieren" los bloques libres que no se van a alocar que estaban en el bloque con espacio

			if(!transfer_freespace(i, dim))	{

				return NULL;
			}
			
			lock_block(i);
			heap[i].dim = dim;
			return heap[i].initial_address;
		}
	}

	return NULL;
}								

void mm_free(void * p)	{

//Checkeo para saber si es una direccion valida:

	if(OUT_OF_RANGE_EXCLUSIVE(p, start, start + size - 1))	{

		return;
	}

	int index = get_address_index(p);

	if( index >= 0 && is_locked(index))	{

		free_block(index);
	}
}

// /* Anda bastante bien, queda pisar la memoria usada y que libere todos los
//  * bloques que se le dió a cada puntero */
// int main()  {

//     char hola[30];

//     mm_init(hola, 30);

//     char * v = (char *) mm_malloc(5);

//     v[0] = 'H';
//     v[1] = 'o';
//     v[2] = 'l';
//     v[3] = 'a';
//     v[4] = '\0';

//     printf("%s\n", v);

//     mm_free(v);
    
//     printf("%s\n",v);
// }
