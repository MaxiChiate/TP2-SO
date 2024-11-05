// systemCallsWrappers.c

#include <systemCallsWrappers.h>

syscall_wrapper syscalls[SYSCALL_COUNT] = {
	
    &sysWrite_wrapper, &sysRead_wrapper, &sysClear_wrapper, 
    &rand_wrapper, &create_process_wrapper, &kill_process_by_pid_wrapper,
    &block_process_wrapper, &unblock_process_wrapper, &change_process_priority_wrapper,
    &waitpid_wrapper, &haltcpu_wrapper, &get_current_pid_wrapper, &give_up_cpu_wrapper,
    &wait_wrapper, &suicide_wrapper, &timeManager_wrapper,
	&kill_sem_wrapper, &up_wrapper, &down_wrapper, &is_sem_alive_wrapper, &new_sem_wrapper
};

void call_syscall(int id, int64_t * args) {
    
	if (id < SYSCALL_COUNT) {

        syscalls[id](args);
    }
}

void haltcpu_wrapper(int64_t *args) {
    
	haltcpu();
}

void sysWrite_wrapper(int64_t *args) {
    
	sysWrite((unsigned int) args[0], (unsigned int) args[1]);            
}

void sysRead_wrapper(int64_t *args) {
    
	sysRead((unsigned int) args[0], (char *) args[1], (char) args[2]);           
}

void sysClear_wrapper(int64_t *args) {
    
	sysClear();
}

void rand_wrapper(int64_t *args) {
    
	rand();
}

void create_process_wrapper(int64_t *args) {
    
	create_process(args[0], (int) args[1], (char **) args[2], (unsigned int) args[3], (bool) args[4]);            
}

void get_current_pid_wrapper(int64_t *args) {
    
	get_current_pid();
}

void give_up_cpu_wrapper(int64_t *args) {
    
	give_up_cpu();
}

void kill_process_by_pid_wrapper(int64_t *args) {
    
	kill_process_by_pid(args[0]);            
}

void block_process_wrapper(int64_t *args) {
    
	block_process(args[0]);            
}

void unblock_process_wrapper(int64_t *args) {
    
	unblock_process(args[0]);            
}

void change_process_priority_wrapper(int64_t *args) {
    
	change_process_priority(args[0], (int) args[1]);            
}

void wait_wrapper(int64_t *args) {
    
	wait();            
}

void waitpid_wrapper(int64_t *args) {
    
	waitpid(args[0]);            
}

void suicide_wrapper(int64_t *args) {
    
	suicide();
}

void timeManager_wrapper(int64_t *args) {

    timeManager();
}

void new_sem_wrapper(int64_t *args)	{

	new_sem((uint8_t) args[0]);
}
        
void is_sem_alive_wrapper(int64_t *args)	{

	is_sem_alive((uint8_t) args[0]);
}

void kill_sem_wrapper(int64_t *args)	{

	kill_sem((uint8_t) args[0]);
}

void up_wrapper(int64_t *args)	{

	up((uint8_t) args[0]);
}

void down_wrapper(int64_t *args)	{

	down((uint8_t) args[0]);
}