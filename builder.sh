printf "\ec\e[44;37m\n"
mcs Program.cs -o $1
mono --aot=asmonly  $1
cat $1.s
