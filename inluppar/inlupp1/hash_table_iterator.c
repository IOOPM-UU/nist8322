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
  // 1. Spärr: Avbryt BARA om vi faktiskt gått förbi sista bucketen.
  // Vi får inte avbryta för att current_entry är NULL, för då kan vi inte
  // hoppa över tomma buckets!
  if (it->current_bucket >= No_Buckets) return;

  // 2. Om vi står på en giltig nod, ta ett steg framåt i listan
  if (it->current_entry != NULL) {
    it->current_entry = it->current_entry->next;
  }

  // 3. Om vi trillade av listan (eller om bucketen var tom från start),
  // stega framåt i arrayen tills vi hittar nästa bucket med data.
  while (it->current_entry == NULL)
  {
    it->current_bucket += 1;

    // Har vi gått igenom hela arrayen?
    if (it->current_bucket >= No_Buckets)
    {
      it->current_entry = NULL;
      return;
    }

    // Ställ oss på nästa buckets FÖRSTA RIKTIGA element (hoppa över dummyn!)
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

char *ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it)
{

  return it->current_entry != NULL ? it->current_entry->key : NULL;
}

int ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it)
{
  
  return it->current_entry != NULL ? it->current_entry->value : 0;
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it)
{

  return it->current_bucket >= No_Buckets || it->current_entry == NULL;
}