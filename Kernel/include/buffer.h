#ifndef _BUFFER_H
#define _BUFFER_H
    
// Este TAD no se va a usar mucho, pero será una buena referencia 
// para usar en el userspace

#define BUFFER_SIZE 256
#define BUFFER_FIRSTPOS 0

// El ultimo caracter es siempre '\0'.
    
    int bufferIsEmpty();

    int bufferIsFull();

// Ingresa un caracter en el Buffer.
// Si el buffer estaba lleno, vuelve al principio pisando lo anterior.
    void putChar(char c);

// Ingresa un string en el Buffer.
// Retorna -1 si no había suficiente espacio, si esto sucede el buffer queda como estaba antes de llamar a putString()
// Si pudo ingresarlo correctamente retorna la longitud de @s, sin contar el cero del final.
    int putString(const char * s);
    
// Obtiene el ultimo caracter, borrandolo del Buffer.
// Si el buffer esta vacío devuelve el caracter nulo: '\0'.
    char readChar();

// Copia todo el buffer en @string, dejandolo vacio.
    void readBuffer(char * string, int dim);

// Copia todo el buffer en @string.
    void peekAllBuffer(char * string, int dim);

// Limpia el Buffer.
    void cleanBuffer();

#endif