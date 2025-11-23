/*
    De Castro, Jediaelle Denise
    S17B

    This program compares the performance of C and x86-64 assembly implementations
    for calculating Euclidean distances.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
#include <malloc.h>
#define ALLOC_ALIGNED(alignment, size) _aligned_malloc(size, alignment)
#define FREE_ALIGNED(ptr) _aligned_free(ptr)
#else
#define ALLOC_ALIGNED(alignment, size) aligned_alloc(alignment, size)
#define FREE_ALIGNED(ptr) free(ptr)
#endif

/*
    Returns the current time in seconds with nanosecond precision
    Uses monotonic clock for accurate performance measurements
*/
double now() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

/*
    Initializes input vectors with random floating-point values
    Each coordinate is randomly generated in the range [0, 10]
    Parameters:
    n - number of points to generate
    X1, X2 - x-coordinates of first and second points
    Y1, Y2 - y-coordinates of first and second points
*/
void init_vectors(int n, float *X1, float *X2, float *Y1, float *Y2) {
    for (int i = 0; i < n; i++) {
        X1[i] = (float)rand() / RAND_MAX * 10.0f;
        X2[i] = (float)rand() / RAND_MAX * 10.0f;
        Y1[i] = (float)rand() / RAND_MAX * 10.0f;
        Y2[i] = (float)rand() / RAND_MAX * 10.0f;
    }
}

/*
    Verifies that C and assembly implementations produce identical results
    Compares outputs element-by-element with a small tolerance for floating-point error

    Parameters:
    n - number of elements to compare
    C - output array from C implementation
    ASM - output array from assembly implementation

    Returns: 1 if results match, 0 if there's a mismatch
*/
int verify_results(int n, const float *C, const float *ASM) {
    for (int i = 0; i < n; i++) {
        if (fabsf(C[i] - ASM[i]) > 1e-5f) {
            printf("Mismatch at i=%d: C=%.6f ASM=%.6f\n", i, C[i], ASM[i]);
            return 0;
        }
    }
    return 1;
}

/*
    Prints the first 10 elements of the output array

    Parameters:
    Z - output array containing calculated distances
*/
void print_first_10(const float *Z) {
    for (int i = 0; i < 10; i++) {
        printf("Z[%d] = %.6f\n", i, Z[i]);
    }
}

/*
    External assembly function declaration
    Calculates the Euclidean distance in x86-64 assembly
    Formula: Z[i] = sqrt((X2[i] - X1[i])^2 + (Y2[i] - Y1[i])^2)
*/
extern void calculate_distance_asm(int n, const float *X1, const float *X2,
                                   const float *Y1, const float *Y2, float *Z);

/*
    Calculates the Euclidean distance in C

    Parameters:
    n - number of point pairs to process
    X1, X2 - x-coordinates of first and second points
    Y1, Y2 - y-coordinates of first and second points
    Z - output array to store calculated distances
 */
void calculate_distance_c(int n, const float *X1, const float *X2,
                         const float *Y1, const float *Y2, float *Z) {
    for (int i = 0; i < n; i++) {
        float dx = X2[i] - X1[i];
        float dy = Y2[i] - Y1[i];
        Z[i] = sqrtf(dx * dx + dy * dy);
    }
}

int main() {
    // Test vector sizes: 2^20, 2^24, 2^30
    // fallback to 2^26 if memory allocation fails
    int test_sizes[] = {1 << 20, 1 << 24, 1 << 30, 1 << 26};
    const int NUM_RUNS = 30;
    int large_size_found = 0;

    for (int i = 0; i < 4; i++) {
        // Skip 2^26 if 2^30 already succeeded
        if (i == 3 && large_size_found) {
            continue;
        }

        int n = test_sizes[i];
        printf("\n============================================\n");
        printf("Vector size = %d elements (2^%d)\n", n, (int)round(log2(n)));
        printf("Running each kernel %d times...\n", NUM_RUNS);

        float *X1 = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *X2 = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *Y1 = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *Y2 = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *Z_c = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *Z_asm = (float *)ALLOC_ALIGNED(32, n * sizeof(float));

        // Check if memory allocation was successful
        if (!X1 || !X2 || !Y1 || !Y2 || !Z_c || !Z_asm) {
            printf("Error: Memory allocation failed for N=%d (2^%d).\n", n, (int)round(log2(n)));
            
            if (i == 2) {
                printf("Attempting fallback to 2^26...\n");
            } else {
                printf("Skipping.\n");
            }
            
            FREE_ALIGNED(X1); FREE_ALIGNED(X2); FREE_ALIGNED(Y1);
            FREE_ALIGNED(Y2); FREE_ALIGNED(Z_c); FREE_ALIGNED(Z_asm);
            continue;
        }

        // Mark that 2^30 successfully allocated
        if (i == 2) {
            large_size_found = 1;
        }

        srand(12345);
        init_vectors(n, X1, X2, Y1, Y2);

        // Time C kernel (30 runs)
        double total_time_c = 0.0;
        for (int run = 0; run < NUM_RUNS; run++) {
            double t0 = now();
            calculate_distance_c(n, X1, X2, Y1, Y2, Z_c);
            double t1 = now();
            total_time_c += (t1 - t0);
        }
        double avg_time_c = total_time_c / NUM_RUNS;

        printf("\n--- C Kernel Results ---\n");
        printf("First 10 outputs:\n");
        print_first_10(Z_c);
        printf("Average Time (30 runs): %.6f sec\n", avg_time_c);

        // Time ASM kernel (30 runs)
        double total_time_asm = 0.0;
        for (int run = 0; run < NUM_RUNS; run++) {
            double t0 = now();
            calculate_distance_asm(n, X1, X2, Y1, Y2, Z_asm);
            double t1 = now();
            total_time_asm += (t1 - t0);
        }
        double avg_time_asm = total_time_asm / NUM_RUNS;

        printf("\n--- ASM Kernel Results ---\n");
        printf("First 10 outputs:\n");
        print_first_10(Z_asm);
        printf("Average Time (30 runs): %.6f sec\n", avg_time_asm);

        printf("\n--- Correctness Check ---\n");
        if (verify_results(n, Z_c, Z_asm)) {
            printf("C Kernel output: CORRECT (reference)\n");
            printf("x86-64 Kernel output: CORRECT (matches C)\n");
        } else {
            printf("x86-64 Kernel output: INCORRECT (does not match C)\n");
        }

        printf("\n--- Performance Summary ---\n");
        printf("C Kernel:          %.6f sec\n", avg_time_c);
        printf("ASM Kernel:        %.6f sec\n", avg_time_asm);
        printf("Performance Ratio: %.2fx ", avg_time_c / avg_time_asm);
        if (avg_time_c > avg_time_asm) {
            printf("(ASM is %.2fx faster)\n", avg_time_c / avg_time_asm);
        } else {
            printf("(C is %.2fx faster)\n", avg_time_asm / avg_time_c);
        }

        FREE_ALIGNED(X1); FREE_ALIGNED(X2); FREE_ALIGNED(Y1);
        FREE_ALIGNED(Y2); FREE_ALIGNED(Z_c); FREE_ALIGNED(Z_asm);
    }

    return 0;
}
