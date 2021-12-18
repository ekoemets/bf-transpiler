#ifndef BF_NODE_H
#define BF_NODE_H

#include "list.h"

enum BfNodeType
{
    ADD,
    MOVE,
    LOOPL,
    LOOPR,
    WRITE,
    READ
};

struct BfNode
{
    enum BfNodeType type;
    int value;
};

struct BfNode *bf_node_create(enum BfNodeType type, int value);
void bf_node_free(void *node);

#endif
