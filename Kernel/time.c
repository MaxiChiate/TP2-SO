#include <time.h>
#include <videoDriver.h>
#include <process/process_management.h>

static unsigned long ticks = 0;

uint64_t timer_handler(uint64_t rsp) {
	ticks++;
	return get_scheduler_status() ? schedule(rsp) : rsp;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 19;
}

int alarmAt(int at)	{
	return seconds_elapsed() % at == 0 && ticks % 19 == 0 && seconds_elapsed()!=0 ; 
}
int alarmAtTicks(int at){
    return ticks%at==0;
}
