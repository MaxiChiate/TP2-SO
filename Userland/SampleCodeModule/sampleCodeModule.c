#include <userlib.h>
#include <shell.h>
#include <snake.h>
#include <frontSnake.h>
int main() {
    initShell();
    char buffer[254] = {'\0'};

    do	{
        read(buffer);
        getMenu(buffer);
    }	while(1);

	return 0;
}