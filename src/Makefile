CROSSCOMPILE?=riscv32-unknown-elf-
CC=${CROSSCOMPILE}gcc
LD=${CROSSCOMPILE}ld
OBJCOPY=${CROSSCOMPILE}objcopy
OBJDUMP=${CROSSCOMPILE}objdump
CFLAGS=-I. -O2 -ggdb -march=rv64imafdc -mabi=lp64d -Wall -mcmodel=medany -mexplicit-relocs -fno-tree-loop-distribute-patterns
CCASFLAGS=-I. -mcmodel=medany -mexplicit-relocs
LDFLAGS=-nostdlib -nostartfiles

LIB_BOOT_O=\
	spi.o \
	uart.o 

H=$(wildcard *.h */*.h)

BIN=bootloader.bin 
ELF=bootloader.elf 
ASM=bootloader.asm 
all: $(BIN)

elf: $(ELF)

asm: $(ASM)

ux00boot.o: ux00boot.c
	$(CC) $(CFLAGS) -c -o $@ $^

bootloader.elf: start.o main.o $(LIB_BOOT_O) ux00boot.o bootloader.lds
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(filter %.o,$^) -T$(filter %.lds,$^)

%.bin: %.elf
	$(OBJCOPY) -S -R .comment -R .note.gnu.build-id -O binary $^ $@

%.asm: %.elf
	$(OBJDUMP) -S $^ > $@


%.o: %.S
	$(CC) $(CFLAGS) $(CCASFLAGS) -c $< -o $@

%.o: %.c $(H)
	$(CC) $(CFLAGS) -o $@ -c $<

clean::
	rm -f */*.o */*.dtb $(BIN) $(ELF) $(ASM)
