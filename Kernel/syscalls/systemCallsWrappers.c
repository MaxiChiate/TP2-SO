// systemCallsWrappers.c

#include <systemCallsWrappers.h>

syscall_wrapper syscalls[SYSCALL_COUNT] = {
	
    &set_stdout_fd_wrapper, &set_stdin_fd_wrapper, &get_stdout_fd_wrapper, &get_stdin_fd_wrapper, &set_stdio_wrapper, 
	&standard_write_wrapper, &standard_read_wrapper, &consume_stdin_wrapper,
	&write_wrapper, &read_wrapper, &read_all_wrapper, &open_wrapper, &close_wrapper, &pipe_wrapper, &sysClear_wrapper, 
    &rand_wrapper, &create_process_wrapper, &kill_process_by_pid_wrapper,
    &block_process_wrapper, &unblock_process_wrapper, &change_process_priority_wrapper,
    &waitpid_wrapper, &haltcpu_wrapper, &get_current_pid_wrapper, &give_up_cpu_wrapper,
    &wait_wrapper, &suicide_wrapper, &ps_wrapper, &timeManager_wrapper, &sleep_wrapper,
	&kill_sem_wrapper, &up_wrapper, &down_wrapper, &is_sem_alive_wrapper, &new_sem_wrapper,
	&mm_malloc_wrapper, &mm_free_wrapper, &mm_mem_wrapper
};


void call_syscall(int id, int64_t * args) {
    
	if (id < SYSCALL_COUNT) {

        syscalls[id](args);
    }
}

void haltcpu_wrapper(int64_t *args) {
    
	haltcpu();
}

void write_wrapper(int64_t *args) {
    
	write((unsigned int) args[0], (char *) args[1], (unsigned int) args[2]);
}

void read_wrapper(int64_t *args) {
    
	read((int) args[0], (char *) args[1], (int) args[2]);
}

void read_all_wrapper(int64_t * args)	{

	read_all((int) args[0], (char*) args[1]);
}

void open_wrapper(int64_t * args) {

	open((rw_flags_t) args[0]);
}

void close_wrapper(int64_t * args) {

	close((int) args[0]);
}

void pipe_wrapper(int64_t * args) {

	int fildes[2] = {(unsigned int) args[0], (unsigned int) args[1]};
	pipe(fildes);
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

void ps_wrapper(int64_t * args) {

	get_ps((ps_t **) args[0]);
}

void timeManager_wrapper(int64_t *args) {

    timeManager();
}

void sleep_wrapper(int64_t * args) {

	sleep((unsigned long) args[0]);
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

void mm_malloc_wrapper(int64_t * args)	{
 
  	mm_malloc((uint32_t) args[0]);
}

void mm_free_wrapper(int64_t * args)	{

 	mm_free((void *) args[0]);
}

void mm_mem_wrapper(int64_t * args)	{

	mm_mem();
}

void set_stdout_fd_wrapper(int64_t * args)	{

	set_stdout_fd(args[0], (int) args[1]);
}

void set_stdin_fd_wrapper(int64_t * args)	{

    set_stdin_fd(args[0], (int) args[1]);
}

void set_stdio_wrapper(int64_t * args)	{

	set_stdio(args[0], (int) args[1], (int) args[2]);
}

void standard_write_wrapper(int64_t * args)	{

	standard_write((char *) args[0], (int) args[1]);
}

void standard_read_wrapper(int64_t * args)	{

	standard_read((char *) args[0], (int) args[1]);
}
    
void consume_stdin_wrapper(int64_t * args)	{

	consume_stdin();
}

void get_stdout_fd_wrapper(int64_t * args)	{

	get_stdout_fd(args[0]);
}
    
void get_stdin_fd_wrapper(int64_t * args)	{

	get_stdin_fd(args[0]);
}