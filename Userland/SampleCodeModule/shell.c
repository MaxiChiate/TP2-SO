#include <userlib.h>
#include <snake.h>
#include <frontSnake.h>
#include <shell.h>
#include <colours.h>

void initShell()    {
    print(INIT_MESSAGE);
}

void read(char * buffer)   {
    print("$> ");
    int i=0;
    char c;
    while((c = getChar())!='\n')    {
        
        if (c!='\0')    {
            
            if(c=='\b' ) {

                if(i!=0)    {
                
                    int pixelsToDelete = (buffer[i-1] == '\t')? 3 : 1; //Si borro un tab queda en 3 sino queda en 1
                    
                    for(int j=0; j<pixelsToDelete; ++j) {

                        putChar('\b');
                    }
                    
                    buffer[--i] = '\0';
                }
                
            }

            else    {

                buffer[i++] = c;
                putChar(buffer[i-1]);
                
            }
        }        
    }

    buffer[i] = '\0';
}

    static char function[15]={'\0'};
    static char argument[15]={'\0'};
    static int hasArgs=0;
void getMenu(char* buffer){
    
    int i=0;
    buffer=stringNormalizer(buffer);
    hasArgs=stringTrimmerBySpace(buffer, function, argument);
    if (hasArgs==15 || function[14]!='\0') {
        print("\n");
        print("You're trying to overflow the buffer, aren't you? By the way, that's an Unknown Command");
        print("\n");
        return;
    }
    while (menuNames[i]){
        if (strEquals(menuNames[i], function)) {
            print("\n");
            menuFunctions[i]();
            print("\n");
            return;
        }
        i++;
    }
    print("\n");
    print("Unknown Command");
    print("\n");
}

void help(){
    if (hasArgs){
        print("Function doesn't have arguments");
        return;
    }
    putnEnters(2);
    for (int i = 0; i<AVAILABLE_FUNCTIONS; i++) {
        print(menuNames[i]);
        print(":");
        print(menuDescriptions[i]);
        putnEnters(2);
    }
}
void time(){
    if (hasArgs){
        print("Function doesn't have arguments");
        return;
    }
    time_getter();
}

void snake(){

    if (strEquals("1", argument))   {
        startSnake(1);
        initShell();
    }
        
    else if (argument[0]=='2' && argument[1]=='\0')  {
        startSnake(2);
        initShell();
    }
        
    else print("Arguments are necessary or the argument written is not defined");



}
void textSize(){
   if (strEquals("smaller", argument)){
       make_text_smaller();
   }
   else if (strEquals("bigger", argument)){
       make_text_bigger();
   }
   else print("Arguments are necessary or the argument written is not defined");
}

void exitProgram(){
    if (hasArgs){
        print("Function doesn't have arguments");
        return;
    }
    exit_shell();
}
void colorChanging(){
    if (strEquals(argument, "white")){
        recolor(WHITE);
    }
    else if (strEquals(argument, "red")){
        recolor(RED);
    }
    else if (strEquals(argument, "blue")){
        recolor(BLUE);
    }
    else if (strEquals(argument, "green")){
        recolor(GREEN);
    }
    else if (strEquals(argument, "yellow")){
        recolor(YELLOW);
    }
    else if (strEquals(argument, "orange")){
        recolor(ORANGE);
    }
    else if (strEquals(argument, "violet")){
        recolor(VIOLET);
    }
    else print("Arguments are necessary or the argument written is not defined");

}
void tetrisSong(){
    if (hasArgs){
        print("Function doesn't have arguments");
        return;
    }
    play_song(1);
}
void jingleBellsSong(){
    if (hasArgs){
        print("Function doesn't have arguments");
        return;
    }
    play_song(2);
}

void div0() {
    if (hasArgs){
        print("Function doesn't have arguments");
        return;
    }
    int i= 1/0;
}

