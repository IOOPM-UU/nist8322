#pragma once
#include "linked_list.h"
#include <stddef.h>

typedef struct link ioopm_link_t;

struct link {
    int element;
    struct link *next;
};

struct list {
    ioopm_link_t *first;
    ioopm_link_t *last;
    size_t size;
};