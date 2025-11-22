# Kernel Performance Comparison (C vs. x86-64 Assembly)

This repository contains two kernel implementations for calculating the Euclidean distance between two sets of 2D points: a pure C implementation and an optimized x86-64 Assembly implementation leveraging SIMD instructions.

The project is designed to compare the execution time and performance efficiency of the compiler-optimized C code against manually optimized assembly code, particularly for large vector sizes.

### Comparative execution time and Analysis of the performance of the kernels
| Vector size (n) | n(2^n) | C kernel time | ASM kernel time | Performance ratio (C/ASM) |
| --------------- | ------ | ------------- | --------------- | ------------------------- |
|     1048576     | 2^20 | 0.003870 | 0.001538 | ------------------------- |
| --------------- | ------ | ------------- | --------------- | ------------------------- |
| --------------- | ------ | ------------- | --------------- | ------------------------- |
| --------------- | ------ | ------------- | --------------- | ------------------------- |




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
run.exe
```
