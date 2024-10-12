#include <stdio.h>
#include <memory_manager.h>

#define OUT_OF_RANGE_EXCLUSIVE(x,a,b) ((x) < (a) || (x) > (b))

static void * start;
static void ** free_ptrs;

static int current; 
static int size;


void mm_init(void * start_given, int size_requested)	{

  void * heap_assigned[size_requested];

  free_ptrs = (void **) heap_assigned;
  
	start = (void *) start_given;
	size = size_requested;	
	current = 0;

	for(int i=0; i<size; i++)	{

		free_ptrs[i] = start + i*BLOCK_SIZE;
	}
}


void * mm_malloc(size_t dim)	{

/*    Version donde da mas bloques para la dim que piden:
 *
 *    int new_blocks = 1 + dim / BLOCK_SIZE;

    if(dim == 0 || new_blocks > size - current - 1)  {

      return NULL;
    }

    void * memory_to_return = free_ptrs[current];

    current+=new_blocks;

		return memory_to_return;
    */

    //return (dim > 0 && dim <= BLOCK_SIZE) ? free_ptrs[current++] : NULL;    

    return (current < size && dim != 0) ? free_ptrs[current++] : NULL;
}


void mm_free(void * p)	{

//Checkeo para saber si es una direccion valida:

	if(OUT_OF_RANGE_EXCLUSIVE(p, start, start + size - 1) || current == 0)	{

		return;
	}
	
	void * i = start;

	while(i < ( start + BLOCK_SIZE*size ))	{ 

	//Si es coincide es una direccion valida, entonces lo guardo:

		if(i == p)	{

			free_ptrs[--current] = p;
			return;
		}

		i+=BLOCK_SIZE;		
	}


	//Si salio del while, no era una direccion valida.
}

/* Anda bastante bien, queda pisar la memoria usada y que libere todos los
 * bloques que se le dió a cada puntero
int main()  {

    char hola[30];

    mm_init(hola, 30);

    char * v = (char *) malloc(5);

    v[0] = 'H';
    v[1] = 'o';
    v[2] = 'l';
    v[3] = 'a';
    v[4] = '\0';

    printf("%s\n", v);

    free(v);
    
    printf("%s\n",v);
}*/
