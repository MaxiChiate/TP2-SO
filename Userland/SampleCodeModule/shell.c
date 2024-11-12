#include <shell.h>

// Fd globales que solo sabe la shell:

#include <../../../Kernel/include/process/globalfd.h>


build_in_f build_in_functions[AVAILABLE_BUILDIN_F]={&help, &time, &clear, &div0, &invalidOpcode};
process_f  process_functions[AVAILABLE_PROCESS_F]={&mem,&loop, &ps, &block,&kill, &nice,&phylo};
process_f  test_functions[AVAILABLE_TESTS]={&test_processes, &test_prio, &test_sync, &test_mm};


char* build_in_names[AVAILABLE_BUILDIN_F+1]= {"help", "time", "clear", "div0", "invalidopcode", 0};
char* process_names[AVAILABLE_PROCESS_F+1]={"mem","loop", "ps", "block", "kill", "nice","phylo" ,'\0'};
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
                            "Gives information about free and used memory.",
                            "Prints the process id in a loop",
                            "Prints the status of every process in execution",
                            "Blocks a process by it's pid",
                            "Kills a process by it's pid",
                            "Changes a process' priority by it's pid",
                            "Simulates the Philosophers problem solved with semaphores",
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

void print2(char * str, unsigned int dim)   {

    int64_t args[] = {(int64_t) STDOUT_FILENO, (int64_t) str, (int64_t) dim};

    _int80(SYS_WRITE, args);
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


static void setting_stdio(int fdin, int fdout)    {

    if(fdin >= 0)   {

        set_stdin_fd(current_pid(), fdin);
    }
    if(fdout >= 0) {

        set_stdout_fd(current_pid(), fdout);
    }
}

static void default_stdio() {

    set_stdin_fd(current_pid(), STDIN_FILENO);
    set_stdout_fd(current_pid(), STDOUT_FILENO);
}

static bool check_and_run( build_in_f * f,  char ** names, int argc, char ** argv, char * function, int fdin, int fdout) {

    if(f != NULL)   {

        for(int i=0; names[i]; i++)   {
            
            if (strEquals(names[i], function)) {

                setting_stdio(fdin, fdout);

                putEnter();
                f[i](argc, argv);
                putEnter();

                default_stdio();

                return true;
            }
        }
    }

    return false;
}

static inline bool is_piping(int fdin, int fdout)   {

    return (fdin != STDIN_FILENO) || (fdout != STDOUT_FILENO);
}

static int64_t check_and_run_process( process_f * p,  char ** names, int argc, char ** argv,  char * function, int fdin, int fdout )  {

    if(p != NULL && names != NULL)   {

        for(int i=0; names[i]; i++)   {
            
            if (strEquals(names[i], function))  {
                    
                    setting_stdio(fdin, fdout);
                    
                    bool background = false;
                    int back_index = 0;

                // Busco el BACKGROUND_CHARACTER:
                    for (int i = 0; i < argc && !background; i++)   {

                        if (argv[i][0] == BACKGROUND_CHARACTER) {
                
                            background = true;
                            back_index = i;
                        }
                    }

                    if(background && is_piping(fdin, fdout))  {

                        print("\n\nError: Can not pipe background processes\n\n");
                        return 0;
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
                else  if(!is_piping(fdin, fdout))  {

                    int64_t cpid = run_process((int64_t) p[i], argc, argv, 1, true);
                    waitpid(cpid);
                }
                else    {// pipe!

                    bool is_fg = fdout == STDOUT_FILENO;

                    return spawn_process((int64_t) p[i], argc-1, argv, 1, is_fg);
                }

                putEnter();
                return 1;
            }
        }
    }

    return false;
}

static int64_t runIt(int argc, char ** argv, char * function, int fdin, int fdout) {

    // Si es build-in:

    if(check_and_run(build_in_functions, build_in_names, argc, argv, function, fdin, fdout))   {

        return 0;
    }

    // Si es programa o test:

    return check_and_run_process(process_functions, process_names, argc, argv, function, fdin, fdout) ||
        check_and_run_process(test_functions, test_names, argc, argv, function, fdin, fdout);

}


void getMenu(char * buffer, unsigned int buflen)  {
   
    buffer = stringNormalizer(buffer, buflen);

    char function[MAX_ARG_LONG]={'\0'};

    char arg1[MAX_ARG_LONG]={'\0'};
    char arg2[MAX_ARG_LONG]={'\0'};
    char arg3[MAX_ARG_LONG]={'\0'};
    char arg4[MAX_ARG_LONG]={'\0'};

    char * argv[MAX_ARGS+1] = {arg1, arg2, arg3, arg4, NULL};

    int argc = stringTrimmerBySpace(buffer, function, argv, MAX_ARG_LONG);

    if (argc>MAX_ARGS || argc < 0 || function[MAX_ARG_LONG-1]!='\0') {

        print(OVERFLOW_MESSAGE);
        return;
    }

    for(int i = argc; i<MAX_ARGS; ++i)     {

        argv[i] = NULL;
    }

    bool piping = false;
    int pipe_pos = 0;

    for (int i = 0; i < argc && !piping; i++) {
        
        if (argv[i][0] == PIPE_CHARACTER) {
            piping = true;
            pipe_pos = i;
        }
    }

    if(piping)    {

        argv[pipe_pos] = NULL;
        
        int fd[2];
        pipe(fd);

        int64_t pid1 = runIt(argc, argv, function, fd[0], STDOUT_FILENO);
        int64_t pid2 = runIt(argc, argv, function, STDIN_FILENO, fd[1]);

        if(pid1 && pid2)    {

            waitpid(pid1);
            waitpid(pid2);
        }
        else    {

            close(fd[0]);
            close(fd[1]);
        }
    }   
    else    {

        int pid;

        if(pid = runIt(argc, argv, function, STDIN_FILENO, STDOUT_FILENO))  {

            waitpid(pid);
        }
        
        return;
    }

    print(INVALID_INPUT_MESSAGE);
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

