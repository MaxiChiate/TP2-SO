#include <shell.h>

// Fd globales que solo sabe la shell:
#include <../../../Kernel/include/process/globalfd.h>

process_f  process_functions[AVAILABLE_PROCESS_F]={&mem,&loop, &ps, &block,&kill, &nice,&phylo, &cat, &filter, &wc, &help, &clear};
process_f  test_functions[AVAILABLE_TESTS]={&test_processes, &test_prio, &test_sync, &test_mm, &test_all};


char* process_names[AVAILABLE_PROCESS_F+1]={"mem","loop", "ps", "block", "kill", "nice", "phylo", "cat", "filter", "wc", "help", "clear", '\0'};
char* test_names[AVAILABLE_TESTS+1]={"test_processes", "test_prio", "test_sync", "test_mm", "test_all",'\0'};

char* process_descriptions[AVAILABLE_PROCESS_F+1]={
                            "Gives information about free and used memory.",
                            "Prints the process id in a loop",
                            "Prints the status of every process in execution",
                            "Blocks a process by it's pid",
                            "Kills a process by it's pid",
                            "Changes a process' priority by it's pid",
                            "Simulates the Philosophers problem solved with semaphores",
                            "Prints input from stdin",
                            "Filter vocals from stdin input",
                            "Count enters amount from stdin input",
                            "Gives information about the available commands to execute",
                            "Cleans the terminal",
                            '\0'
                            };
char* test_descriptions[AVAILABLE_TESTS+1]={
                            "Test process creations. \nUsage: test_processes <max processes> <show process, 1 or 0>",
                            "Test process priority. \nUsage: test_priority",
                            "Test semaphore syncro. \nUsage: test_sync <increment times> <use sem, 1 or 0>",
                            "Test memory manager. \nUsage: test_mm <max_memory>",
                            "Run all tests together",
                            '\0'
                            };


void initShell()    {

    print(INIT_MESSAGE);
}

static void new_line()  {

    putEnter();
    print(LINE_STRING);
}

void read(char * buffer, unsigned int buflen)   {

    if(buflen < 1 || buffer == NULL)  {

        print(BUFFER_ARGS_ERROR_MESSAGE);
        return;
    }

    int i=0;
    char c;

    new_line();

// Por si escribieron en stdin durante la ejecucion de un proceso que no la leyó:
    consume_stdin();

    while(true) {

        c = getChar();

        if(c!=KEY_ENTER)    {
            
            if(c == EOF)    {

                suicide();
            }
            else if (c!='\0')    {

                if (c == CTRL_C)     {

                    i = 0;
                    buffer[i] = '\0';
                    new_line();
                }
                else if(c==KEY_BACKSPACE )    {

                    if(i!=0)    {
                    
                        int pixelsToDelete = (buffer[i-1] == KEY_TAB)? 3 : 1; //Si borro un tab queda en 3 sino queda en 1
                        
                        for(int j=0; j<pixelsToDelete; ++j) {

                            putChar(KEY_BACKSPACE);
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

static bool check_and_run_process( process_f * p,  char ** names, int argc, char ** argv,  char * function)  {

    if(p != NULL && names != NULL)   {

        for(int i=0; names[i]; i++)   {
            
            if (strEquals(names[i], function))  {
                argv[0]=names[i];

                bool background = false;
                int back_index = 0;

            // Busco el BACKGROUND_CHARACTER:
                for (back_index = 0; back_index < argc && !background; back_index++)   {

                    background = (bool) (argv[back_index][0] == BACKGROUND_CHARACTER);
                }

                if(back_index < argc-1) {

                    char s[2] = {argc-1-back_index + '0', '\0'};

                    print("\nWaring: Ignoring ");
                    print(s);
                    print(" arguments after &\n");
                }
                
                putEnter();

                if(background)  {
   
                    spawn_process((int64_t) p[i], argc-1, argv, 1, false);
                }
                else    {

                    int64_t cpid = run_process((int64_t) p[i], argc, argv, 1, true);
                    waitpid(cpid);
                }

                putEnter();
                return true;
            }
        }
    }

    return false;
}


void getMenu(char * buffer, unsigned int buflen)  {
   
    buffer = stringNormalizer(buffer, buflen);

    char function[MAX_ARG_LONG]={'\0'};

    char name[MAX_ARG_LONG]={'\0'};
    char arg1[MAX_ARG_LONG]={'\0'};
    char arg2[MAX_ARG_LONG]={'\0'};
    char arg3[MAX_ARG_LONG]={'\0'};

    char * argv[MAX_ARGS+2] = {name,arg1, arg2, arg3, NULL};

    int argc = stringTrimmerBySpace(buffer, function, argv+1, MAX_ARG_LONG)+1;

    if (argc>=MAX_ARGS || argc < 0 || function[MAX_ARG_LONG-1]!='\0') {

        print(OVERFLOW_MESSAGE);
        return;
    }

    for(int i = argc; i<MAX_ARGS; ++i)     {

        argv[i] = NULL;
    }


    if(strEquals(function, "clear"))    {

    // Build in:
        clear();
        return;
    }
    if(check_and_run_process(process_functions, process_names, argc, argv, function) ||
        check_and_run_process(test_functions, test_names, argc, argv, function)) {

        return;
    }

    print(INVALID_INPUT_MESSAGE);
}

void print_commands() {


    char ** names[SECTIONS] = {process_names, test_names};
    char ** descriptions[SECTIONS] = {process_descriptions, test_descriptions};
    int amount[SECTIONS] = {AVAILABLE_PROCESS_F, AVAILABLE_TESTS};

    for(int k=0; k < SECTIONS; k++)    {
        
        for (int i = 0; i<amount[k]; i++) {
        
            print(names[k][i]);
            print(":");
            print(descriptions[k][i]);
            putnEnters(2);
        }
    }
    
}

