#include <shell.h>

// Fd globales que solo sabe la shell:
#include <../../../Kernel/include/process/globalfd.h>

process_f  process_functions[AVAILABLE_PROCESS_F]={&mem,&loop, &ps, &block,&kill, &nice,&phylo, &cat, &filter, &wc, &help, &clear,
                                                    &test_processes, &test_prio, &test_sync, &test_mm, &test_all};


char* process_names[AVAILABLE_PROCESS_F+1]={"mem","loop", "ps", "block", "kill", "nice", "phylo", "cat", "filter", "wc", "help", "clear", 
                                            "test_processes", "test_prio", "test_sync", "test_mm", "test_all",'\0'};

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

static int64_t check_and_run_process( process_f * p,  char ** names, int argc, char ** argv,  char * function, bool pipe)  {

    if(p != NULL && names != NULL)   {

        for(int i=0; names[i]; i++)   {
            
            if (strEquals(names[i], function))  {
                argv[0]=names[i];

                if(pipe)    {

                    return spawn_process((int64_t) p[i], argc, argv, 1, true);
                }


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

                int64_t cpid;

                if(background)  {
   
                    cpid = spawn_process((int64_t) p[i], argc-1, argv, 1, false);
                }
                else    {

                    cpid = run_process((int64_t) p[i], argc, argv, 1, true);
                    waitpid(cpid);
                }

                putEnter();
                return cpid;
            }
        }
    }

    return 0;
}


void getMenu(char * buffer, unsigned int buflen)  {
   
    buffer = stringNormalizer(buffer, buflen);

    char function[MAX_ARG_LONG]={'\0'};

    char name[MAX_ARG_LONG]={'\0'};
    char arg1[MAX_ARG_LONG]={'\0'};
    char arg2[MAX_ARG_LONG]={'\0'};
    char arg3[MAX_ARG_LONG]={'\0'};
    char arg4[MAX_ARG_LONG]={'\0'};

    char * argv[MAX_ARGS+2] = {name,arg1, arg2, arg3, arg4, NULL};

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

    putEnter();

    for(int i=2; i<argc; i++)   {

        if(argv[i] && argv[i][0] == PIPE_CHARACTER) {

            print("Error: Shell doesn't support piping commands with args!\n");
            return;
        }
    }

    if(argc > 0 && argv[1][0] == PIPE_CHARACTER) {

        // Comandos pipeados no reciben args!!
        argv[1] = NULL;

        char name2[MAX_ARG_LONG]={'\0'};
        char * argv2[MAX_ARGS+1] = {name2, NULL};

        int fd[2];
        pipe(fd);

        set_stdio(STDIN_FILENO, fd[1]);        

        int64_t pid1 = check_and_run_process(process_functions, process_names, 1, argv, function, true);

        if(pid1 == 0)   {

            close(fd[0]);
            close(fd[1]);

            print(INVALID_INPUT_MESSAGE);
            return;
        }

        set_stdio(fd[0], STDOUT_FILENO);

        int64_t pid2 = check_and_run_process(process_functions, process_names, 1, argv2, argv[2], true);

        if(pid2 == 0)   {

            killp(pid1); 

            close(fd[0]);
            close(fd[1]);

            print(INVALID_INPUT_MESSAGE);
            return;
        }

        set_stdio(STDIN_FILENO, STDOUT_FILENO);

        waitpid(pid1);
        close(fd[1]);

        waitpid(pid2);
        close(fd[0]);



        return;
    }
    else if(check_and_run_process(process_functions, process_names, argc, argv, function, false))   {
        
        return;
    }

    print(INVALID_INPUT_MESSAGE);
}

void print_commands() {

    for (int i = 0; i<AVAILABLE_PROCESS_F; i++) {
    
        print(process_names[i]);
        print(":");
        print(process_descriptions[i]);
        putnEnters(2);
    }
    
}

