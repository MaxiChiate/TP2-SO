#include <stdio.h>

#define BLOCK_SIZE	10

#define MAX_SIZE BLOCK_SIZE*M

#define OUT_OF_RANGE_EXCLUSIVE(x,a,b) ((x) < (a) || (x) > (b))

static void * start;
static void ** free_ptrs;

static int current; 
static int size;


void mm_init(void * that_start, int that_size)	{

  void * heap_assigned[that_size];

  free_ptrs = (void **) heap_assigned;
  
	start = (void *) that_start;
	size = that_size;	
	current = 0;

	for(int i=0; i<size; i++)	{

		free_ptrs[i] = start + BLOCK_SIZE;
	}
}


void * malloc(size_t dim)	{

    int new_blocks = 1 + dim / BLOCK_SIZE;

    if(dim == 0 || new_blocks > size - current - 1)  {

      return NULL;
    }

    void * memory_to_return = free_ptrs[current];

    current+=new_blocks;

		return memory_to_return;
}


void free(void * p)	{

//Checkeo para saber si es una direccion valida:

	if(OUT_OF_RANGE_EXCLUSIVE(p, start, start + size - 1))	{

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
