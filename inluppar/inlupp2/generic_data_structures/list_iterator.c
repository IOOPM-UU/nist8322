#include "list_iterator.h"
#include <stdlib.h>
#include <assert.h>
#include "linked_list_private.h"
#include <stddef.h>

struct list_iterator {
    ioopm_link_t *current;
    ioopm_list_t *list;
};

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l) {
    
    assert(l != NULL);

    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));

    iter->list = l;

    iter->current = l->first->next;

    return iter;
}

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter) {
    free(iter);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter) {
    assert(iter != NULL);

    if (iter->current == NULL) {
        return true;
    }
    else {
        return false;
    }
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter) {
    assert(iter != NULL);

    if (iter->current != NULL) {
        iter->current = iter->current->next;
    }
}

elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter) {
    assert(iter != NULL);
    
    return iter->current != NULL ? iter->current->element : int_elem(0);
}

/* int ioopm_list_iterator_remove(ioopm_list_iterator_t *iter) {
    // Optional
} */
