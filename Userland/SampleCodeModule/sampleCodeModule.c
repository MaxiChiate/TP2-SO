#include <userlib.h>
#include <shell.h>
#include <processlib.h>
#include <Testing/tests.h>


int main() {

    initShell();

    char buffer[254] = {'\0'};

    while(1)	{

        read(buffer);
        getMenu(buffer);
    }

	return 0;
}


