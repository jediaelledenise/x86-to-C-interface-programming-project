# x86-to-C-interface-programming-project

### Compile and Run
Use 'nasmpath' from downloaded NASM
Compile C version
```
gcc -c Kernel_C.c -o Kernel_C.o
```
Compile x86-64 version
```
nasm -f win64 Kernel_ASM.asm -o Kernel_ASM.obj
```
Link
```
gcc Kernel_C.o Kernel_ASM.obj -o run.exe -lm
```

Compile Summary
```
gcc -c Kernel_C.c -o Kernel_C.o
nasm -f win64 Kernel_ASM.asm -o Kernel_ASM.obj
gcc Kernel_C.o Kernel_ASM.obj -o run.exe -lm
```

Run
```
./runme
```
