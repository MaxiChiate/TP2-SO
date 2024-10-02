#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <colours.h>
#include <systemCalls.h>
#include <buffer.h>



#define TIMERTICK_INTERRUPTION_MESSAGE "Tick numero: "

static void int_21();
static void int_20();

typedef void (*int_xx)(void);

static int_xx interruptions[2] = {&int_20, &int_21};


void irqDispatcher(uint64_t irq) {
	interruptions[irq]();
	return;
}


void int_20() {
	timer_handler();
}
static int printingRegisters=0;

void int_21() {
    char key=map(keyboard_handler());
    if(!printingRegisters)	{
        if(key == '\a')	{	// Si se quieren imprimir los registros
            printingRegisters=1;
            registerPrintInit();
        }
        else
            putChar(key);
    }
    if (key=='\r') {
        refillScreen();
        printingRegisters = 0;
        }
}

// Syscalls:
// ¡ATENCION! Ya todos los parametros estan en uso, no cambiar los tipos de dato.
void int_80(int id, unsigned int rbx,  char * rcx, unsigned int rdx, char rsi, unsigned int rdi){
	
	switch(id)	{
        case SYSTEM_EXIT_ID : {
            exitProgram();
            break;
        }
		case SYSTEM_WRITE_ID :	{
			sysWrite(rbx, rdx);
			break;
		}
		
		case SYSTEM_READ_ID :	{
			sysRead(rbx, rcx, rsi);
			break;
		}
        case SYSTEM_TIME_ID :{
            timeManager();
            break;
        }
		case SYSTEM_DRAW_ID : {
			sysDraw(rbx, rdx, rdi, rsi);
			break;
		}
		case SYSTEM_CLEAR_ID:	{
			sysClear();
			break;
		}
        case SMALLER_TEXT : {
            makeTextSmaller();
            break;
        }
        case BIGGER_TEXT : {
            makeTextBigger();
            break;
        }
		case RANDLONG_ID :	{
			rand();
			break;
		}
        case RECOLOR:{
            changeColor(rbx);
            break;
        }
        case SONG:{
            playSong(rbx);
            break;
        }
		default:	{
			break;
			}
	}
}

