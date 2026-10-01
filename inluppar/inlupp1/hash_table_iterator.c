#include "hash_table_iterator.h"
#include "hash_table_private.h"
#include <stdbool.h>
#include <stdlib.h>

struct hash_table_iterator
{
  ioopm_hash_table_t *ht;
  int current_bucket;
  entry_t *current_entry;
};

// En enda smart funktion för att navigera hela tabellen säkert
static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{

  if (it->current_bucket >= No_Buckets) return;


  if (it->current_entry != NULL) {
    it->current_entry = it->current_entry->next;
  }

  while (it->current_entry == NULL)
  {
    it->current_bucket += 1;


    if (it->current_bucket >= No_Buckets)
    {
      it->current_entry = NULL;
      return;
    }

    it->current_entry = it->ht->buckets[it->current_bucket].next;
  }
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
  // calloc nollställer minnet! Det botar Valgrinds "uninitialised value"-varning
  ioopm_hash_table_iterator_t *it = calloc(1, sizeof(ioopm_hash_table_iterator_t));
  it->ht = ht;
  it->current_bucket = 0;
  
  // Ställ iteratorn direkt på det första riktiga elementet (efter dummyn)
  it->current_entry = ht->buckets[0].next;
  
  // Om bucket 0 var tom, använd vår smarta advance för att leta upp nästa data
  if (it->current_entry == NULL)
  {
    advance_iterator_state(it);
  }
  
  return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
  free(it);
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
  advance_iterator_state(it); 
}

elem_t ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it)
{

  return it->current_entry != NULL ? it->current_entry->key : ptr_elem(NULL);
}

elem_t ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it)
{
  
  return it->current_entry != NULL ? it->current_entry->value : int_elem(0);
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it)
{

  return it->current_bucket >= No_Buckets || it->current_entry == NULL;
}