#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

uint64_t timer_handler(uint64_t rsp);
int ticks_elapsed();
int seconds_elapsed();
void reset_timer();
int alarmAt(int at);
int alarmAtDecimal(double at);
int alarmAtTicks(int at);
void waitTime(double at);
void sleep(unsigned long ms);

#endif
