#include <userlib.h>
#include <shell.h>
#include <processlib.h>
#include <Testing/tests.h>


int main() {

    initShell();

    char buffer[MAX_ARG_LONG*MAX_ARGS + 1] = {'\0'};

    unsigned int len = MAX_ARG_LONG*MAX_ARGS + 1;

    while(1)	{

        read(buffer, len);
        getMenu(buffer, len);
    }

	return 0;
}


