#include <Apps/apps.h>

#define BUFF_LEN (1<<10)

static char buff[BUFF_LEN+1];

void cat(int argc, char ** argv){

    int i=0;
    char c;

    while(true)     {

        c = getChar();
            
        if(c == EOF || c == '\0')    {

            buff[i] = '\0';
            print(buff);
            suicide();
        }

        if(c==KEY_BACKSPACE )    {

            if(i!=0)    {
                
                int pixelsToDelete = (buff[i-1] == KEY_TAB)? 3 : 1;
                    
                for(int j=0; j<pixelsToDelete; ++j) {

                    putChar(KEY_BACKSPACE);
                }
                                        
                buff[--i] = '\0';
            }   
        }
        else if (i < BUFF_LEN - 1)    {
                        
            buff[i++] = c;
            putChar(buff[i-1]);
        }
    }
}

