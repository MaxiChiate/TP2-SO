#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <stringPrinter.h>
#include <colours.h>

#include <process/process_management.h>
#include <process/ipc_management.h>
#include <memory_manager.h>
#include <semaphore.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = 	(void*)0x400000;
static void * const sampleDataModuleAddress = 	(void*)0x500000;
static void * const memoryHeapAddress =   		(void*)0x600000;

typedef int (*EntryPoint)(); 


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	cpuVendor(buffer);

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress,
		memoryHeapAddress 
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main() {

	load_idt();

	while(1)	{

		_cli();

		welcome();

		mm_init(memoryHeapAddress);
		init_semaphore_store();
		init_ipc();
		load_idt();

		int argc = 1;
		char * argv[2] = {"shell", NULL};

		sleep(2000);
		refillScreen();	

		scheduler_init((int64_t) sampleCodeModuleAddress, argc, argv); // Asumir que hace _sti

		printTextDefault("\n\nRestarting OS...\n\n", YELLOW, BLACK);

		sleep(1000);		
		fillScreen(BLACK);
    	resetPosition();
    	resetBuffer();
	}

	return 0;
}
