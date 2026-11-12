#include <stdlib.h>

#include "train.h"

struct track {
    track_t *available_track;
    track_t *next;
    bool train;
};

track_t *track_create(track_t *next) {
  track_t *track = calloc(1, sizeof(track_t));
  track->next = next;
  track->train = false;
  return track;
}

void track_destroy(track_t *track) {
  if (track == NULL) {
    free(track);
    return;
  }
  track_destroy(track->available_track);
  track_destroy(track->next);

  free(track);
}

bool track_add_switch(track_t *from, track_t *to) {
  if (from->available_track == NULL) {
    from->available_track = to;
    return true;
  }
  
  return false;
}

bool track_has_train(track_t *track) {
  return track->train;
}

track_t *next_segment(track_t *current, bool take_switch) {
  if (take_switch && current->available_track != NULL) {


    return current->available_track;

  }
  return current->next;
}

track_t *track_move(track_t *start, size_t steps, bool take_switch) {
  if (!start->train) {
    start->train = true;
  }
  size_t counter = 0;
  track_t *current = start;

  while(counter < steps) {
    track_t *next = next_segment(current, take_switch);

    if (next == NULL || track_has_train(next)) {
      start->train = false;
      current->train = true;
      return current;
    }
    
    current = next;
    counter++;
  }

  start->train = false;
  current->train = true;
  return current;
}






#ifndef TEST
#include <stdio.h>

int main(void)
{
  puts("If you want to you can write test code here and run it using `make run`");
  return 0;
}
#endif