//  randlib.c

#include <randlib.h>

int randInt(unsigned int izq, unsigned int der)    {

    if ( izq > der) {
        int aux = izq;
        izq = der;
        der = aux;
    }
    
    return rand() % (der - izq + 1)  + izq;

}

unsigned long rand()  {

    return (int) _int80(SYS_RAND, NULL);
}