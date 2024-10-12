#include <videoDriver.h>
#include <colours.h>
#include <keyboard.h>
#include <time.h>

#define SCREEN_BUFFER_SIZE 3500
char screenBuffer[SCREEN_BUFFER_SIZE];
static unsigned int bufferColors[SCREEN_BUFFER_SIZE][2];
int position=0;
int positionTraveller=0;

int canBlink=1;
void printCharDefault(char c,int fgcolor, int bgcolor);
void refillScreen();

void printTextDefault(char* string, int fgcolor, int bgcolor) {
    blockBlink();
    for (int i = 0; string[i] != '\0'; i++) {
        deleteSlash();
        printCharDefault(string[i], fgcolor, bgcolor);
        printCursor();
    }
    allowBlink();
}

void printNewline(){
    blockBlink();
    deleteSlash();
    newline();
    printCursor();
    allowBlink();

}
void printTab(){
    blockBlink();
    deleteSlash();
    char * tab="   ";
    printTextDefault(tab, BLACK, BLACK);
    printCursor();
    allowBlink();
}


void backspace(){
    blockBlink();
    deleteSlash();
    drawCharOnPreviousPosition(' ',BLACK, BLACK);
    printCursor();
    allowBlink();
}

void printCharDefault(char c,int fgcolor, int bgcolor){
    if (c==0)
        return;
    if (c == '\a'){
        return;
    }
    if (c == '\b') {
        backspace();
        if (position > 0) {
            position--;
        }
    } else {
        screenBuffer[position % SCREEN_BUFFER_SIZE] =c;
        bufferColors[position % SCREEN_BUFFER_SIZE][0] = fgcolor;
        bufferColors[position % SCREEN_BUFFER_SIZE][1] = bgcolor;
        position++;
        if (position == positionTraveller) {
            positionTraveller++;
        }
        if (c == '\n')
            printNewline();
        else if (c == '\t')
            printTab();
        else
            drawCharOnCurrentPos(c, fgcolor, bgcolor);
    }
}

void blink(){
    if (canBlink) {
        if (alarmAt(1) && seconds_elapsed()%2==0)
            deleteSlash();
        if (alarmAt(1) && seconds_elapsed()%2==1)
            printCursor();
    }
}
void allowBlink(){
    canBlink=1;
}
void blockBlink(){
    canBlink=0;
}


void changeColors(int fgNewColor){
    fillScreen(0x0);
    resetPosition();
    int startPos = positionTraveller % SCREEN_BUFFER_SIZE;
    int endPos = position % SCREEN_BUFFER_SIZE;

    if (endPos < startPos) {
        endPos += SCREEN_BUFFER_SIZE;
    }
    positionTraveller = position;
    for (int i = startPos; i < endPos; i++) {
        int index = i % SCREEN_BUFFER_SIZE;
        bufferColors[index][0]=fgNewColor;
        printCharDefault(screenBuffer[index], bufferColors[index][0], bufferColors[index][1]);
    }
}
void refillScreen() {
    fillScreen(0x0);
    resetPosition();
    int startPos = positionTraveller % SCREEN_BUFFER_SIZE;
    int endPos = position % SCREEN_BUFFER_SIZE;

    if (endPos < startPos) {
        endPos += SCREEN_BUFFER_SIZE;
    }
    positionTraveller = position;
    for (int i = startPos; i < endPos; i++) {
        int index = i % SCREEN_BUFFER_SIZE;
        printCharDefault(screenBuffer[index], bufferColors[index][0], bufferColors[index][1]);
    }

}
void resetBuffer(){
    positionTraveller=position;
}





