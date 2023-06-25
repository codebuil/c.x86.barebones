printf "\ec\e[44;37m\n"
mcs $1 -o $1.exe
mkbundle -o out.a $1.exe

