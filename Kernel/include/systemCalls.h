//sytemCalls.h

#ifndef _SYSCALLS_H
#define _SYSCALLS_H

    #include <bool.h>
    #include <stdint.h>
    #include <systemCallDefs.h>
    #include <colours.h>
    #include <systemCalls.h>
    #include <videoDriver.h>
    #include <stringPrinter.h>
    #include <rtcDriver.h>
    #include <time.h>
    #include <process_management.h>


// Borra toda la pantalla:
    void sysClear();

// Deja el programa detenido indefinidamente
    void killOS();

//Imprime el horario en pantalla
    void timeManager();

// retorna un numero pseudo-random
    unsigned long rand();


    extern void haltcpu();



#endif