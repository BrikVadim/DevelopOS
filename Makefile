CC      = gcc
CFLAGS  = -Wall -fno-builtin -nostdinc -nostdlib -ggdb3
LD      = ld

OBJFILES = \
	loader.o  \
	common/printf.o  \
	common/screen.o  \
	common/pci.o  \
	common/string.o \
	kernel.o \
	common/callrealmode.o \
	common/callrealmode_asm.o \
	common/descriptor.o 

image:
	@echo "Создание образа (hdd.img)..."
	@dd if=/dev/zero of=./hdd.img bs=512 count=16065 1>/dev/null 2>&1

	@echo "Создание загрузочного FAT32 раздела..."
	@losetup /dev/loop1 ./hdd.img
	@(echo c; echo u; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | fdisk /dev/loop1 1>/dev/null 2>&1 || true

	@echo "Монтирование /dev/loop2..."
	@losetup /dev/loop2 ./hdd.img \
    --offset    `echo \`fdisk -lu /dev/loop1 | sed -n 10p | awk '{print $$3}'\`*512 | bc` \
    --sizelimit `echo \`fdisk -lu /dev/loop1 | sed -n 10p | awk '{print $$4}'\`*512 | bc`
	@losetup -d /dev/loop1

	@echo "Форматирование раздела..."
	@mkdosfs /dev/loop2

	@echo "Копирование файлов ядра и grub на раздел..."
	@mkdir -p tempdir
	@mount /dev/loop2 tempdir
	@mkdir tempdir/boot
	@cp -r grub tempdir/boot/
	@cp kernel.bin tempdir/
	@sleep 1
	@umount /dev/loop2
	@rm -r tempdir
	@losetup -d /dev/loop2

	@echo "Установка GRUB..."
	@echo "device (hd0) hdd.img \n \
	       root (hd0,0)         \n \
	       setup (hd0)          \n \
	       quit\n" | grub --batch 1>/dev/null
	@echo "Готово!"

all: kernel.bin
rebuild: clean all
.s.o:
	as -I include -o $@ $<
.c.o:
	$(CC) -Iinclude $(CFLAGS) -o $@ -c $<
kernel.bin: $(OBJFILES)
	$(LD) -T linker.ld -o $@ $^
	cp $@ $@.dbg
	strip $@
clean:
	rm -f $(OBJFILES) hdd.img kernel.bin
