#include <stdio.h>

#define BLOCK_SIZE	10
#define BLOCK_AMOUNT  	40				// para el main
#define TOTAL_SPACE	BLOCK_SIZE*BLOCK_AMOUNT		// para el main

#define OUT_OF_RANGE_EXCLUSIVE(x,a,b) ((x) < (a) || (x) > (b))

static void * start;
static void * free_ptrs[TOTAL_SPACE];

static int current; 
static int size;


void mm_init(void * that_start, int that_size)	{

	start = (void *) that_start;
	size = that_size;	
	current = 0;

	for(int i=0; i<size; i++)	{

		free_ptrs[i] = start + BLOCK_SIZE;
	}
}


void * malloc()	{

	if(current < size)	{
	
		return free_ptrs[current++];
	}
	
	else	{

		return NULL;
	}
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

