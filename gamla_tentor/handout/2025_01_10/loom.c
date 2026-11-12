#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "loom.h"

/**
 * @brief check that an instruction is well-formed and produces a
 * row of a certain width. If the expected width is 0 it is
 * ignored.
 * @param instruction the instruction to check (can be NULL)
 * @param expected_width the expected width of the result of the instruction
 * @return true if `instruction` is NULL or if the width of the
 * result of the instruction is `expected_width`
 */
static bool loom_check_instruction(char *instruction, size_t expected_width) {
  if (!instruction) return true;

  size_t length = strlen(instruction);
  size_t width = 0;
  for (size_t i = 0; i < length; i++) {
    char c = instruction[i];
    if (isdigit(c)) {
      short value = c - '0'; // Get the value of the digit
      width += value;

      // Skip the next character, but check that it is not a digit
      // or the end of the string
      c = instruction[++i];
      if (!c || isdigit(c)) return false;
    }
    else {
      width++;
    }
  }

  return (width == expected_width);
}

typedef struct loom loom_t;

struct loom {
  char **instructions;
  size_t width;
  size_t instr_size;
  size_t counter;
};

loom_t *loom_create(size_t instr_size, size_t width) {
   if (instr_size == 0 || width == 0) {
    return NULL;
   }
    loom_t *loom = calloc(1, sizeof(loom_t));
    loom->instructions = calloc(instr_size, sizeof(char *));
    loom->width = width;
    loom->instr_size = instr_size;
  return loom;
}

void loom_destroy(loom_t *loom) {
  for (size_t i = 0; i < loom->instr_size; i++) {
    free(loom->instructions[i]);
  }
    free(loom->instructions);
    free(loom);
}

bool loom_add_instruction(loom_t *loom, char *instruction) {
  if (instruction == NULL)
  {
    return false;
  }
  if (loom->counter >= loom->instr_size)
  {
    return false;
  }
  if (loom_check_instruction(instruction, loom->width))
  {
    loom->instructions[loom->counter] = strdup(instruction);
    loom->counter++;
    return true;
  }
  return false;
} 


char **loom_weave(loom_t *loom) {
    
  for (size_t i = 0; i < loom->instr_size; i++) {
    char x = loom->instructions[i];
    short value = x - '0';
    if (isdigit(loom->instructions[i])) {
       for (size_t x = 0; x > 1; x++) {

       loom->instructions[i] = loom->instructions[i + x - 1];


       }
    }
  }
    
}

#ifndef TEST
int main(void)
{
  puts("You can put your own tests here if you want");
  return 0;
}
#endif
