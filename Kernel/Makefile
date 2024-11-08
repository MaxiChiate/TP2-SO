include Makefile.inc

KERNEL=kernel.bin
KERNEL_BUDDY = kernel.bin
# Include paths for the compiler
INCLUDE_PATHS = -I./include -I./include/collections -I./include/asm -I./include/process

# Manually specify subdirectories for source files (hardcoded)
SOURCES_ALL = $(wildcard ./*.c) $(wildcard asm/*.c) $(wildcard collections/*.c) $(wildcard process/*.c)
SOURCES_ASM = $(wildcard asm/*.asm) $(wildcard collections/*.asm)


SOURCES_BUDDY = $(filter-out ./memory_manager.c, $(SOURCES_ALL))
SOURCES = $(filter-out ./buddy.c ./buddy_library.c, $(SOURCES_ALL))

# Convert the source files to object files
OBJECTS = $(SOURCES:.c=.o)
OBJECTS_ALL = $(SOURCES_ALL:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)
OBJECTS_BUDDY = $(SOURCES_BUDDY:.c=.o)


LOADERSRC = loader.asm
LOADEROBJECT = $(LOADERSRC:.asm=.o)
STATICLIBS =

buddy: $(KERNEL_BUDDY)

$(KERNEL_BUDDY): $(LOADEROBJECT) $(OBJECTS_BUDDY) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL_BUDDY) $(LOADEROBJECT) $(OBJECTS_BUDDY) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS_BUDDY) $(OBJECTS_ASM) $(STATICLIBS)


all: $(KERNEL)

# Linking the kernel with all objects, ensure loader.o is included only once
$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

# Compile .c files into .o files using the include paths variable
%.o: %.c
	$(GCC) $(GCCFLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Compile .asm files into .o files
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Compile loader.asm separately
$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# Clean the build
clean:
	rm -rf $(OBJECTS_ALL) $(OBJECTS_ASM) $(LOADEROBJECT) $(KERNEL) asm/*.o collections/*.o process/*.o *.o *.bin *.elf

.PHONY: all clean buddy
