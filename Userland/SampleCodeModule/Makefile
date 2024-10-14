include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin

# Include paths for the compiler
INCLUDE_PATHS = -I./include #-I./include/Testing

# Manually specify subdirectories for source files
SOURCES = $(wildcard *.c) #$(wildcard Testing/*.c)
SOURCES_ASM = $(wildcard asm/*.asm)

# Convert source files to object files
OBJECTS = $(SOURCES:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

all: $(MODULE)

# Linking module with objects
$(MODULE): $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o sampleCodeModule.elf $(OBJECTS) $(OBJECTS_ASM)

# Compile .c files into .o files using the include paths
%.o: %.c
	$(GCC) $(GCCFLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Compile .asm files into .o files
%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o Testing/*.o asm/*.o *.bin *.elf

.PHONY: all clean print
