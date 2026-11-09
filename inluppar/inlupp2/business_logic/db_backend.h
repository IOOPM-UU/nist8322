#pragma once
#include <stdbool.h>
#include "hash_table.h"
#include "linked_list.h"

// --- Datastrukturer ---

struct merch {
  char *name;
  char *description;
  int price;
  ioopm_list_t *location;
};
typedef struct merch merch_t;

struct shelf {
    char *shelf_location;
    int quantity;
};
typedef struct shelf shelf_t;

struct cart {
    int id;
    ioopm_hash_table_t *items;
};
typedef struct cart cart_t;

struct db {
    ioopm_hash_table_t *ht_n;
    ioopm_hash_table_t *ht_sl;
    
    ioopm_hash_table_t *carts;
    int cart_counter;
};
typedef struct db db_t;

// --- Systemets livscykel ---

db_t *db_create(void);
void db_destroy(db_t *db);

// --- Hantering av Varor (Katalog) ---

bool add_merch_to_db(db_t *db, char *name, char *description, int price);
bool remove_merch_from_db(db_t *db, char *name);
bool edit_db(db_t *db, char *old_name, char *new_name, char *new_desc, int new_price);


merch_t *get_merch(db_t *db, char *name); 

// --- Hantering av Lagersaldo ---

bool add_stock(db_t *db, char *name, int amount, char *shelf);

int get_total_stock(db_t *db, char *name);

int get_cart_reserved_stock(db_t *db, char *name);

bool add_to_cart(db_t *db, int cart_id, char *name, int amount);

bool remove_from_cart(db_t *db, int cart_id, char *name, int amount);

int calculate_cost(db_t *db, int cart_id);

bool checkout_cart(db_t *db, int cart_id);

int create_cart(db_t *db);

bool remove_cart(db_t *db, int cart_id);