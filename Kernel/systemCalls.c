//sytemCalls.c

#include <systemCalls.h>

static int characterColor=WHITE;

syscall_wrapper syscalls[SYSCALL_COUNT] = {
	
	&sysWrite_wrapper, &sysRead_wrapper, &sysClear_wrapper, 
	&rand_wrapper, &create_process_wrapper,  &kill_process_by_pid_wrapper,
	&block_process_wrapper, &unblock_process_wrapper, &change_process_priority_wrapper,
	&waitpid_wrapper, &haltcpu_wrapper, &get_current_pid_wrapper, &give_up_cpu_wrapper,
	&wait_wrapper, &suicide_wrapper, &timeManager_wrapper
};


void call_syscall(int id, int64_t rdi, int64_t rsi, int64_t rdx, int64_t rcx, int64_t r8)	{

	if(id < SYSCALL_COUNT)	{

		syscalls[id](rdi, rsi, rdx, rcx, r8);
	}
}


void haltcpu_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	haltcpu();
}

void sysWrite_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	sysWrite((unsigned int) q, (unsigned int) w);			
}

void sysRead_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	sysRead((unsigned int) q, (char *) w, (char) e);			
}

void sysClear_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	sysClear();
}

void rand_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	rand();
}

void create_process_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	create_process(q, (int) w, (char **) e, (unsigned int) r, (bool) t);			
}

void get_current_pid_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	get_current_pid();
}

void give_up_cpu_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	give_up_cpu();
}

void kill_process_by_pid_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	kill_process_by_pid(q);			
}

void block_process_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	block_process(q);			
}

void unblock_process_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	unblock_process(q);			
}

void change_process_priority_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	change_process_priority(q, (int) w);			
}

void wait_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	wait();			
}

void waitpid_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	waitpid(q);			
}

void suicide_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	suicide();
}

void timeManager_wrapper(int64_t q, int64_t w, int64_t e, int64_t r, int64_t t)	{

	timeManager();
}


char sysWrite(unsigned int fd, unsigned int count)	{

	switch(fd)	{
	// STDOUT : Salida estandar.
		case STDOUT :	{
			char s [count+1];
			readBuffer(s, count+1);
			printTextDefault(s, characterColor, BLACK);
			break;
		}
		case CHARSTDOUT : {
			printCharDefault(readChar(), characterColor, BLACK);
			break;
		}
	// STDERR : Salida de error (salida estandar pero en rojo).
		case STDERR :	{
			char s [count+1];
			readBuffer(s, count+1);
			printTextDefault(s, RED, BLACK);
			break;
		}
	// RETURN_CHAR : Devuelve en rax el ultimo caracter del buffer.
		case RETURN_CHAR :	{ 
			return readChar();
			break;
		}
		case RETURNANDSTDOUT_CHAR :	{
			char c = readChar();
			printCharDefault(c, characterColor, BLACK);
			return c;
		}

		default :
			break;
	}

	return 0;
}

void sysRead( unsigned int fd, const char * s, char c)	{
    switch(fd)	{
		case STRING :	{
			putString(s);
			break;
		}
		case CHAR :	{
			putChar(c);
			break;
		}
		default :
			break;
	}
	
}

void sysKillBuffer()	{
	cleanBuffer();
}

void sysClear()	{
	fillScreen(BLACK);
    resetPosition();
    resetBuffer();
}

void timeManager(){
    char* time=getTime();
    printTextDefault(time, characterColor, BLACK);
}

void killOS(){
    fillScreen(BLACK);
    haltcpu();          //queda trabado acá y no puede hacer nada más

}

static unsigned long seed = 0x5A5A5A5A;

unsigned long rand() {
    unsigned long ans = 1103515245;

    char * currentTime = getTime();
    
        ans *= currentTime[1] * currentTime[0] +
               currentTime[3] * currentTime[4] +
               currentTime[6] * currentTime[7] +
               currentTime[9] * currentTime[10] +
               currentTime[12] * currentTime[13] +
               currentTime[15] * currentTime[16] * currentTime[17] * currentTime[18];

    ans += ((unsigned long) currentTime) << 11;

    seed = seed * ans;
    ans = ans ^ seed;

    ans *= ticks_elapsed();

    // Return a 31-bit positive number
    return ans & 0x7fffffff;
}


