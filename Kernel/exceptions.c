#include <stdint.h>
#include <colours.h>
#include <videoDriver.h>
#include <stringPrinter.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6

#define ZERO_EXCEPTION_MESSAGE ("Excepcion numero 0: Divide por cero\n")
#define INVALID_OPCODE_MESSAGE ("Excepcion numero 1: Operacion Invalida\n")

static void zero_division();
static void invalid_opcode();


void exceptionDispatcher(int exception) {
    
    switch (exception) {
        case ZERO_EXCEPTION_ID: {
            zero_division();
            break;
    }
        case INVALID_OPCODE_EXCEPTION_ID: {   
            invalid_opcode();
            break;
    }
        default:
            break;
    }
}


static void zero_division() {

    printTextDefault(ZERO_EXCEPTION_MESSAGE, RED, BLACK);
}

static void invalid_opcode(){
    
    printTextDefault(INVALID_OPCODE_MESSAGE, RED, BLACK);
}