; De Castro, Jediaelle Denise | S17B
global calculate_distance_asm
section .text

calculate_distance_asm:
    push r12
    push r13
    push r14
    push r15
    
    mov  r10, rcx               ; n
    mov  r11, rdx               ; X1
    mov  r12, r8                ; X2
    mov  r13, r9                ; Y1
    mov  r14, [rsp + 72]        ; Y2
    mov  r15, [rsp + 80]        ; Z
    xor  rax, rax               ; i = 0

.loop:
    cmp  rax, r10
    jge  .done

    movss xmm0, [r12 + rax*4]   ; X2[i]
    subss xmm0, [r11 + rax*4]   ; dx = X2[i] - X1[i]
    mulss xmm0, xmm0            ; dx²

    movss xmm1, [r14 + rax*4]   ; Y2[i]
    subss xmm1, [r13 + rax*4]   ; dy = Y2[i] - Y1[i]
    mulss xmm1, xmm1            ; dy²

    addss xmm0, xmm1            ; dx² + dy²
    sqrtss xmm0, xmm0           ; sqrt(dx² + dy²)
    movss [r15 + rax*4], xmm0   ; Z[i] = result

    inc rax
    jmp .loop

.done:
    pop r15
    pop r14
    pop r13
    pop r12
    ret
