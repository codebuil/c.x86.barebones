printf "\ec\e[44;37m\n"
rm *.o
rm *.elf
rm *.img
rm *.exe
rm *.dll
nasm -felf32 -o boot.o boot.S
gcc -c kernel.c -o kernel.o -nostdlib
gcc link.ld boot.o kernel.o -o kernel.elf -nostdlib
objcopy kernel.elf -O binary kernel.bin
qemu-system-i386 -kernel kernel.elf
