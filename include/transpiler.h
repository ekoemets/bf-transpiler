#ifndef TRANSPILER_H
#define TRANSPILER_H

#include <stdio.h>
#include "list.h"

void transpile_bf(struct List *bf_node_list, FILE *out);

#endif