#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash_table.h"         
#include "hash_table_private.h"


static entry_t *find_previous_entry(ioopm_hash_table_t *ht, elem_t key);
static void entry_destroy(entry_t *entry);
static entry_t *entry_create(elem_t key, elem_t value, entry_t *next);

size_t string_knr_hash(elem_t key)
{
    const char *str = key.s;
    size_t result = 0;
    
    while (*str != '\0')
    {
        result = result * 31 + ((unsigned char) *str);
        str++;
    }
    return result;
}


static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *entry = malloc(sizeof(entry_t));

  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
  
}

static void entry_destroy(entry_t *entry) {

    free(entry);
}



ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn) {
    
    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
    
   
    ht->hash_fn = hash_fn;
    ht->key_eq_fn = key_eq_fn;
    
   
    return ht;
}

// Returnerar en int (1 för success, 0 för fail), och lägger värdet i pekaren 'result'
int ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result) {
    
    entry_t *previous = find_previous_entry(ht, key);
    entry_t *current = previous->next;
    
    if (current != NULL && ht->key_eq_fn(current->key, key) == true) {
        
        
        if (result != NULL) {
            *result = current->value;
        }
        
        // 2. Överbrygga hålet
        previous->next = current->next;
        
        // 3. Frigör minnet
        entry_destroy(current);
        
        return 1; // Success!
    }
    
    return 0; // Fail! (Nyckeln fanns inte)
}


entry_t *find_previous_entry(ioopm_hash_table_t *ht, elem_t key) 
{
  size_t bucket = ht->hash_fn(key) % No_Buckets;

  entry_t *previous = &ht->buckets[bucket];
  entry_t *current = previous->next;

  while (current != NULL && ht->key_eq_fn(current->key, key) == false) {
    previous = current;
    current = current->next;
}

  return previous;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  // find previous entry, or the last entry if the key does not exist
  entry_t *previous = find_previous_entry(ht, key);

  // if the key exists, update the value, otherwise create a new entry
  if (previous->next != NULL)
  {
    previous->next->value = value;
  }
  else
  {
    previous->next = entry_create(key, value, NULL);
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result) {
    // 1. Låt din redan fixade hjälpfunktion göra sökjobbet
    entry_t *previous = find_previous_entry(ht, key);
    entry_t *current = previous->next;
    
    // 2. Kolla om vi hittade en matchning
    if (current != NULL && ht->key_eq_fn(current->key, key)) {
        if (result != NULL) {
            *result = current->value;
        }
        return true;
    }
    return false;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    
    for (size_t i = 0; i < No_Buckets; i++) {
        
        
        entry_t *current = ht->buckets[i].next; 
        
        while (current != NULL) {
            entry_t *next_node = current->next;
            
            
            entry_destroy(current); 
            
            current = next_node;
        }
    }
    
    free(ht);
}


void remove_entry(ioopm_hash_table_t *ht, elem_t key) {
  
  entry_t *previous = find_previous_entry(ht, key);
  entry_t *current = previous->next;

  if (current != NULL && ht->key_eq_fn(current->key, key) == true) {
    
    previous->next = current->next;
    
    entry_destroy(current);
  }
}

// TODO: documentation
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key) {
    // Skicka in NULL som result-pekare, vi bryr oss bara om true/false
    return ioopm_hash_table_lookup(ht, key, NULL);
}


static size_t bucket_size(entry_t *current_node)
{
  size_t size = 0;

  while (current_node != NULL)
  {
    size = size + 1;
    current_node = current_node->next;
  }

  return size;
}


size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t size = 0;
  for (size_t i = 0; i < No_Buckets; i++) {
    size += bucket_size(ht->buckets[i].next);
  }
  return size;
}


bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  if (ioopm_hash_table_size(ht) == 0)
  {
    return true;
  } 
  else
  {
    return false;
  }
}

