#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "common.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

size_t string_hash(elem_t key) {
    const char *str = (char *)key.p;
    size_t result = 0;
    while (*str != '\0') {
        result = result * 31 + ((unsigned char) *str);
        str++;
    }
    return result;
}

bool string_eq(elem_t a, elem_t b) {
    return strcmp((char *)a.p, (char *)b.p) == 0;
}

size_t int_hash(elem_t key) {
    return (size_t)key.i;
}

bool int_eq(elem_t a, elem_t b) {
    return a.i == b.i;
}

void process_word(elem_t word, ioopm_hash_table_t *ht)
{
    elem_t result = int_elem(0);
    elem_t freq =
      ioopm_hash_table_lookup(ht, word, &result) ?
      result: int_elem(0);

    ioopm_hash_table_insert(ht, ptr_elem(strdup((char *)word.s)), int_elem(freq.i + 1));
}


void process_file(char* filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");
  if (f == NULL) return;

  while (true)
{
    char *buf = NULL;
    size_t len = 0;

    ssize_t read = getline(&buf, &len, f);
    if (read == -1)
    {
        free(buf);
        break;
    }

    
    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters))
    {
      
        process_word(ptr_elem(word), ht);
    }
    free(buf);
}
  fclose(f);
}

struct freq_word
{
  char *word;
  int freq;
};


static int cmp_freq_words(const void *p1, const void *p2)
{
  const struct freq_word *w1 = p1;
  const struct freq_word *w2 = p2;

  return w1->freq - w2->freq;
}


static int cmp_freq_words_reverse(const void *p1, const void *p2)
{
  return -cmp_freq_words(p1, p2);
}


void sort_freq_words(struct freq_word words[], size_t no_words)
{
  qsort(words, no_words, sizeof(struct freq_word), cmp_freq_words_reverse);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s file1 ... filen", argv[0]);
    return 1;
  }

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  for (size_t i = 1; i < argc; ++i)
  {
    process_file(argv[i], ht);
  }

  size_t size = ioopm_hash_table_size(ht);
  struct freq_word freq_words[size];
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  size_t index = 0;


while (!ioopm_hash_table_iterator_at_end(it)) 
{
    elem_t freq = ioopm_hash_table_iterator_current_value(it);
    elem_t word = ioopm_hash_table_iterator_current_key(it);

    freq_words[index].freq = freq.i;
    freq_words[index].word = (char *)word.s;
    index++;

    ioopm_hash_table_iterator_advance(it);
}

ioopm_hash_table_iterator_destroy(it);


  sort_freq_words(freq_words, size);

  for (int i = 0; i < size; ++i)
  {
    printf("%s: %d\n", freq_words[i].word, freq_words[i].freq);
  }

  ioopm_hash_table_destroy(ht);
}
