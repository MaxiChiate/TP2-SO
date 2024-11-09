#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <colours.h>
#include <systemcallsWrappers.h>
#include <buffers_manager.h>
#include <stringPrinter.h>
#include <process/process_management.h>
#include <semaphore.h>


uint64_t irqDispatcher(uint64_t irq, uint64_t rsp) {
	
    switch (irq)    {

        case 0: return timer_handler(rsp); 
        case 1: {
            
            char c = map(keyboard_handler());
            kernel_write(STDIN_FILENO, &c, 1); 
            return 0;
        }
        default: return -1;
    }
}


void int_80(int id, int64_t * args)    {	

    call_syscall(id, args);
}