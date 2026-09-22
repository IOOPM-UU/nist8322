#include <stddef.h>
#include <stdlib.h>
#include "hash_table.h"
#include <stdbool.h>
#include <string.h>
#define No_Buckets 17

typedef struct entry entry_t;

struct entry
{
  char *key;     // holds the key
  int value;     // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{

  entry_t buckets[No_Buckets];
};

static entry_t *find_previous_entry(ioopm_hash_table_t *ht, char *key);
static void entry_destroy(entry_t *entry);
static entry_t *entry_create(char *key, int value, entry_t *next);

static size_t string_knr_hash(const char *str)
{
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char) *str);
    str++;
  }
  return result;
}


static entry_t *entry_create(char *key, int value, entry_t *next)
{
  entry_t *entry = malloc(sizeof(entry_t));

  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
  
}

static void entry_destroy(entry_t *entry) {
    // Just nu gör den bara free. 
    // Men om du senare i uppgiften (t.ex. för minneshantering av strängar) 
    // behöver köra free(entry->key), är det HÄR du lägger till det!
    free(entry);
}



ioopm_hash_table_t *ioopm_hash_table_create() {

    return calloc(1, sizeof(ioopm_hash_table_t));
 
}

// Returnerar en int (1 för success, 0 för fail), och lägger värdet i pekaren 'result'
int ioopm_hash_table_remove(ioopm_hash_table_t *ht, char *key, int *result) {
    
    entry_t *previous = find_previous_entry(ht, key);
    entry_t *current = previous->next;
    
    if (current != NULL && strcmp(current->key, key) == 0) {
        
        // 1. Spara undan värdet INNAN noden raderas (Klassisk minnesfälla!)
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


entry_t *find_previous_entry(ioopm_hash_table_t *ht, char *key) 
{
  size_t bucket = string_knr_hash(key) % No_Buckets;

  entry_t *previous = &ht->buckets[bucket];
  entry_t *current = previous->next;

  while (current != NULL && strcmp(current->key, key) != 0) {
    previous = current;
    current = current->next;
  }

  return previous;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, char *key, int value)
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

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, char *key, int *result)
{
  entry_t *previous = find_previous_entry(ht, key);
  entry_t *current = previous->next;

  
  while (current != NULL && strcmp(current->key, key) != 0)
  {
    current = current->next;
  }

  // if the key exists, return the value, otherwise, indicate that the lookup failed
  if (current != NULL)
  {
    *result = current->value;
    return true;
  }
  else
  {
    return false;
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    
    for (int i = 0; i < No_Buckets; i++) {
        
        // Börja titta på noden EFTER dummyn (index 0 är dummyn)
        entry_t *current = ht->buckets[i].next; 
        
        // Loopa igenom den länkade listan och förstör alla riktiga noder
        while (current != NULL) {
            entry_t *next_node = current->next; // Spara nästa-pekaren
            
            // Använd din entry_destroy-funktion!
            entry_destroy(current); 
            
            current = next_node; // Hoppa till nästa
        }
    }
    
    // När alla länkade listor är tömda, frigör hela tabellen
    free(ht);
}


void remove_entry(ioopm_hash_table_t *ht, char *key) {
  
  entry_t *previous = find_previous_entry(ht, key);
  entry_t *current = previous->next;

  if (current != NULL && strcmp(current->key, key) == 0) {
    
    previous->next = current->next;
    
    entry_destroy(current);
  }
}

// TODO: documentation
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, char *key)
{
  entry_t *previous = find_previous_entry(ht, key);
  entry_t *current = previous->next;

  if (ioopm_hash_table_lookup(ht, key, &current->value) == true) 
  {
    return true;
  }
  else
  {
    return false;
  }
}


static int bucket_size(entry_t *current_node)
{
  int size = 0;

  while (current_node != NULL)
  {
    size = size + 1;
    current_node = current_node->next;
  }

  return size;
}


int ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  int size = 0;
  for (int i = 0; i < No_Buckets; i++) {
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

