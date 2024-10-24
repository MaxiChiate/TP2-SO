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
    #include <buffer.h>
    #include <rtcDriver.h>
    #include <time.h>
    #include <process_management.h>

    void call_syscall(int id, int64_t rdi, int64_t rsi, int64_t rdx, int64_t rcx, int64_t r8);


//  Wrappers:

    typedef void (*syscall_wrapper)(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);

    void sysWrite_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void sysRead_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void sysClear_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void rand_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void create_process_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void kill_process_by_pid_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void block_process_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void unblock_process_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void change_process_priority_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void waitpid_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void haltcpu_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void get_current_pid_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void give_up_cpu_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void wait_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void suicide_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);
    void timeManager_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t);

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