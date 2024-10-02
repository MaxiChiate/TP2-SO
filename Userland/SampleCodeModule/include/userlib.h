#ifndef _USERLIB_H
#define _USERLIB_H

    #include <defs.h>

    void print(char * str);

    void printUinteger(unsigned int n);

    void printUlong(unsigned long n);

    unsigned char getChar();

    void putChar(char c);

    char getAndPrintChar();

    void draw(int model, int x, int y, char color);

    void drawBLackSquare(unsigned int x, unsigned int y);
    void drawGreenSquare(unsigned int x, unsigned int y);
    void drawRedSquare(unsigned int x, unsigned int y);
    void drawYellowSquare(unsigned int x, unsigned int y);
    void drawWhiteSquare(unsigned int x, unsigned int y);

    void clear();

    void exit_shell();

    void putEnter();

    void putnEnters(unsigned int n);

    void putBackSpace();

    void putTab();

    void time_getter();

    void make_text_smaller();

    void make_text_bigger();

    void recolor(unsigned int colour);

    void play_song(int name);

    extern void invalidOpcode();

//  Strings:

    unsigned int strLength(const char * str);

    unsigned int strEquals(char * s1, char * s2);

    unsigned int strToUint(char * s);

    int isDigit(unsigned char c);

    int isUpper(unsigned char c);

    int getUintDigits(unsigned int n );

    int getUlongDigits(unsigned long n);

    char* stringNormalizer(char* origString);

    int stringTrimmerBySpace(char* origString, char* aux1, char* aux2);
    
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


#endif
