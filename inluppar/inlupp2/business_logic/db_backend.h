#pragma once
#include <stdbool.h>
#include "hash_table.h"
#include "linked_list.h"

// Datastrukturer

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

// Systemets livscykel

/// @brief Creates a new database
/// @return a pointer to the newly created database
db_t *db_create(void);

/// @brief Destroys the database and frees all associated memory
/// @param db the database to be destroyed
void db_destroy(db_t *db);

// --- Hantering av Varor (Katalog) ---

/// @brief Adds a new merchandise item to the database
/// @param db the database operated upon
/// @param name name of the merchandise
/// @param description description of the merchandise
/// @param price price of the merchandise
/// @return true if successful, false if the merchandise already exists
bool add_merch_to_db(db_t *db, char *name, char *description, int price);

/// @brief Removes a merchandise item from the database
/// @param db the database operated upon
/// @param name name of the merchandise to remove
/// @return true if successful, false if it doesn't exist or is currently in a cart
bool remove_merch_from_db(db_t *db, char *name);

/// @brief Edits the details of an existing merchandise item
/// @param db the database operated upon
/// @param old_name the current name of the merchandise
/// @param new_name the new name for the merchandise
/// @param new_desc the new description for the merchandise
/// @param new_price the new price for the merchandise
/// @return true if successful, false if old_name doesn't exist or new_name is already taken
bool edit_db(db_t *db, char *old_name, char *new_name, char *new_desc, int new_price);

/// @brief Retrieves a merchandise item from the database
/// @param db the database operated upon
/// @param name name of the merchandise to retrieve
/// @return pointer to the merch_t struct, or NULL if not found
merch_t *get_merch(db_t *db, char *name); 

// --- Hantering av Lagersaldo ---

/// @brief Adds stock to a specific shelf for a merchandise item
/// @param db the database operated upon
/// @param name name of the merchandise
/// @param amount quantity to add
/// @param shelf shelf location to add stock to
/// @return true if successful, false if the merchandise doesn't exist
bool add_stock(db_t *db, char *name, int amount, char *shelf);

/// @brief Calculates the total available stock for a merchandise item
/// @param db the database operated upon
/// @param name name of the merchandise
/// @return the total quantity in stock across all shelves
int get_total_stock(db_t *db, char *name);

/// @brief Calculates the total stock reserved across all active carts for a merchandise item
/// @param db the database operated upon
/// @param name name of the merchandise
/// @return the total quantity currently reserved in carts
int get_cart_reserved_stock(db_t *db, char *name);

/// @brief Adds a specified amount of merchandise to a shopping cart
/// @param db the database operated upon
/// @param cart_id ID of the cart
/// @param name name of the merchandise
/// @param amount quantity to add
/// @return true if successful, false if stock is insufficient or cart/merch doesn't exist
bool add_to_cart(db_t *db, int cart_id, char *name, int amount);

/// @brief Removes a specified amount of merchandise from a shopping cart
/// @param db the database operated upon
/// @param cart_id ID of the cart
/// @param name name of the merchandise
/// @param amount quantity to remove
/// @return true if successful, false if cart/merch doesn't exist or amount is invalid
bool remove_from_cart(db_t *db, int cart_id, char *name, int amount);

/// @brief Calculates the total cost of all items currently in a cart
/// @param db the database operated upon
/// @param cart_id ID of the cart
/// @return the total cost of the cart
int calculate_cost(db_t *db, int cart_id);

/// @brief Checks out a cart, permanently removing the purchased stock and destroying the cart
/// @param db the database operated upon
/// @param cart_id ID of the cart to checkout
/// @return true if successful, false if the cart doesn't exist
bool checkout_cart(db_t *db, int cart_id);

/// @brief Creates a new shopping cart
/// @param db the database operated upon
/// @return the unique ID of the newly created cart
int create_cart(db_t *db);

/// @brief Removes a cart and releases its reserved items back to available stock
/// @param db the database operated upon
/// @param cart_id ID of the cart to remove
/// @return true if successful, false if the cart doesn't exist
bool remove_cart(db_t *db, int cart_id);

/// @brief Checks if a given string is a valid shelf format (e.g., a letter followed by digits)
/// @param str the string to be validated
/// @return true if the string is a valid shelf format, false otherwise
bool is_shelf(char *str);