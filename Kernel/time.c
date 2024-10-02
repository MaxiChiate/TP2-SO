#include <time.h>
#include <videoDriver.h>

static unsigned long ticks = 0;
static int snakeIsOn=0;

void timer_handler() {
	ticks++;
    if (!snakeIsOn)
        blink();
}
int getSnakeStatus(){
    return snakeIsOn;
}
void nowSnakeIsOn(){
    snakeIsOn=1;
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
