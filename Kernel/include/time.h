#ifndef _TIME_H_
#define _TIME_H_

void timer_handler();
void nowSnakeIsOn();
int getSnakeStatus();
int ticks_elapsed();
int seconds_elapsed();
void reset_timer();
int alarmAt(int at);
int alarmAtDecimal(double at);
int alarmAtTicks(int at);
void waitTime(double at);

#endif
