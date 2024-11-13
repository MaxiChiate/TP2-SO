#ifndef _USERLIB_H
#define _USERLIB_H

    #include <defs.h>
    #include <bool.h>
    #include <stdint.h>
    #include <../../../Kernel/include/systemCallDefs.h>

    int64_t _int80(int id, int64_t * args);

    void print(char * str);

    void print2(char * str, unsigned int dim);

    void puts(char * str);

    void print_ps(ps_t * ps);

    void print_all_ps(ps_t ** to_print);

    void printUinteger(unsigned int n);

    void printUlong(unsigned long n);

    int getChar();

    void putChar(char c);

    char getAndPrintChar();

    void clear();

    void putEnter();

    void putnEnters(unsigned int n);

    void putBackSpace();

    void putTab();

    void time_getter();

    extern void invalidOpcode();

    void sleep(int64_t ms);


    void sleep(int64_t ms);


//  Strings:

    unsigned int strLength( char * str);

    unsigned int strEquals( char * s1,  char * s2);

    unsigned int strToUint( char * s);

    bool isDigit(unsigned char c);

    bool isUpper(unsigned char c);

    bool isVocal(unsigned char c);

    int getUintDigits(unsigned int n );

    int getUlongDigits(unsigned long n);

    char* stringNormalizer(char * origString,unsigned int strlen);

    int stringTrimmerBySpace( char * origString, char * function_name, char ** argv, unsigned int max_dim);

    void strcopy(char* dest, char* string);

    int atoi(char* value);

    void itoa(int value, char* buff);

    int abs(int value);


//  Al no tener memoria dinamica uIntToString depende de que se le pase la cantidad de digitos
//  del numero a transformar. Para eso se dispone de la funcion getUintDigits.

//  Ejemplo de invocacion:

//      int n = 22;
//      int digits = getUintDigits(n);
//      char s[digits+1];
//      uIntToString(n, s, digits);
//      print(s);

//  Se deberia imprimir en pantalla: 22

    void uIntToString(unsigned int input, char * ans, int digits);

    void uLongToString(unsigned long input, char * ans, int digits);

//  Math:

    unsigned long pow(unsigned long x, unsigned long n);

    void * memset(void * destiny, int32_t c, uint64_t length);


#endif //_USERLIB_H
