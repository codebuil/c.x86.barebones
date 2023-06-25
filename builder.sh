mcs -unsafe kernel.cs -o kernel.exe
mono --aot=asmonly kernel.exe
cp format.asm out.s
cat kernel.exe.s >> out.s
as -o hello.o out.s
ld link.ld hello.o -o kernel.elf

