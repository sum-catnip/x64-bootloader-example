mkdir -p bin

# compile the bootloader
nasm -f bin bootloader.asm -o bin/bootloader.bin 

# compile program and link it
nasm centry.asm -f elf64 -o bin/centry.o 
gcc -ffreestanding -c program.c -o bin/program.o -march=x86-64
ld -o bin/program.bin -T link.ld --oformat binary bin/centry.o bin/program.o
cat bin/bootloader.bin bin/program.bin > bin/image