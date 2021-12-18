#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"
#include "list.h"
#include "bf_node.h"

struct List *parse_bf_str(char *str)
{
    // Each str symbol will be parsed to BfNode
    struct List *list = list_create(10);

    // Each loop [] pair gets it's own unique index
    struct List *loop_ids = list_create(2);
    int loop_counter = 0;

    // Fail flag
    bool fail = false;

    char c;
    int i = 0;
    while ((c = str[i]))
    {
        struct BfNode *node;
        bool unknown = false;
        switch (c)
        {
        case '>':
            node = bf_node_create(MOVE, 1);
            break;
        case '<':
            node = bf_node_create(MOVE, -1);
            break;
        case '+':
            node = bf_node_create(ADD, 1);
            break;
        case '-':
            node = bf_node_create(ADD, -1);
            break;
        case '.':
            node = bf_node_create(WRITE, 0);
            break;
        case ',':
            node = bf_node_create(READ, 0);
            break;
        case '[':
        {
            int *id = malloc(sizeof(int));
            *id = loop_counter;
            list_push(loop_ids, id);
            node = bf_node_create(LOOPL, loop_counter);
            loop_counter++;
            break;
        }
        case ']':
        {
            int *value = list_pop(loop_ids);
            if (value == NULL)
            {
                printf("Found loop end ] without matching start, at: %d\n", i);
                fail = true;
                break;
            }
            node = bf_node_create(LOOPR, *value);
            free(value);
            break;
        }
        default:
            unknown = true;
            break;
        }
        // Break out when fail flag is set
        if (fail)
            break;

        if (!unknown)
        {
            list_push(list, node);
        }
        i++;
    }

    if (loop_ids->size > 0)
    {
        printf("Found more loop starts [ than ends\n");
        fail = true;
    }

    list_free(loop_ids);
    if (fail)
    {
        list_free(list);
        return NULL;
    }

    return list;
}

void optimize_bf_list(struct List *list)
{
    if (list->size < 2)
        return;

    int i = 0;

    while (i < list->size - 1)
    {
        struct BfNode *current = list_at(list, i);
        struct BfNode *next = list_at(list, i + 1);

        // Join MOVE nodes together
        if (current->type == MOVE && next->type == MOVE)
        {
            current->value += next->value;
            void *node = list_remove(list, i + 1);
            bf_node_free(node);
            continue;
        }

        // Join ADD nodes together
        if (current->type == ADD && next->type == ADD)
        {
            current->value += next->value;
            void *node = list_remove(list, i + 1);
            bf_node_free(node);
            continue;
        }
        i++;
    }
}