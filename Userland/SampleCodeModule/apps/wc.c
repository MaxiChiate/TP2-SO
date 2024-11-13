#include <Apps/apps.h>

#define LINE_STRING "$> "

void read_wc(char * buffer, unsigned int buflen, int * counter);

void wc(int argc, char ** argv) {

    int to_return = 0;

    int buflen = 1024;
    char buffer[buflen];

    puts("Starting wc...");

    read_wc(buffer, buflen, &to_return);

    printUinteger(to_return);
    suicide();
    
}

static void new_line()  {

    putEnter();
    print(LINE_STRING);
}

void read_wc(char * buffer, unsigned int buflen, int * len)   {

    if(buflen < 1 || buffer == NULL)  {

        print("\n\nBuffer Error\n\n");
        return;
    }

    int i=0;
    char c;

    new_line();

    while(true) {

        getChar(&c);

        if(c!='\n')    {
            
            if(c == EOF)    {

                suicide();
            }
            else if (c!='\0')    {

                if (c == CTRL_C)     {

                    buffer[0] = '\0';
                    new_line();
                }
                else if(c=='\b' )    {

                    if(i!=0)    {
                    
                        int pixelsToDelete = (buffer[i-1] == '\t')? 3 : 1; //Si borro un tab queda en 3 sino queda en 1
                        
                        for(int j=0; j<pixelsToDelete; ++j) {

                            putChar('\b');
                        }
                                            
                        buffer[--i] = '\0';
                    }   
                }
                else if (i < buflen - 1)    {
                        
                    buffer[i++] = c;
                    putChar(buffer[i-1]);
                }
            }
        }
        else if(i > 0)   {

            buffer[i] = '\0';
            return;
        }
        else  {

        // No escribieron nada, meto un enter y vamos de vuelta:
            new_line();
        }
    }    
}
