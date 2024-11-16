#ifndef _SHELL_H
#define _SHELL_H

    #include <userlib.h>
    #include <Testing/tests.h>
    #include <Apps/apps.h>
    #include <ipclib.h>	

    #define INIT_MESSAGE "Welcome to shell, write help if you want to know the available options\n"
    // #define OVERFLOW_MESSAGE "\nYou're trying to overflow the buffer, aren't you? By the way, that's an Unknown Command\n"
    #define OVERFLOW_MESSAGE "\n\nIncorrect argument format: maximum number of arguments is 3 with a length of 14 characters each\n\n"

    #define AVAILABLE_PROCESS_F 17

    #define BACKGROUND_CHARACTER '&'
    #define PIPE_CHARACTER '|'
    #define LINE_STRING "$> "
    #define INVALID_INPUT_MESSAGE "\n\nUnknown Command\n\n"
    #define BUFFER_ARGS_ERROR_MESSAGE "\n\nBuffer error\n\n"

    #define MAX_ARGS    5
    #define MAX_ARG_LONG 15

    typedef void (*build_in_f)(int, char **);
    typedef void (*process_f)(int, char **);

    void initShell();
    void read(char * buffer, unsigned int buflen);
    void getMenu(char * buffer, unsigned int buflen);
    void print_commands();

// Imprimir char en pantalla forzado:
    void putChar_shell(char c);

#endif