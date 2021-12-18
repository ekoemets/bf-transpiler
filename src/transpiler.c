#include <stdlib.h>
#include <stdbool.h>
#include "bf_node.h"
#include "transpiler.h"

static void print_printchar(FILE *out) {
    char *printchar =
        "; Outputs 1 char from address in EAX to STDOUT\n"
        "putchar:\n"
        "    ; Save registers that we modify\n"
        "    push    edx\n"
        "    push    ecx\n"
        "    push    ebx\n"
        "    push    eax\n"
        "    ; Output 1 char from EAX address\n"
        "    mov     ecx, eax \n"
        "    mov     edx, 1\n"
        "    mov     ebx, 1\n"
        "    mov     eax, 4\n"
        "    int     80h\n"
        "    ; Restore registers that we modified\n"
        "    pop     eax\n"
        "    pop     ebx\n"
        "    pop     ecx\n"
        "    pop     edx\n"
        "    ret\n";
    fprintf(out, printchar);
}

static void print_getchar(FILE* out) {
    char *getchar =
        "; Reads one char to address in EAX from STDIN and clears STDIN\n"
        "getchar:\n"
        "    ; Save registers that we modify\n"
        "    push    edx\n"
        "    push    ecx\n"
        "    push    ebx\n"
        "    push    eax\n"
        "    ; Read 1 char from STDIN\n"
        "    mov     ecx, eax\n"
        "    mov     edx, 1\n"
        "    mov     ebx, 0\n"
        "    mov     eax, 3\n"
        "    int     80h\n"
        "    ; Restore registers that we modified\n"
        "    pop     eax\n"
        "    pop     ebx\n"
        "    pop     ecx\n"
        "    pop     edx\n"
        "    ret\n\n"
        "clear_stdin:\n"
        "    push   eax\n"
        "    push   eax\n"
        "    mov    eax, esp\n"
        "clear_next:\n"
        "    call   getchar\n"
        "    cmp    byte [eax], 0Ah\n"
        "    jz     finished\n"
        "    cmp    byte [eax], -1\n"
        "    jz     finished\n"
        "    jmp    clear_next\n"
        "finished:\n"
        "    pop    eax\n"
        "    pop    eax\n"
        "    ret\n";
    fprintf(out, getchar);
}

static void print_quit(FILE* out) {
    char *quit =
        "; Exits with code from EAX\n"
        "quit:\n"
        "    mov     ebx, eax\n"
        "    mov     eax, 1\n"
        "    int     80h\n"
        "    ret\n";
    fprintf(out, quit);
}

void transpile_bf(struct List *bf_node_list, FILE* out)
{

    print_printchar(out);
    print_getchar(out);
    print_quit(out);

    fprintf(out, "SECTION .bss\n");
    fprintf(out, "memory: resb 30000\n");

    fprintf(out, "SECTION .text\n");
    fprintf(out, "global  _start\n");

    fprintf(out, "_start:\n");
    fprintf(out, "    mov eax, memory\n");
    for (size_t i = 0; i < bf_node_list->size; i++)
    {
        struct BfNode *node = list_at(bf_node_list, i);
        switch (node->type)
        {
        case MOVE:
            fprintf(out, "    add eax, %d\n", node->value);
            break;
        case ADD:
            fprintf(out, "    mov ebx, [eax]\n");
            fprintf(out, "    add ebx, %d\n", node->value);
            fprintf(out, "    mov [eax], ebx\n");
            break;
        case READ:
            fprintf(out, "    call getchar\n");
            fprintf(out, "    call clear_stdin\n");
            break;
        case WRITE:
            fprintf(out, "    call putchar\n");
            break;
        case LOOPL:
            fprintf(out, "loopl_%d:\n", node->value);
            fprintf(out, "    cmp byte[eax], 0\n");
            fprintf(out, "    jz loopr_%d\n", node->value);
            break;
        case LOOPR:
            fprintf(out, "loopr_%d:\n", node->value);
            fprintf(out, "    cmp byte[eax], 0\n");
            fprintf(out, "    jnz loopl_%d\n", node->value);
            break;
        default:
            break;
        }
    }

    fprintf(out, "_end:\n");
    fprintf(out, "    mov eax, 0\n");
    fprintf(out, "    call quit\n");
}