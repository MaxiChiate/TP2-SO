#include <colours.h>
#include <stdint.h>
#include <systemCalls.h>
#include <videoDriver.h>
#include <stringPrinter.h>
#include <buffer.h>
#include <rtcDriver.h>
#include <time.h>
extern unsigned long long* register_saviour();

static char* registrerNames[16] = {
        "R15: 0x","R14: 0x", "R13: 0x","R12: 0x","R11: 0x", "R10: 0x", "R9:  0x","R8:  0x","RSI: 0x",
        "RDI: 0x", "RBP: 0x", "RDX: 0x", "RCX: 0x", "RBX: 0x", "RAX: 0x", "RSP: 0x"};
static char output[17]={'\0'};
void registerToString(uint64_t value) {
    char temp[17]; //buffer temp para ir guardando el numero
    int index = 0;
    do {
        int digit = value & 0xF;
        temp[index++] = (digit < 10) ? (char)('0' + digit) : (char)('a' + digit - 10);
        value >>= 4;
    } while (value>0);
    //la longitud de mi string depende del indice
    int length = index;

    // empiezo a dar vuelta el string
    for (int i = 0; i < length; i++) {
        output[i] = temp[length - 1 - i];
    }
    //le pongo NULL al final
    output[length] = '\0';
}

void outputRegisterCaller(uint64_t * registerArray,int i){
    registerToString(registerArray[i]);
    printRegisterDefault(output);
    printRegisterDefault("\n");
}
void registerRetriever() {
    uint64_t* registerArray=(uint64_t*)register_saviour();
    for (int i = 0; i < 16; i++) {
        printRegisterDefault(registrerNames[i]);
        outputRegisterCaller(registerArray, i);
    }
}
void registerPrintFull(uint64_t * registerInfo){
    fillScreen(0x0);
    resetPosition();
    int i=0;
    for (; i < 16; i++) {
        printRegisterDefault(registrerNames[i]);
        outputRegisterCaller(registerInfo, i);
    }
    printRegisterDefault("RIP: 0x");
    outputRegisterCaller(registerInfo, i);
}

