#include <colours.h>
#include <systemCalls.h>
#include <videoDriver.h>
#include <stringPrinter.h>
#include <buffer.h>
#include <rtcDriver.h>
#include <time.h>
#include <speaker.h>

static int characterColor=WHITE;

extern void haltcpu();

char sysWrite(unsigned int fd, unsigned int count)	{

	switch(fd)	{
	// STDOUT : Salida estandar.
		case STDOUT :	{
			char s [count+1];
			readBuffer(s, count+1);
			printTextDefault(s, characterColor, BLACK);
			break;
		}
		case CHARSTDOUT : {
			printCharDefault(readChar(), characterColor, BLACK);
			break;
		}
	// STDERR : Salida de error (salida estandar pero en rojo).
		case STDERR :	{
			char s [count+1];
			readBuffer(s, count+1);
			printTextDefault(s, RED, BLACK);
			break;
		}
	// RETURN_CHAR : Devuelve en rax el ultimo caracter del buffer.
		case RETURN_CHAR :	{ 
			return readChar();
			break;
		}
		case RETURNANDSTDOUT_CHAR :	{
			char c = readChar();
			printCharDefault(c, characterColor, BLACK);
			return c;
		}

		default :
			break;
	}

	return 0;
}

void sysRead( unsigned int fd, const char * s, char c)	{
    switch(fd)	{
		case STRING :	{
			putString(s);
			break;
		}
		case CHAR :	{
			putChar(c);
			break;
		}
		default :
			break;
	}
	
}

void sysKillBuffer()	{
	cleanBuffer();
}

void sysClear()	{
	fillScreen(BLACK);
    resetPosition();
    resetBuffer();
}

void timeManager(){
    char* time=getTime();
    printTextDefault(time, characterColor, BLACK);
}
void makeTextBigger(){
    biggerText();
}
void makeTextSmaller(){
    smallerText();
}
void changeColor(unsigned int fgcolor){
    characterColor=fgcolor;
    changeColors(fgcolor);
}

void killOS(){
    fillScreen(BLACK);
    haltcpu();          //queda trabado acá y no puede hacer nada más

}
void playSong(int i){
    switch (i) {
        case TETRIS:
            playTetris();
            return;
        case JINGLEBELLS:
            playJingleBells();
            return;
        case BEEP:
            beep();
            return;
        case MARIO:
            playMario();
            return;
    }
}

static unsigned long seed = 0x5A5A5A5A;

unsigned long rand() {
    unsigned long ans = 1103515245;

    char * currentTime = getTime();
    
    // Ensure currentTime is a valid string with at least 19 characters.
    if (strlen(currentTime) >= 19) {
        ans *= currentTime[1] * currentTime[0] +
               currentTime[3] * currentTime[4] +
               currentTime[6] * currentTime[7] +
               currentTime[9] * currentTime[10] +
               currentTime[12] * currentTime[13] +
               currentTime[15] * currentTime[16] * currentTime[17] * currentTime[18];
    } else {

        ans *= ticks_elapsed();
    }

    ans += ((unsigned long) currentTime) << 11;

    seed = seed * ans;
    ans = ans ^ seed;

    ans *= ticks_elapsed();

    // Return a 31-bit positive number
    return ans & 0x7fffffff;
}

