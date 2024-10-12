#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <colours.h>
#include <systemCalls.h>
#include <buffer.h>
#include <stringPrinter.h>
#include <process_management.h>


#define TIMERTICK_INTERRUPTION_MESSAGE "Tick numero: "

static void int_21();
static void int_20();

typedef void (*int_xx)(void);

static int_xx interruptions[2] = {&int_20, &int_21};


void irqDispatcher(uint64_t irq) {
	interruptions[irq]();
}


void int_20() {
	timer_handler();
}


void int_21() {
    
    char key=map(keyboard_handler());
    
    putChar(key);
}

// Syscalls:
// ¡ATENCION! Ya todos los parametros estan en uso, no cambiar los tipos de dato.


void int_80(int id, unsigned int rbx,  char * rcx, unsigned int rdx, char rsi, unsigned int rdi, char * r9[], uint64_t r8 ){	

        switch(id)	{
            case SYSTEM_KILLOS_ID : {
                killOS();
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
            case SYSTEM_CLEAR_ID:	{
                sysClear();
                break;
            }
            case RANDLONG_ID :	{
                rand();
                break;
            }


        case PS_ID: {
            get_ps();
            break;
        }
        case KILL_PROCESS: {
            kill_process_by_pid((uint64_t)rbx);
            break;
        }
        case GET_CURRENT_PID: {
            get_current_pid();
            break;
        }
        case CHANGE_PROCESS_PRIORITY: {
            change_process_priority((uint64_t) rbx, rdx); // rbx: pid, rdx: priority
            break;
        }
        case BLOCK_PROCESS: {
            block_process((uint64_t) rbx); // rbx: pid
            break;
        }
        case UNBLOCK_PROCESS: {
            unblock_process((uint64_t) rbx); // rbx: pid
            break;
        }
        case GIVE_UP_CPU: {
            give_up_cpu();
            break;
        }
        case WAIT: {
            wait();
            break;
        }
        case WAIT_PID: {
            waitpid((uint64_t) rbx);
            break;
        }
		default: {
			break;
		}
    }
}