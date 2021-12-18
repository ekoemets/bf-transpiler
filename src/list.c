#include <stdio.h>
#include <string.h>
#include "list.h"

static void resize_list(struct List *list, size_t new_size)
{
    list->arr = realloc(list->arr, new_size * sizeof(void *));
    if (!list->arr)
    {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    list->capacity = new_size;
}

struct List *list_create(size_t initial_capacity)
{
    struct List *list = malloc(sizeof(struct List));
    if (!list)
    {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    list->size = 0;
    list->capacity = initial_capacity;
    list->arr = malloc(initial_capacity * sizeof(void *));
    if (!list->arr)
    {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    return list;
}

void list_free(struct List *list)
{
    for (size_t i = 0; i < list->size; i++)
    {
        free(list->arr[i]);
    }
    free(list->arr);
    list->arr = NULL;
    free(list);
    list = NULL;
}

void *list_at(struct List *list, int index)
{
    if (index >= 0 && index < list->size)
    {
        return list->arr[index];
    }
    return NULL;
}

void list_push(struct List *list, void *value)
{
    if (list->size >= list->capacity)
    {
        resize_list(list, list->size * 2);
    }

    list->arr[list->size] = value;
    list->size++;
}

void *list_pop(struct List *list)
{
    return list_remove(list, list->size - 1);
}

void *list_remove(struct List *list, int index)
{
    if (index >= 0 && index < list->size)
    {
        void *value = list->arr[index];
        if (index != list->size - 1)
        {
            void **value_addr = list->arr + index;
            size_t n = list->size - index - 1;
            memcpy(value_addr, value_addr + 1, n * sizeof(void *));
        }
        list->size--;

        if (list->size < list->capacity / 2)
        {
            resize_list(list, list->capacity / 2);
        }
        return value;
    }

    return NULL;
};
