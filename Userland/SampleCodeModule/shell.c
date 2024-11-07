
#include <shell.h>


build_in_f build_in_functions[AVAILABLE_BUILDIN_F]={&help, &time, &clear, &div0, &invalidOpcode};
process_f  process_functions[AVAILABLE_PROCESS_F]={};
process_f  test_functions[AVAILABLE_TESTS]={&test_processes, &test_prio, &test_sync, &test_mm};


char* build_in_names[AVAILABLE_BUILDIN_F+1]= {"help", "time", "clear", "div0", "invalidopcode", 0};
char* process_names[AVAILABLE_PROCESS_F+1]={'\0'};
char* test_names[AVAILABLE_TESTS+1]={"test_processes", "test_prio", "test_sync", "test_mm", '\0'};

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
                            "Test process creations. \nUsage: test_processes <max processes> <show process, 1 or 0>",
                            "Test process priority. \nUsage: test_priority",
                            "Test semaphore syncro. \nUsage: test_sync <increment times> <use sem, 1 or 0>",
                            "Test memory manager. \nUsage: test_mm <max_memory>",
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


static bool check_and_run( build_in_f * f,  char ** names, int argc, char ** argv, char * function) {

    if(f != NULL)   {

        for(int i=0; names[i]; i++)   {
            
            if (strEquals(names[i], function)) {
                putEnter();
                f[i](argc, argv);
                putEnter();
                return true;
            }
        }
    }

    return false;
}

static bool check_and_run_process( process_f * p,  char ** names, int argc, char ** argv,  char * function)  {

    if(p != NULL && names != NULL)   {

        for(int i=0; names[i]; i++)   {
            
            if (strEquals(names[i], function))  {

                bool background = (bool) (argc > 0 && argv[argc - 1][0] == BACKGROUND_CHARACTER);

                putEnter();

                if(background)  {
                    
                    spawn_process((int64_t) p[i], argc, argv, 2, true);
                }
                else    {

                    int64_t cpid = run_process((int64_t) p[i], argc, argv, 2, false);
                    waitpid(cpid);
                }

                putEnter();
                return true;
            }
        }
    }

    return false;
}


void getMenu(char* buffer)  {
   
    buffer = stringNormalizer(buffer);

    char function[MAX_ARG_LONG]={'\0'};

    char arg1[MAX_ARG_LONG]={'\0'};
    char arg2[MAX_ARG_LONG]={'\0'};
    char arg3[MAX_ARG_LONG]={'\0'};

    char * argv[MAX_ARGS+1] = {arg1, arg2, arg3, NULL};

    int argc = stringTrimmerBySpace(buffer, function, argv);

    if (argc>=4 || argc < 0 || function[MAX_ARG_LONG-1]!='\0') {

        print(OVERFLOW_MESSAGE);
        return;
    }

    for(int i = argc; i<MAX_ARGS; ++i)     {

        argv[i] = NULL;
    }

// Si es build-in:

    if(check_and_run(build_in_functions, build_in_names, argc, argv,  function))   {

        return;
    }


// Si es programa o test:

    if(check_and_run_process(process_functions, process_names, argc, argv, function) ||
        check_and_run_process(test_functions, test_names, argc, argv, function)) {

        return;
    }

    print("\nUnknown Command\n");
}

void help(int argc, char ** argv) {

    if (argc){
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

void time(int argc, char ** argv){
    if (argc){
        print("Function doesn't have arguments");
        return;
    }
    time_getter();
}

void div0(int argc, char ** argv) {
    int i = 1/0;
    i++;
}

