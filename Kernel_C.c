/*
    De Castro, Jediaelle Denise
    S17B
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

double now() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

void init_vectors(int n, float *X1, float *X2, float *Y1, float *Y2) {
    for (int i = 0; i < n; i++) {
        X1[i] = (float)rand() / RAND_MAX * 10.0f;
        X2[i] = (float)rand() / RAND_MAX * 10.0f;
        Y1[i] = (float)rand() / RAND_MAX * 10.0f;
        Y2[i] = (float)rand() / RAND_MAX * 10.0f;
    }
}

int verify_results(int n, const float *C, const float *ASM) {
    for (int i = 0; i < n; i++) {
        if (fabsf(C[i] - ASM[i]) > 1e-5f) {
            printf("Mismatch at i=%d: C=%.6f ASM=%.6f\n", i, C[i], ASM[i]);
            return 0;
        }
    }
    return 1;
}

void print_first_10(const float *Z) {
    for (int i = 0; i < 10; i++) {
        printf("Z[%d] = %.6f\n", i, Z[i]);
    }
}

extern void calculate_distance_asm(int n, const float *X1, const float *X2,
                                   const float *Y1, const float *Y2, float *Z);

void calculate_distance_c(int n, const float *X1, const float *X2,
                         const float *Y1, const float *Y2, float *Z) {
    for (int i = 0; i < n; i++) {
        float dx = X2[i] - X1[i];
        float dy = Y2[i] - Y1[i];
        Z[i] = sqrtf(dx * dx + dy * dy);
    }
}

int main() {
    int test_sizes[] = {1 << 20, 1 << 24, 1 << 30};
    const int NUM_RUNS = 30;

    for (int i = 0; i < 3; i++) {
        int n = test_sizes[i];
        printf("\n============================================\n");
        printf("Vector size = %d elements (2^%d)\n", n, (int)round(log2(n)));
        printf("Running each kernel %d times...\n", NUM_RUNS);

        // Allocate aligned memory
        float *X1 = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *X2 = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *Y1 = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *Y2 = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *Z_c = (float *)ALLOC_ALIGNED(32, n * sizeof(float));
        float *Z_asm = (float *)ALLOC_ALIGNED(32, n * sizeof(float));

        if (!X1 || !X2 || !Y1 || !Y2 || !Z_c || !Z_asm) {
            printf("Error: Memory allocation failed for N=%d. Skipping.\n", n);
            FREE_ALIGNED(X1); FREE_ALIGNED(X2); FREE_ALIGNED(Y1);
            FREE_ALIGNED(Y2); FREE_ALIGNED(Z_c); FREE_ALIGNED(Z_asm);
            continue;
        }

        // Initialize vectors
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

        // Verify correctness
        printf("\n--- Correctness Check ---\n");
        printf("Result: %s\n", verify_results(n, Z_c, Z_asm) ? "PASS" : "FAIL");

        // Performance summary
        printf("\n--- Performance Summary ---\n");
        printf("C Kernel:   %.6f sec\n", avg_time_c);
        printf("ASM Kernel: %.6f sec\n", avg_time_asm);
        printf("Speedup:    %.2fx\n", avg_time_c / avg_time_asm);

        // Free memory
        FREE_ALIGNED(X1); FREE_ALIGNED(X2); FREE_ALIGNED(Y1);
        FREE_ALIGNED(Y2); FREE_ALIGNED(Z_c); FREE_ALIGNED(Z_asm);
    }

    return 0;
}
