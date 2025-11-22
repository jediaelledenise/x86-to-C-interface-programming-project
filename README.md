# x86-to-C-interface-programming-project

### Compile
Compile C version
```
gcc -O2 -c CalculateDistance_c.c
```

Compile x86-64 version
```
gcc -O2 -c CalculateDistance_asm.asm
```

```
gcc -O2 -o vecdist main.o CalculateDistance_c.o CalculateDistance_asm.o -lm
```

