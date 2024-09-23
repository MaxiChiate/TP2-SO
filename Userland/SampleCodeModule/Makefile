include ../Makefile.inc

GCC=gcc
LD=ld
AR=ar
ASM=nasm
GCCFLAGS=-g -m64 -fno-exceptions -fno-asynchronous-unwind-tables -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -mno-red-zone -Wall -ffreestanding -nostdlib -fno-common -std=c99
ARFLAGS=rvs
ASMFLAGS=-felf64
LDFLAGS=--warn-common -z max-page-size=0x1000
MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS)  $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)
	#$(GCC) $(GCCFLAGS) -I./include -w -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../0000-sampleCodeModule.elf


%.o: %.c
	$(GCC) $(GCCFLAGS) -w -Iinclude -c $< -o $@


%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@


clean:
	rm -rf *.o

.PHONY: all clean print
