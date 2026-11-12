#include "line.h"

#include <stdlib.h>



// Helper function for getting the capacity for a given kind

static size_t kind_capacity(rack_kind_t kind) {

return kind == RACK_CUTLERY ? CUTLERY_CAPACITY :

kind == RACK_CUPS ? CUPS_CAPACITY :

PLATES_CAPACITY;

}



struct rack {

/// EC: Du behöver också veta hur många saker som finns i korgen just nu

rack_kind_t kind;

/// EC: Kapaciteten är alltid kind_capacity(kind), så den behöver inte lagras

int capacity;

rack_t *next;

};





rack_kind_t rack_kind(rack_t *rack) {

return rack->kind;

}



size_t rack_count(rack_t *rack) {

return kind_capacity(rack->kind);

}



rack_t *create_rack(rack_kind_t kind) {

rack_t *rack = calloc(1, sizeof(rack_t));

rack->kind = kind;

rack->capacity = kind_capacity(kind);

rack->next = NULL;

return rack;

}



struct line {

rack_t *head;

rack_t *last;

/// EC: Storleken behövs egentligen inte

size_t size;

};



line_t *line_create(void) {

line_t *line = calloc(1, sizeof(line_t));

/// EC: Egentligen onödigt eftersom calloc nollställer minnet

line->head = NULL;

line->last = NULL;

line->size = 0;

return line;

}



void line_destroy(line_t *line) {

rack_t *current = line->head;

/// EC: If-satsen gör ingenting meningsfullt

if(current == NULL) {

free(current);

}

while(current != NULL) {

rack_t *temp = current;

current = current->next;

free(temp);

}

free(line);

return;

}



rack_t *line_add_rack(line_t *line, rack_kind_t kind) {

rack_t *rack = create_rack(kind);

/// EC: Det här fungerar bara om det redan finns en korg i

/// listan! Annars måste line->first (och last) uppdateras

line->last->next = rack;

line->last = line->last->next;

return rack;

}



int max_capacity(rack_kind_t kind) {

return kind == RACK_CUTLERY ? 3:

kind == RACK_CUPS ? 2 :

4;

}



bool line_add_item(line_t *line, rack_kind_t kind) {

rack_t *current = line->head;

/// EC: Du måste också titta att det är rätt kind i korgarna

while(current->capacity >= max_capacity(kind)) {

current = current->next;

}

if (current->kind == kind) {

/// EC: Här verkar du blanda ihop kapacitet med antal saker

current->capacity++;

return true;

}

return false;

}



rack_t *line_get_rack(line_t *line, size_t pos) {

rack_t *current = line->head;

for (size_t i = 0; i < pos; i++) {

/// EC: Du måste kolla så du inte går för långt i listan

current = current->next;

}

if (current != NULL) {

return current;

} else {

return NULL;

}

}



int line_wash_next(line_t *line) {

rack_t *first = line->head;

first->next = line->head;

int temp = first->capacity;

first = NULL;

/// EC: Du frigör line->head, men den ligger kvar i listan

free(first);

if (line->head != NULL) {

return temp;

}

return -1;

}



#ifndef TEST

#include <stdio.h>



int main(int argc, char *argv[]) {

puts("If you want to you can write test code here and run it using `make run`");

return 0;

}

#endif 