//sytemCalls.c

#include <systemCalls.h>

void sysClear()	{
	fillScreen(BLACK);
    resetPosition();
    resetBuffer();
}

void timeManager(){
    char* time=getTime();
    printTextDefault(time, WHITE, BLACK);
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


