#ifndef _REGISTERS_H
#define _REGISTERS_H
#include <stdint.h>

//guarda los registros actuales
unsigned long long* register_saviour();
//llama a register_saviour y realiza la impresión de todos los registros
void registerRetriever();
//función auxiliar de las que imprimen los registros que llama a registerToString
void outputRegisterCaller(uint64_t * registerArray,int i);
//función que pasa un registro de hexa a ascii en un buffer
void registerToString(uint64_t value);
//funión que imprime los registros durante las excepcionees
void registerPrintFull(uint64_t *registerInfo);

#endif