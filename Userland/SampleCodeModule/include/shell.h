#ifndef _SHELL_H
#define _SHELL_H

    #define INIT_MESSAGE "Welcome to shell, write help if you want to know the available options\n"
    #define AVAILABLE_FUNCTIONS 6

    void initShell();
    void read(char * buffer);
    void getMenu(char * buffer);
    void help();
    void time();
    void exitProgram();
    void div0();


    

#endif