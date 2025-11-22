# x86-to-C-interface-programming-project

### Compile and Run (Linux)
Compile C version
```
gcc -c -O2 CalculateDistance_c.c -o CalculateDistance_c.o
```

Compile x86-64 version
```
nasm -f elf64 CalculateDistance_asm.asm -o CalculateDistance_asm.o
```

Link
```
gcc CalculateDistance_c.o CalculateDistance_asm.o -o runme -lm
```

Run
```
./runme
```



### Compile and Run (Mac)
Compile C version
```
clang -c CalculateDistance_c.c -o CalculateDistance_c.o
```

Compile x86-64 version
```
nasm -f macho64 CalculateDistance_asm.asm -o CalculateDistance_asm.o
```

Link
```
clang CalculateDistance_c.o CalculateDistance_asm.o -o runme -lm    
```

Run
```
./runme
```

