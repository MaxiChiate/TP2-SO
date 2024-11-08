#ifndef _SHELL_H
#define _SHELL_H

    #include <userlib.h>
    #include <Testing/tests.h>

    #define INIT_MESSAGE "Welcome to shell, write help if you want to know the available options\n"
    // #define OVERFLOW_MESSAGE "\nYou're trying to overflow the buffer, aren't you? By the way, that's an Unknown Command\n"
    #define OVERFLOW_MESSAGE "\nIncorrect argument format: maximum number of arguments is 3 with a length of 14 characters each\n"

    #define SECTIONS 3

    #define AVAILABLE_FUNCTIONS ((AVAILABLE_BUILDIN_F)+(AVAILABLE_PROCESS_F)+(AVAILABLE_TESTS))
    #define AVAILABLE_BUILDIN_F 5
    #define AVAILABLE_PROCESS_F 0
    #define AVAILABLE_TESTS     4

    #define BACKGROUND_CHARACTER '&'
    #define LINE_STRING "$> "

    #define MAX_ARGS    4
    #define MAX_ARG_LONG 15

    typedef void (*build_in_f)(int, char **);
    typedef void (*process_f)(int, char **);

    void initShell();
    void read(char * buffer);
    void getMenu(char * buffer);
    void help(int argc, char ** argv);
    void time(int argc, char ** argv);
    void div0(int argc, char ** argv);


    

#endif