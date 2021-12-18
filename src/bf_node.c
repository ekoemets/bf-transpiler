#include "bf_node.h"
#include <stdlib.h>

struct BfNode *bf_node_create(enum BfNodeType type, int value)
{
    struct BfNode *node = malloc(sizeof(struct BfNode));
    node->type = type;
    node->value = value;
    return node;
};

void bf_node_free(void *value)
{
    if (value == NULL)
    {
        return;
    }

    struct BfNode *node = value;
    free(node);
}