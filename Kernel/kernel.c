#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <stringPrinter.h>
#include <buffer.h>
#include <colours.h>

//testing mm:
//
#include <test_mm.h>
#include <memory_manager.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = 	(void*)0x400000;
static void * const sampleDataModuleAddress = 	(void*)0x500000;
static void * const memoryHeapAddress = 	(void*)0x600000;

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

int main(){

	load_idt();


	//mm_init(memoryHeapAddress, MAX_BLOCKS);
	printCharDefault('?', WHITE, BLACK);

	uint64_t args[4] = {314, MAX_BLOCKS, memoryHeapAddress, 0x00}; 
	
	test_mm(3, args);

//	((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}
