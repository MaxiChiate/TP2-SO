#include <keyboard.h>



int caps=0;
int shift=0;

static char kbd_US [81][2] =   {
        {0,0},  {'|',0}, {'1','!'}, {'2','"'}, {'3','#'},{'4','$'}, {'5','%'}, {'6','&'}, {'7','/'}, {'8','('}, {'9',')'}, {'0','='}, {'\'','\?'},{0,0}, {'\b','\b'},
        {'\t','\t'}/*Tab*/,{'q','Q'}, {'w','W'}, {'e','E'}, {'r','R'}, {'t','T'}, {'y','Y'}, {'u','U'}, {'i','I'}, {'o','O'}, {'p','P'}, {0,0}, {'+','*'}, {'\n','\n'},
        {0,0}/*CTRL*/,{'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {'}',']'}, {0,0},{0,0}, {'S',0}, {'}',']'},
        {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',',';'}, {'.',':'}, {'-','_'}, {'S',0},
        {0,0},{0,0}/* Alt */,{' ',' '}/* Space bar */,  {'M',0} /* Caps lock */,
        {'\a','\a'}/*F1 key ... > */,{0,0},   {0,0},   {0,0},   {0,0},   {0,0},   {0,0},   {0,0},   {0,0}, {0,0}/* < ... F10 */,{0,0}/* 69 - Num lock*/,{0,0} /* Scroll Lock */,{0,0}/* Home key */,
        {'U',0}/* Up Arrow */,{0,0}/* Page Up */,{0,0},{'L',0}/* Left Arrow */,{0,0},{'R',0}/* Right Arrow */,{0,0},{0,0},{'D',0}/* Down Arrow */
};


char map(unsigned char c)  {
    char letter=kbd_US[c][0];

    if (letter=='S') {     //me fijo si tengo el shift, c-128 sirve para ver si se soltó
        shift = 1;
        return '\0';
    }
    if (kbd_US[c-128][0]=='S') {
        shift = 0;
        return '\0';
    }

    if (c<=58) {
        switch (letter) {
            case 'M' :  caps=!caps;
                        return '\0';
            default: return ((caps-shift)?((caps && letter>='0' && letter<='9')?letter:kbd_US[c][1]):(shift && letter>='0' && letter<='9')?kbd_US[c][1]:letter);   //con esto se verifica cual de los dos elems de cada subarray debe devolver. Además verifica que solo shift puede modificar el retorno de numeros y sus simbolos, caps no los activa
        }
    }
    return '\0';
}


