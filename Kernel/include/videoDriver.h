#ifndef _VIDEODRIVER_H_
#define _VIDEODRIVER_H_

#include <stdint.h>

//si se cambia la resolución habría que cambiarlo
#define PIXELS 3
#define WIDTH 1024
#define HEIGHT 768


//pone un pixel de un color en una ubicación
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
//llena la pantalla de un solo color
void fillScreen(uint32_t hexColor);
//llama a render enviándole el subarray con los datos de una letra en específico
void drawCharWithoutDisplacement(unsigned char c,int fgcolor, int bgcolor);
void drawCharOnCurrentPos(unsigned char c,int fgcolor, int bgcolor);
void drawCharOnPreviousPosition(unsigned char c,int fgcolor, int bgcolor);
//imprime la letra en pantalla usando la fuente que se manda desde draw char
void render(char *bitmap, int fgcolor, int bgcolor, int xinit, int yinit, int charSize);
//permite cambiar el tamaño de la letra(en multiplos de 8)
void setCharWidth(unsigned int size);
//setea la dirección de video en función de las líneas que se desplazó la pantalla
void setCurrentVideoLinePos(int linesToScroll);
//mueve la pantalla hacia abajo
void scroll();
//imprime el cursor en pantalla
void printCursor();
//borra el cursor de la pantalla
void deleteSlash();
//mueve el cursor hasta la nueva línea
void newline();
//verifica si es posible imprimir el cursor
void blink();
//activa el cursor
void allowBlink();
//apaga el cursor
void blockBlink();
//borra la última línea al hacer scroll
void cleanLastLine();
//cambia el tamaño de caracter estándar
void setCharWidth(unsigned int size);
//vuelve a la posición 0 de la pantalla
void resetPosition();
//dibuja la cabeza de la serpiente durante el snake
void snakeHeadDrawer(int direction, int initPosY, int initPosX, char color);
//dibuja el cuerpo de la serpiente durante el snake
void snakeBodyDrawer(int initPosY, int initPosX, int size);
//imprime un pixel en una determinada posición, con un tamaño y color que se envía como argumento
void drawPixel(int y, int x, int size, char * draw, int colour);

#endif