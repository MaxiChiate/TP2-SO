#ifndef _SYSCALLS_H
#define _SYSCALLS_H

    #include <bool.h>
    #include <stdint.h>

// Sys_Id en rax:
#define SYSTEM_KILLOS_ID                0x01
#define SYSTEM_READ_ID                  0x03
#define SYSTEM_WRITE_ID                 0x04
#define SYSTEM_TIME_ID                  0x05
#define SYSTEM_CLEAR_ID                 0x06
#define RANDLONG_ID                     0x07

// Process:

#define PS_ID                           0xB0
#define CREATE_PROCESS                  0xB1
#define KILL_PROCESS                    0xB3
#define GET_CURRENT_PID                 0xB4
#define CHANGE_PROCESS_PRIORITY         0xB5
#define BLOCK_PROCESS                   0xB6
#define UNBLOCK_PROCESS                 0xB7
#define GIVE_UP_CPU                     0xB8
#define WAIT                            0xB9
#define WAIT_PID                        0xBA
#define SUICIDE                         0xBB



// write @fd codes: (en rbx)
    #define STDOUT 						0x01
    #define CHARSTDOUT                  0x02
    #define STDERR 						0x03
    #define RETURN_CHAR                 0xA0	
    #define RETURNANDSTDOUT_CHAR        0xA1

// Devuelve segun fd lo que hay en el buffer, count caracteres:
    char sysWrite(unsigned int fd, unsigned int count);

// read @fd codes: (en rbx)
    #define STRING  0x02
    #define CHAR    0x01

// Carga en el buffer lo que hay en @s:
    void sysRead(unsigned int fd, const char * s, char c);

// Borra toda la pantalla:
    void sysClear();

// Deja el programa detenido indefinidamente
    void killOS();

//Imprime el horario en pantalla
    void timeManager();

//aumenta el tamaño de la letra
    void makeTextBigger();

//reduce el tamaño de la letra
    void makeTextSmaller();
//cambia el color de lo escrito hasta ahora
    void changeColor(unsigned int fgcolor);

// retorna un numero pseudo-random
    unsigned long rand();
//sonidos/Canciones disponibles para usar
    #define TETRIS 1
    #define JINGLEBELLS 2
    #define BEEP 3
    #define MARIO 4

//pone la canción del tetris
    void playSong(int which);

#define PROCESS_NAME_LENGTH 10

    typedef struct  {

        uint64_t sp;
        uint64_t bp;
        uint64_t id;
        bool foreground;
        int priority;
        char name[PROCESS_NAME_LENGTH+1];
    } ps_t;

#endif