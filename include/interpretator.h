#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

struct BfState
{
    char *array;
    int pointer;
};

void interpret_bf(struct List *bf_node_list);

#endif