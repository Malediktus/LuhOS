.PHONY: all, install, install-headers

DESTDIR?=.

SRCDIR?=src
INCDIR?=include
OBJDIR?=build
BINDIR?=bin

ARCH?=x86_64
ARCHDIR=$(SRCDIR)/arch/$(ARCH)

ASFLAGS=
CFLAGS=-ffreestanding -mcmodel=kernel -O0 -g -Wall -Wextra -Wpedantic -Werror -fno-exceptions -static -std=gnu11 -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin -Wno-unused-parameter -Wno-unused-variable -nostdlib -nostartfiles -nodefaultlibs -I$(INCDIR)
LDFLAGS=-lgcc

OBJECTS=$(OBJDIR)/sys/kernel.c.o \
				$(OBJDIR)/sys/port.c.o \
				$(OBJDIR)/lib/string.c.o \
				$(OBJDIR)/lib/log.c.o \
				$(OBJDIR)/arch/$(ARCH)/crti.S.o \
				$(OBJDIR)/arch/$(ARCH)/crtn.S.o \

$(OBJDIR)/%.S.o: $(SRCDIR)/%.S
	mkdir -p $(@D)
	x86_64-elf-gcc $(CFLAGS) -c -o $@ $^

$(OBJDIR)/%.c.o: $(SRCDIR)/%.c
	mkdir -p $(@D)
	x86_64-elf-gcc $(CFLAGS) -c -o $@ $^

$(BINDIR)/kernel.elf: $(OBJECTS) $(ARCHDIR)/linker.ld
	mkdir -p $(@D)
	x86_64-elf-gcc -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LDFLAGS) $(OBJECTS)

all: $(BINDIR)/kernel.elf

install: $(BINDIR)/kernel.elf
	cp $^ $(DESTDIR)

install-headers:
	mkdir -p $(DESTDIR)/include
	cp -r $(INCDIR)/* $(DESTDIR)/include
