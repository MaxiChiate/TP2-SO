#ifndef _SHELL_H
#define _SHELL_H

    #define INIT_MESSAGE "Welcome to shell, write help if you want to know the available options\n"

    #define SECTIONS 3

    #define AVAILABLE_FUNCTIONS ((AVAILABLE_BUILDIN_F)+(AVAILABLE_PROCESS_F)+(AVAILABLE_TESTS))
    #define AVAILABLE_BUILDIN_F 5
    #define AVAILABLE_PROCESS_F 0
    #define AVAILABLE_TESTS     2

    #define BACKGROUND_CHARACTER '&'
    #define LINE_STRING "$> "

    typedef void (*build_in_f)(void);
    typedef void (*process_f)(int, char **);

    void initShell();
    void read(char * buffer);
    void getMenu(char * buffer);
    void help();
    void time();
    void div0();


    

#endif