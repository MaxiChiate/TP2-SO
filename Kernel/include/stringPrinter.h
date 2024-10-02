#ifndef STRINGPRINTER_H
#define STRINGPRINTER_H

//imprime texto c
void printText(char* string, int fgcolor, int bgcolor, int charSize);
//imprime texto tamaño estándar con color de fuente y de fondo enviados por parametros
void printTextDefault(char* string, int fgcolor, int bgcolor);
//imprime caracteres tamaño estándar con color de fuente y de fondo enviados por parametros
void printCharDefault(char c,int fgcolor, int bgcolor);
//inicializa la impresión de registros (prepara la pantalla)
void registerPrintInit();
//función que imprime los registros
void printRegisterDefault(char* string);
//imprime salto de línea
void printNewline();
//imprime un tab
void printTab();
//hace un backspace
void backspace();
//aumenta el tamaño de la letra
void biggerText();
//achica el tamaño de la letra
void smallerText();
//vacía la pantalla
void refillScreen();
//cambia el color de la fuente al enviado por argumentos para toda la fuente que se muestra en pantalla
void changeColors(int fgcolor);
//mueve la variable que recorre el buffer hasta la posición inicial para reiniciarlo
void resetBuffer();



#endif //STRINGPRINTER_H
