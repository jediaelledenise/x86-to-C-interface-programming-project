# Kernel Performance Comparison (C vs. x86-64 Assembly)

This repository contains two kernel implementations for calculating the Euclidean distance between two sets of 2D points: a pure C implementation and an optimized x86-64 Assembly implementation leveraging SIMD instructions.

The project is designed to compare the execution time and performance efficiency of the compiler-optimized C code against manually optimized assembly code, particularly for large vector sizes.

### Comparative and Analysis of the performance of the kernels
| Vector size (n) | n(2^n) | C kernel time(sec) | ASM kernel time(sec) | Performance ratio (C/ASM) |
| --------------- | ------ | ------------- | --------------- | ------------------------- |
| 1,048,576 | 2^20 | 0.003888 | 0.001677 | 2.32 |
| 16,777,216 | 2^24 | 0.069578 | 0.025009 | 2.78 |
| 1,073,741,824 | 2^30 | failed | failed | failed |
| 67,108,864 | 2^26 (fallback) | 0.285679 | 0.107190 | 2.67 |

The x86-64 assembly kernel produced identical, element-by-element correct results compared to the C version across all tested vector sizes, confirming its exact functional match.

In terms of performance, the assembly kernel was consistently faster than the C kernel. This superiority, which grew significantly with larger vector sizes, stems from its direct use of scalar SIMD floating-point instructions, which minimizes overhead and memory traffic. In contrast, the C version relies on compiler optimizations that can be less efficient in register usage, magnifying the assembly's scalability and efficiency advantage for large inputs.


### Short Video
- (https://youtu.be/qgWMjWXj2QY)
