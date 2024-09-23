#include <rtcDriver.h>
#include <stringPrinter.h>
#include <colours.h>
#include <stdint.h>

extern int timeParam(int argument);

char numberBuffer[20]={'\0'};

unsigned int decode(unsigned int time){
    return (time>>4)* 10 + (time & 0x0F);				//Funcion que pasa el tiempo en BCD(binary coded decimal) a un numero en decimal
}
static uint32_t numberToBase(uint64_t value, char * buffer, uint32_t base){
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }
    while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}
void buildTwoDigitsData(char* buffer, int dataTime){
    if (dataTime < 10) {
        buffer[0] = '0';
        numberToBase(dataTime, buffer+1, 10);
    } else {
        numberToBase(dataTime, buffer, 10);
    }
}
char* getTime(){
    int hours = decode(timeParam(HOURS));
    int minutes = decode(timeParam(MINUTES));
    int seconds = decode(timeParam(SECONDS));
    int day = decode(timeParam(DAY));
    int month = decode(timeParam(MONTH));
    int year = decode(timeParam(YEAR));

    //si alguno es menor que 10 le enchufo un 0 adelante

    buildTwoDigitsData(numberBuffer, hours);
    numberBuffer[2] = ':';
    buildTwoDigitsData(numberBuffer+3, minutes);
    numberBuffer[5] = ':';
    buildTwoDigitsData(numberBuffer+6, seconds);
    numberBuffer[8] = ' ';
    buildTwoDigitsData(numberBuffer+9, day);
    numberBuffer[11] = '/';
    buildTwoDigitsData(numberBuffer+12, month);
    numberBuffer[14] = '/';
    buildTwoDigitsData(numberBuffer+15, year);
    return numberBuffer;
}
