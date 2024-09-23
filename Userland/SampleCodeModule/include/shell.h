#ifndef _SHELL_H
#define _SHELL_H

    #define INIT_MESSAGE "Welcome to shell, write help if you want to know the available options\n"
    #define AVAILABLE_FUNCTIONS 11

    void initShell();
    void read(char * buffer);
    void getMenu(char* buffer);
    void help();
    void time();
    void snake();
    void textSize();
    void exitProgram();
    void colorChanging();
    void tetrisSong();
    void jingleBellsSong();
    void div0();
    
    typedef void (*shellFunctions)(void);

    static shellFunctions menuFunctions[AVAILABLE_FUNCTIONS]={&help, &snake, &time, &textSize,&colorChanging, &exitProgram, &clear, &tetrisSong, &jingleBellsSong, &div0, &invalidOpcode};

    static char* menuNames[AVAILABLE_FUNCTIONS+1]= {"help", "snake", "time", "size", "color", "exit", "clear","tetris", "jinglebells", "div0", "invalidopcode", 0};

    static char* menuDescriptions[AVAILABLE_FUNCTIONS]={
                            "Gives information about the available commands to execute",
                            "Starts a new Snake game, add '1' or '2' for the number of players wanted. To pause use 'p' key. Don't watch registers when the game is not paused",
                            "Prints the RTC's time on the screen",
                            "Changes font size unless minimum/maximum size has been reached. Use argument 'small' or 'bigger'",
                            "Changes the font's color, available arguments are 'white', 'red', 'blue', 'green', 'yellow', 'orange', 'violet'",
                            "Closes the Shell and finishes the execution of the program",
                            "Cleans the terminal",
                            "Plays tetris music",
                            "Plays jinglebells",
                            "Div by 0 and throws the exception",
                            "Excecutes an invalid opcode and throws the exception"
                            };


#endif