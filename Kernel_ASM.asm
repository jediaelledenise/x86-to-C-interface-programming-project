global calculate_distance_asm
section .text

calculate_distance_asm:
    mov     r10, rsi
    mov     r11, rdx
    mov     r12, rcx
    mov     r13, r8
    mov     r14, r9
    xor     eax, eax

.loop:
    cmp     eax, edi
    jge     .done

    movss   xmm0, [r11 + rax*4]
    subss   xmm0, [r10 + rax*4]
    mulss   xmm0, xmm0

    movss   xmm1, [r13 + rax*4]
    subss   xmm1, [r12 + rax*4]
    mulss   xmm1, xmm1

    addss   xmm0, xmm1
    sqrtss  xmm0, xmm0
    movss   [r14 + rax*4], xmm0

    inc     eax
    jmp     .loop

.done:
    ret

section .note.GNU-stack
