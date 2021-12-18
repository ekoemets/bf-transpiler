; Outputs 1 char from address in EAX to STDOUT
putchar:
    ; Save registers that we modify
    push    edx
    push    ecx
    push    ebx
    push    eax
    ; Output 1 char from EAX address
    mov     ecx, eax 
    mov     edx, 1
    mov     ebx, 1
    mov     eax, 4
    int     80h
    ; Restore registers that we modified
    pop     eax
    pop     ebx
    pop     ecx
    pop     edx
    ret


; Reads one char to address in EAX from STDIN and clears STDIN
getchar:
    ; Save registers that we modify
    push    edx
    push    ecx
    push    ebx
    push    eax
    ; Read 1 char from STDIN
    mov     ecx, eax
    mov     edx, 1
    mov     ebx, 0
    mov     eax, 3
    int     80h
    ; Restore registers that we modified
    pop     eax
    pop     ebx
    pop     ecx
    pop     edx
    ret

clear_stdin:
    push   eax
    push   eax
    mov    eax, esp

clear_next:
    call   getchar
    cmp    byte [eax], 0Ah
    jz     finished
    cmp    byte [eax], -1
    jz     finished
    jmp    clear_next

finished:
    pop    eax
    pop    eax
    ret


; Exits with code from EAX
quit:
    mov     ebx, eax
    mov     eax, 1
    int     80h
    ret