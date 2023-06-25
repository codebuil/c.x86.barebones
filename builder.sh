printf "\ec\e[44;37m\n"
mcs $1 -o $1.exe
mono --aot=asmonly  $1.exe
cp format.asm out.s
cat $1.exe.s >> out.s
as -o out.o out.s
ld link.ld out.o -o out.elf

