#include <stdlib.h>
#include <stdbool.h>
#include "bf_node.h"
#include "transpiler.h"

void transpile_bf(struct List *bf_node_list, FILE* out)
{
    fprintf(out, "%%include  'functions.asm'\n");
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