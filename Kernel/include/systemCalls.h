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
    #include <buffers_manager.h>
    #include <rtcDriver.h>
    #include <time.h>
    #include <process_management.h>


// Devuelve segun fd lo que hay en el buffer, count caracteres:
    char sysWrite(unsigned int fd, unsigned int count);


// Carga en el buffer lo que hay en @s:
    void sysRead(unsigned int fd, const char * s, char c);

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