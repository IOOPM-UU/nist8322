#include <stdlib.h>

#include "board.h"

struct tile
{
  tile_t *next_square;
  tile_t *dif_path; // antingen stege eller rutschkana
  bool piece_on_square;
};

tile_t *tile_create(tile_t *next)
{
  tile_t *tile = calloc(1, sizeof(tile_t));
  tile->next_square = next;
  tile->dif_path = NULL;
  tile->piece_on_square = false;

  return tile;
}

void tile_destroy(tile_t *tile)
{
    tile_t *current = tile;
    tile_t *next = tile->next_square;

  while (next != NULL) {
    tile_t *temp = current;
    current = next;

    free(temp);
    next = next->next_square;
  }
  free(current);

  return;
}

bool tile_add_link(tile_t *from, tile_t *to)
{
  if (from->dif_path == to) {
    return false;

  } else {
    from->dif_path = to;
    return true;
  }
  
}

bool tile_has_piece(tile_t *tile)
{
  return (tile->piece_on_square);
}

tile_t *where_to(tile_t *start, size_t steps) 
{
    tile_t *current = start;
    size_t counter = 0;

    while (current->next_square != NULL && counter < steps) {
        current = current->next_square;
        counter++;
    }
    return current;
}

tile_t *tile_move(tile_t *start, size_t steps)
{
    if (steps == 0) {
        start->piece_on_square = true;
        return start;
    }

    tile_t *to = where_to(start, steps);

    if (to->dif_path != NULL) {
        to = to->dif_path;
    }

    if (tile_has_piece(to) == true) {
        start->piece_on_square = true;
        return start;
    }

    to->piece_on_square = true;
    start->piece_on_square = false;
    return to;
}

// Write your implementation above this line
#ifndef TEST
#include <stdio.h>

int main(void)
{
  puts("If you want to you can write test code here and run it using `make run`");
  return 0;
}
#endif
