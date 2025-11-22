#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>


double now() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

/*
    initialize values
*/
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
        float diff = fabsf(C[i] - ASM[i]);
        if (diff > 1e-5f) {
            printf("Mismatch at i=%d: C=%.6f ASM=%.6f\n", i, C[i], ASM[i]);
            return 0;  // FAIL
        }
    }
    return 1; // PASS
}


void print_first_10(const float *Z) {
    for (int i = 0; i < 10; i++) {
        printf("Z[%d] = %.6f\n", i, Z[i]);
    }
}

void calculate_distance_asm(int n, const float *X1, const float *X2,
                      const float *Y1, const float *Y2, float *Z);

/*
    computes distances between points of 2 vectors
    X1, X2, Y1, Y2, Z are single-precision float arrays
    n = number of elements
*/
void calculate_distance_c(int n, const float *X1, const float *X2,
                       const float *Y1, const float *Y2, float *Z)
{
    for (int i = 0; i < n; i++) {
        float dx = X2[i] - X1[i];
        float dy = Y2[i] - Y1[i];
        Z[i] = sqrtf(dx * dx + dy * dy);
    }
}

int main() {
    int n = 1 << 20;   // default size = 2^20
    printf("Vector size = %d elements\n", n);

    // Allocate memory
    float *X1  = aligned_alloc(32, n * sizeof(float));
    float *X2  = aligned_alloc(32, n * sizeof(float));
    float *Y1  = aligned_alloc(32, n * sizeof(float));
    float *Y2  = aligned_alloc(32, n * sizeof(float));
    float *Z_c   = aligned_alloc(32, n * sizeof(float));
    float *Z_asm = aligned_alloc(32, n * sizeof(float));

    // Initialize data
    srand(12345);
    init_vectors(n, X1, X2, Y1, Y2);

    // C kernel
    double t0 = now();
    calculate_distance_c(n, X1, X2, Y1, Y2, Z_c);
    double t1 = now();
    double time_c = t1 - t0;

    printf("\n--- C Kernel Output (first 10) ---\n");
    print_first_10(Z_c);
    printf("C Kernel Time: %.6f sec\n", time_c);

    // -----------------------------------------
    // Execute x86-64 assembly kernel
    // -----------------------------------------
    t0 = now();
    calculate_distance_asm(n, X1, X2, Y1, Y2, Z_asm);
    t1 = now();
    double time_asm = t1 - t0;

    printf("\n--- ASM Kernel Output (first 10) ---\n");
    print_first_10(Z_asm);
    printf("ASM Kernel Time: %.6f sec\n", time_asm);

    
    // verify results
    printf("\nChecking correctness...\n");
    if (verify_results(n, Z_c, Z_asm))
        printf("ASM matches C output.\n");
    else
        printf("ASM output mismatch.\n");

    // Free memory
    free(X1);
    free(X2);
    free(Y1);
    free(Y2);
    free(Z_c);
    free(Z_asm);

    return 0;
}

