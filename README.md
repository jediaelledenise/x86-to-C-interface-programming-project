# Kernel Performance Comparison (C vs. x86-64 Assembly)

This repository contains two kernel implementations for calculating the Euclidean distance between two sets of 2D points: a pure C implementation and an optimized x86-64 Assembly implementation leveraging SIMD instructions.

The project is designed to compare the execution time and performance efficiency of the compiler-optimized C code against manually optimized assembly code, particularly for large vector sizes.

### Comparative execution time
| Vector size (n) | n(2^n) | C kernel time(sec) | ASM kernel time(sec) | Performance ratio (C/ASM) |
| --------------- | ------ | ------------- | --------------- | ------------------------- |
| 1048576 | 2^20 | 0.003770 | 0.001632 | 2.31 |
| 16777216 | 2^24 | 0.065210 | 0.023944 | 2.72 |
| 1073741824 | 2^30 | failed | failed | failed |
| 67108864 | 2^26 | 253728 | 097378 | 2.61 |


### Analysis of the performance of the kernels


### Short Video
- (https://youtu.be/TxApv_uO8r0)
