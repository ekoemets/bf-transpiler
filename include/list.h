#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

struct List
{
    size_t size;
    size_t capacity;
    void **arr;
};

struct List *list_create(size_t initial_capacity);
void list_free(struct List *list);

void *list_at(struct List *list, int index);
void list_push(struct List *list, void *value);
void list_insert(struct List *list, void *value, int index);
void *list_pop(struct List *list);
void *list_remove(struct List *list, int index);

#endif
