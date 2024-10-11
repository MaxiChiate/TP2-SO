include Makefile.inc

KERNEL=kernel.bin

# Include paths for the compiler
INCLUDE_PATHS = -I./include -I./include/collections -I./include/asm

# Manually specify subdirectories for source files (hardcoded)
SOURCES = $(wildcard ./*.c) $(wildcard asm/*.c) $(wildcard collections/*.c)
SOURCES_ASM = $(wildcard asm/*.asm) $(wildcard collections/*.asm)


# Convert the source files to object files
OBJECTS = $(SOURCES:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

LOADERSRC = loader.asm
LOADEROBJECT = $(LOADERSRC:.asm=.o)
STATICLIBS =

all: $(KERNEL)

# Linking the kernel with all objects, ensure loader.o is included only once
$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

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
	rm -rf $(OBJECTS) $(OBJECTS_ASM) $(LOADEROBJECT) $(KERNEL) asm/*.o collections/*.o *.o *.bin

.PHONY: all clean
