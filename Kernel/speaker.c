#include <time.h>
#include <stdint.h>

#define DO  262
#define RE  294
#define MI  330
#define FA  349
#define SOL  392
#define SI 494
#define LA 440

#define DO2  523
#define RE2  587
#define MI2  659
#define FA2  698
#define SOL2  784
#define LA2  880

uint16_t jingleBells[] = {MI, MI, MI, MI, MI, MI, MI, SOL, DO, RE, MI,
                          FA, FA, FA,  MI, MI, MI, RE, RE, RE, MI, RE, SOL};
uint16_t timeJB[]={1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 2};
uint32_t tetrisSong[]={MI2, SI, DO2, RE2, DO2, SI, LA, LA, DO2, MI2, RE2, DO2, SI, SI, DO2, RE2, MI2, DO2, LA, LA,
                       10000, RE2, FA2, LA2, SOL2, FA2, MI2, DO2, MI2, RE2, DO2, SI, SI, DO2, RE2, MI2, DO2, LA, LA, 10000};
double timeTetris[]={1, 0.5, 0.5, 1, 0.5, 0.5, 1, 0.5, 0.5, 1, 0.5, 0.5, 1, 0.5, 0.5, 1, 1, 1, 1, 1, 0.5,
                1, 0.5, 1, 0.5, 0.5, 1.5, 0.5, 1, 0.5, 0.5, 1, 0.5, 0.5, 1, 1, 1, 1, 1, 0.5};
uint32_t death2[]={SI, FA2, FA2, FA2, MI2, RE2, DO2, MI, SOL, MI, DO} ;
double deathTime[]={0.5, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 0.5, 0.5, 2};

static int initFreq=500;
static double defaultTime=0.1;


void play_sound(unsigned int frequency);
void nosound();
void playFrecuency(double time, unsigned int frequency) {
    play_sound(frequency);
    uint64_t i=(time)*250000000;
    while(i!=0){
        i--;
    }
    nosound();
}

void beep(){
    playFrecuency(defaultTime, initFreq);
    nosound();
}

void playDefault(){
    beep();
    nosound();
}

void playTetris(){
    for (int index = 0; index < 40; index++) {
        playFrecuency(0.4*timeTetris[index], tetrisSong[index]*2);
    }
    nosound();
}
void playJingleBells(){
    for (int index = 0; index < 23; index++){
        playFrecuency(0.3*timeJB[index], jingleBells[index]*2);
    }
    nosound();
}
void playMario(){
    for (int index = 0; index < 10; index++) {
        playFrecuency(0.35*deathTime[index], death2[index]);
    }
    nosound();
}

