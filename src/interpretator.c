#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "bf_node.h"
#include "interpretator.h"

#define MEM_SIZE 30000

static int jump_loop_end(struct List *bf_node_list, int index, int loop_id)
{
    struct BfNode *temp = list_at(bf_node_list, ++index);
    while (!(temp->type == LOOPR && temp->value == loop_id))
    {
        temp = list_at(bf_node_list, ++index);
    }
    return index;
}

static int jump_loop_start(struct List *bf_node_list, int index, int loop_id)
{
    struct BfNode *temp = list_at(bf_node_list, --index);
    while (!(temp->type == LOOPL && temp->value == loop_id))
    {
        temp = list_at(bf_node_list, --index);
    }
    return index;
}

static char read_one_char()
{
    char ch = getchar();

    // Clears other characters from stdin
    while (true)
    {
        char c = getchar();
        if (c == '\n' || c == EOF)
            break;
    }
    return ch;
}

void interpret_bf(struct List *bf_node_list)
{
    struct BfState state;
    state.array = malloc(MEM_SIZE);
    memset(state.array, 0, MEM_SIZE);
    state.pointer = 0;

    bool fail = false;

    for (size_t i = 0; i < bf_node_list->size && !fail; i++)
    {
        struct BfNode *node = list_at(bf_node_list, i);
        switch (node->type)
        {
        case MOVE:
            state.pointer += node->value;
            if (state.pointer < 0 || state.pointer >= MEM_SIZE)
            {
                printf("Pointer moved out of memory range, pointer:%d\n", state.pointer);
                fail = true;
            }
            break;
        case ADD:
            state.array[state.pointer] += node->value;
            break;
        case READ:
        {
            char c = read_one_char();
            state.array[state.pointer] = c;
            break;
        }
        case WRITE:
            printf("%c", state.array[state.pointer]);
            break;
        case LOOPL:
            if (state.array[state.pointer] == 0)
            {
                i = jump_loop_end(bf_node_list, i, node->value);
            }
            break;
        case LOOPR:
            if (state.array[state.pointer] != 0)
            {
                i = jump_loop_start(bf_node_list, i, node->value);
            }
            break;
        default:
            break;
        }
    }

    free(state.array);
}