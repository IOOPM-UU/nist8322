#pragma once

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