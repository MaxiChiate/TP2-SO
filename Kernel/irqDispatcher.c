#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <colours.h>
#include <systemcallsWrappers.h>
#include <buffer.h>
#include <stringPrinter.h>
#include <process/process_management.h>
#include <semaphore.h>


uint64_t irqDispatcher(uint64_t irq, uint64_t rsp) {
	
    switch (irq)    {

        case 0: return timer_handler(rsp); 
        case 1: putChar(map(keyboard_handler())); return 0;
        default: return -1;
    }
}


void int_80(int id, int64_t * args)    {	

    call_syscall(id, args);
}