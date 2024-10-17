#include <userlib.h>
#include <shell.h>
#include <Testing/tests.h>

build_in_f build_in_functions[AVAILABLE_BUILDIN_F]={&help, &time, &clear, &div0, &invalidOpcode};
process_f  process_functions[AVAILABLE_PROCESS_F]={};
process_f  test_functions[AVAILABLE_TESTS]={&test_processes, &test_prio};


char* build_in_names[AVAILABLE_BUILDIN_F+1]= {"help", "time", "clear", "div0", "invalidopcode", 0};
char* process_names[AVAILABLE_PROCESS_F+1]={'\0'};
char* test_names[AVAILABLE_TESTS+1]={"test_processes", "test_prio", '\0'};

char* build_in_descriptions[AVAILABLE_BUILDIN_F+1]={
                            "Gives information about the available commands to execute",
                            "Prints the RTC's time on the screen",
                            "Cleans the terminal",
                            "Div by 0 and throws the exception",
                            "Excecutes an invalid opcode and throws the exception",
                            '\0'
                            };
char* process_descriptions[AVAILABLE_PROCESS_F+1]={
                            '\0'
                            };
char* test_descriptions[AVAILABLE_TESTS+1]={
                            "Test process creations",
                            "Test process priority",
                            '\0'
                            };


void initShell()    {
    print(INIT_MESSAGE);
}

void read(char * buffer)   {
    print(LINE_STRING);
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

static bool check_and_run(build_in_f * f, char ** names) {

    if(f != NULL)   {

        for(int i=0; names[i]; i++)   {
            
            if (strEquals(names[i], function)) {
                putEnter();
                f[i]();
                putEnter();
                return true;
            }
        }
    }

    return false;
}

static bool check_and_run_process(process_f * p, char ** names, int argc, char ** argv, bool background)  {

    if(p != NULL)   {

        for(int i=0; names[i]; i++)   {
            
            if (strEquals(names[i], function)) {
                putEnter();

                if(background)  {
                    
                    spawn_process((int64_t) p[i], argc, argv, 2, true); // TODO: Blockear salida estandar de proceso en background.
                }
                else    {

                    int64_t cpid = run_process((int64_t) p[i], 1, argv, 2, true);    //TODO: Forzar el timer tick para que corra el nuevo proceso.
                    waitpid(cpid);
                }

                putEnter();
                return true;
            }
        }
    }

    return false;
}

void getMenu(char* buffer){
    
    buffer=stringNormalizer(buffer);
    hasArgs=stringTrimmerBySpace(buffer, function, argument);
    if (hasArgs==15 || function[14]!='\0') {
        print("\nYou're trying to overflow the buffer, aren't you? By the way, that's an Unknown Command\n");
        return;
    }

    if(check_and_run(build_in_functions, build_in_names))   {

        return;
    }

    bool background = false;
    for(int k=0; k<hasArgs && !background; k++)    {

        background = argument[k] == BACKGROUND_CHARACTER;
    }

    //TODO: Corregir pasaje de argumentos a los procesos.
    if(check_and_run_process(process_functions, process_names, 1, NULL, background) ||
    check_and_run_process(test_functions, test_names, 1, NULL, background)) {

        return;
    }

    print("\nUnknown Command\n");
}
void help() {

    if (hasArgs){
        print("Function doesn't have arguments");
        return;
    }
    putnEnters(2);

    char ** names[SECTIONS] = {build_in_names, process_names, test_names};
    char ** descriptions[SECTIONS] = {build_in_descriptions, process_descriptions, test_descriptions};
    int amount[SECTIONS] = {AVAILABLE_BUILDIN_F, AVAILABLE_PROCESS_F, AVAILABLE_TESTS};

    for(int k=0; k < SECTIONS; k++)    {
        
        for (int i = 0; i<amount[k]; i++) {
        
            print(names[k][i]);
            print(":");
            print(descriptions[k][i]);
            putnEnters(2);
        }
    }
    
    putEnter();
}

void time(){
    if (hasArgs){
        print("Function doesn't have arguments");
        return;
    }
    time_getter();
}

void div0() {
    int i = 1/0;
    i++;
}

