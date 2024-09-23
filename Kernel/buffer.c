
#include <buffer.h>

#define TRUE 1
#define FALSE 0

static char buffer [BUFFER_SIZE];
static int next = BUFFER_FIRSTPOS;


void cleanBuffer()  {
    next = BUFFER_FIRSTPOS;
}
int bufferIsEmpty() {
    return next == BUFFER_FIRSTPOS;
}

int bufferIsFull()  {
    return next == BUFFER_SIZE-1;
}

void putChar(char c)   {
    
    if(bufferIsFull())
        cleanBuffer();

    buffer[next++] = c;
}

int putString(const  char * s)    {
    cleanBuffer();
    int i=0;
	while(s[i]!=0)    {
        if(bufferIsFull())  {
            return -1;
        }   
        putChar(s[i++]);
    }
// No contempla el cero al final del string.
    return i;
}

char readChar() {
    return bufferIsEmpty()? 0 : buffer[--next];
}
void peekAllBuffer(char * string, int dim) {

    int i = BUFFER_FIRSTPOS;
    for(; i<dim-1 && i<next; ++i)
        string[i] = buffer[i];

    string[i] = '\0';
}


void readBuffer(char * string, int dim)    {
    
    peekAllBuffer(string, dim);
    cleanBuffer();

}





