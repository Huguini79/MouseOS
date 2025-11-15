clear

nasm -f elf32 src/kernel/kernel.asm -o build/kernel.asm.o
nasm -f elf32 src/io/io.asm -o build/io.asm.o

gcc -I./src -m32 -nostdlib -ffreestanding -c src/kernel/kernel.c -o build/kernel.o
gcc -I./src -m32 -nostdlib -ffreestanding -c src/vga/vga.c -o build/vga.o
gcc -I./src -m32 -nostdlib -ffreestanding -c src/mouse/mouse.c -o build/mouse.o

ld -m elf_i386 -T src/linker/linker.ld -o iso/boot/kernel build/kernel.asm.o build/io.asm.o build/kernel.o build/vga.o build/mouse.o

grub-mkrescue -o bin/MouseOS.iso iso

qemu-system-x86_64 bin/MouseOS.iso
