# x86-to-C-interface-programming-project

### Compile and Run
Compile C version
```
gcc -c -O2 Kernel_C.c -o Kernel_C.o
```

Compile x86-64 version
```
nasm -f win64 Kernel_ASM.asm -o Kernel_ASM.o
```

Link
```
gcc Kernel_C.o Kernel_ASM.o -o runme -lm
```
Compile Summary
```
gcc -c -O2 Kernel_C.c -o Kernel_C.o
nasm -f elf64 Kernel_ASM.asm -o Kernel_ASM.o
gcc Kernel_C.o Kernel_ASM.o -o runme -lm
```

Run
```
./runme
```

S

