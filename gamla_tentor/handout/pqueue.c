#include <stdlib.h>
#include <stdbool.h>
#include "pqueue.h"

struct link {
 void *data_item;
 link_t *next;

};

struct pqueue {
    link_t *first;
    get_priority_fn *get_priority;

};

static link_t *link_create() {
  link_t *link = calloc(1, sizeof(link_t));
  link->data_item = NULL;
  link->next = NULL;
  return link;
}

pqueue_t *pqueue_create(get_priority_fn *get_priority) {
  pqueue_t *pqueue = calloc(1, sizeof(pqueue_t));
  link_t *first = link_create();
  pqueue->get_priority = get_priority;
  pqueue->first = first; 
};

void pqueue_destroy(pqueue_t *pqueue) {
  while (pqueue != NULL) {
    pqueue_dequeue(pqueue);
  }
  free(pqueue);
};

void pqueue_enqueue(pqueue_t *pqueue, void *item) {
  if (pqueue == NULL) {
    pqueue->first->data_item = item;
  }
  if (pqueue->first->next == NULL) {
    pqueue->first->next->data_item = item;
  }
  
};

void *pqueue_dequeue(pqueue_t *pqueue) {
  // TODO
};
