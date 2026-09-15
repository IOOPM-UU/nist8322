#include <stddef.h>
#include <stdlib.h>
#include "hash_table.h"

typedef struct entry entry_t;

struct entry
{
  char *key;     // holds the key
  int value;     // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  // DODGE: hard-coding number of buckets as 17.
  // NOTE: addressing this dodge is optional.
  entry_t *buckets[17];
};


ioopm_hash_table_t *ioopm_hash_table_create() {

    return calloc(1, sizeof(ioopm_hash_table_t));
 
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
 
    free(ht);
}   