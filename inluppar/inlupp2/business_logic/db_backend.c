#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "../generic_utils/utils.h"
#include <string.h>
#include "db_backend.h"
#include "linked_list.h"
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"

static bool int_key_eq(elem_t k1, elem_t k2) {
    return k1.i == k2.i;
}

static size_t int_knr_hash(elem_t key) {
    return key.i;
}

static bool string_key_eq(elem_t k1, elem_t k2)
{
  return strcmp(k1.s, k2.s) == 0; // if elem_t has field char *string
}

static size_t string_knr_hash(elem_t key) {
    char *str = key.s;
    size_t hash = 0;
    
    if (str == NULL) {
        return 0;
    }

    while (*str != '\0') {
        hash = hash * 31 + *str;
        str++;
    }
    
    return hash;
}

static merch_t *make_merch(char *name, char *description, int price) {
    merch_t *new_merch = calloc(1, sizeof(merch_t));
    
    new_merch->name = name;
    new_merch->description = description;
    new_merch->price = price;
    new_merch->location = ioopm_list_create(); 
    
    return new_merch;
}

db_t *db_create(void) {
    db_t *db = calloc(1, sizeof(db_t));

    db->ht_n = ioopm_hash_table_create(string_knr_hash, string_key_eq);
    db->ht_sl = ioopm_hash_table_create(string_knr_hash, string_key_eq);
    db->carts = ioopm_hash_table_create(int_knr_hash, int_key_eq);
    db->cart_counter = 1;

    return db;
}

void db_destroy(db_t *db) {
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(db->ht_n);

    while (!ioopm_hash_table_iterator_at_end(it)) {
        elem_t current_merch_elem = ioopm_hash_table_iterator_current_value(it);
        merch_t *merch = (merch_t *)current_merch_elem.p;

        ioopm_list_iterator_t *list_it = ioopm_list_iterator_create(merch->location);
        while (!ioopm_list_iterator_at_end(list_it)) {
            elem_t current_shelf_elem = ioopm_list_iterator_current(list_it);
            shelf_t *shelf = (shelf_t *)current_shelf_elem.p;
            
            free(shelf->shelf_location);
            free(shelf);
            
            ioopm_list_iterator_advance(list_it);
        }
        ioopm_list_iterator_destroy(list_it);
        ioopm_list_destroy(merch->location);
        
        free(merch->name);
        free(merch->description);
        free(merch);

        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    ioopm_hash_table_iterator_t *cart_it = ioopm_hash_table_iterator_create(db->carts);
    
    while (!ioopm_hash_table_iterator_at_end(cart_it)) {
        elem_t current_cart_elem = ioopm_hash_table_iterator_current_value(cart_it);
        cart_t *cart = (cart_t *)current_cart_elem.p;

        ioopm_hash_table_destroy(cart->items);
        
        free(cart);
        
        ioopm_hash_table_iterator_advance(cart_it);
    }
    ioopm_hash_table_iterator_destroy(cart_it);
    
    ioopm_hash_table_destroy(db->ht_n);
    ioopm_hash_table_destroy(db->ht_sl);
    ioopm_hash_table_destroy(db->carts);

    free(db);
}

// Just nu förstör din funktion bara själva hashtabellerna, men inte varorna och hyllorna inuti dem.

merch_t *get_merch(db_t *db, char *name) {
    elem_t key = { .s = name };
    elem_t result;
    
    if (ioopm_hash_table_lookup(db->ht_n, key, &result)) {
        // result.p är void-pekaren. Vi castar den till en merch_t-pekare
        return (merch_t *) result.p; 
    }

    return NULL; 
}



bool is_shelf(char *str) {
    int len = strlen(str);

    if (len < 2) {
        return false;
    }

    if (!isalpha(str[0])) {
        return false;
    }

    for (int i = 1; i < len; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool add_merch_to_db(db_t *db, char *name, char *description, int price) {
    elem_t lookup_key = { .s = name };
    elem_t dummy_result; 

    if (ioopm_hash_table_lookup(db->ht_n, lookup_key, &dummy_result)) {
        return false;
    }

    char *name_copy = strdup(name);
    char *desc_copy = strdup(description);

    merch_t *new_merch = make_merch(name_copy, desc_copy, price);
    
    elem_t insert_key = { .s = name_copy };
    elem_t value = { .p = new_merch };
    
    ioopm_hash_table_insert(db->ht_n, insert_key, value);

    return true;
}

bool add_stock(db_t *db, char *name, int amount, char *shelf) {
    merch_t *merch = get_merch(db, name);
    if (merch == NULL) return false; 

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->location);
    
    while (!ioopm_list_iterator_at_end(it)) { 
        elem_t current = ioopm_list_iterator_current(it);
    
        if (strcmp(((shelf_t *)current.p)->shelf_location, shelf) == 0) {
            ((shelf_t *)current.p)->quantity += amount;
            ioopm_list_iterator_destroy(it); 
            return true;
        }
    
        ioopm_list_iterator_advance(it);
    } 

    ioopm_list_iterator_destroy(it); 

    elem_t dummy_result;
    elem_t lookup_key = { .s = shelf };

    if (!ioopm_hash_table_lookup(db->ht_sl, lookup_key, &dummy_result)) {
        shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
        
        char *shelf_copy = strdup(shelf);
        
        new_shelf->shelf_location = shelf_copy;
        new_shelf->quantity = amount;

        elem_t list_elem = { .p = new_shelf };
        ioopm_list_append(merch->location, list_elem);

        elem_t ht_key = { .s = shelf_copy };
        elem_t ht_value = { .s = merch->name }; 

        ioopm_hash_table_insert(db->ht_sl, ht_key, ht_value);

        return true;
    }

    return false;
}

bool edit_db(db_t *db, char *old_name, char *new_name, char *new_desc, int new_price) {
    if (get_cart_reserved_stock(db, old_name) > 0) {
        return false; 
    }
    
    merch_t *merch = get_merch(db, old_name);
    if (merch == NULL) {
        return false; 
    }

    // Om vi byter namn, kolla så det nya namnet inte redan är upptaget
    if (strcmp(old_name, new_name) != 0) {
        elem_t dummy;
        if (ioopm_hash_table_lookup(db->ht_n, (elem_t){.s = new_name}, &dummy)) {
            return false; 
        }

        // Ta bort gamla namnet från hashtabellen
        ioopm_hash_table_remove(db->ht_n, (elem_t){.s = old_name}, &dummy);
        
        // Uppdatera namn och lägg in på ny nyckel
        free(merch->name);
        merch->name = strdup(new_name);
        ioopm_hash_table_insert(db->ht_n, (elem_t){.s = merch->name}, (elem_t){.p = merch});
    }

    // Uppdatera beskrivning och pris
    free(merch->description);
    merch->description = strdup(new_desc);
    merch->price = new_price;

    return true;
}

int get_total_stock(db_t *db, char *name) {

    merch_t *merch = get_merch(db, name);
    if (merch == NULL) {
        return 0;
    }

    int total_amount = 0;
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->location);
    
    while (!ioopm_list_iterator_at_end(it)) {
        elem_t current = ioopm_list_iterator_current(it);
        
        shelf_t *current_shelf = (shelf_t *)current.p;
        total_amount += current_shelf->quantity; 
        
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
    
    return total_amount;
}

int get_cart_reserved_stock(db_t *db, char *name) {
    int reserved = 0;
    elem_t merch_key = { .s = name };

    ioopm_hash_table_iterator_t *cart_it = ioopm_hash_table_iterator_create(db->carts);
    
    while (!ioopm_hash_table_iterator_at_end(cart_it)) {
        elem_t current_cart_elem = ioopm_hash_table_iterator_current_value(cart_it);
        cart_t *cart = (cart_t *)current_cart_elem.p;

        elem_t quantity_elem;
        // Om varan finns i just den här vagnen, lägg till antalet till vår summa
        if (ioopm_hash_table_lookup(cart->items, merch_key, &quantity_elem)) {
            reserved += quantity_elem.i;
        }

        ioopm_hash_table_iterator_advance(cart_it);
    }
    ioopm_hash_table_iterator_destroy(cart_it);

    return reserved;
}

int create_cart(db_t *db) {

    cart_t *new_cart = calloc(1, sizeof(cart_t));
    
    new_cart->items = ioopm_hash_table_create(string_knr_hash, string_key_eq);

    int cart_id = db->cart_counter;
    db->cart_counter++;

    elem_t key = { .i = cart_id };
    elem_t value = { .p = new_cart };
    ioopm_hash_table_insert(db->carts, key, value);

    return cart_id;
}

bool remove_cart(db_t *db, int cart_id) {
    elem_t dummy_result;
    elem_t cart_key = { .i = cart_id };

    if (ioopm_hash_table_remove(db->carts, cart_key, &dummy_result)) {
        cart_t *cart = (cart_t *)dummy_result.p;

        ioopm_hash_table_destroy(cart->items); 
        
        free(cart); 
        
        return true;
    }
    
    return false;
}

bool add_to_cart(db_t *db, int cart_id, char *name, int amount) {
    if (amount <= 0) return false;

    merch_t *merch = get_merch(db, name);
    if (merch == NULL) {
        return false;
    }

    elem_t cart_key = { .i = cart_id };
    elem_t cart_elem;
    if (!ioopm_hash_table_lookup(db->carts, cart_key, &cart_elem)) {
        return false; 
    }
    cart_t *cart = (cart_t *)cart_elem.p;

    int total_stock = get_total_stock(db, name);
    int reserved_stock = get_cart_reserved_stock(db, name);

    if (reserved_stock + amount > total_stock) {
        return false;
    }

    elem_t merch_key = { .s = merch->name };
    elem_t current_quantity_elem;

    if (ioopm_hash_table_lookup(cart->items, merch_key, &current_quantity_elem)) {
        int new_quantity = current_quantity_elem.i + amount;
        elem_t dummy;
        ioopm_hash_table_remove(cart->items, merch_key, &dummy);
        elem_t new_value = { .i = new_quantity };
        ioopm_hash_table_insert(cart->items, merch_key, new_value);
    } else {
        elem_t new_value = { .i = amount };
        ioopm_hash_table_insert(cart->items, merch_key, new_value);
    }

    return true;
}

bool remove_from_cart(db_t *db, int cart_id, char *name, int amount) {
    if (amount <= 0) return false;

    merch_t *merch = get_merch(db, name);
    if (merch == NULL) return false;

    elem_t cart_key = { .i = cart_id };
    elem_t cart_elem;
    if (!ioopm_hash_table_lookup(db->carts, cart_key, &cart_elem)) {
        return false;
    }
    cart_t *cart = (cart_t *)cart_elem.p;

    elem_t merch_key = { .s = merch->name };
    elem_t current_quantity_elem;

    if (!ioopm_hash_table_lookup(cart->items, merch_key, &current_quantity_elem)) {
        return false;
    }

    if (amount > current_quantity_elem.i) {
        return false;
    }

    int new_quantity = current_quantity_elem.i - amount;
    elem_t dummy;

    if (new_quantity == 0) {
        ioopm_hash_table_remove(cart->items, merch_key, &dummy);
    } else {
        ioopm_hash_table_remove(cart->items, merch_key, &dummy);
        elem_t new_value = { .i = new_quantity };
        ioopm_hash_table_insert(cart->items, merch_key, new_value);
    }

    return true;
}

int calculate_cost(db_t *db, int cart_id) {
    elem_t dummy_result;
    elem_t cart_key = { .i = cart_id };
    int total_cost = 0;



    if (!ioopm_hash_table_lookup(db->carts, cart_key, &dummy_result)) {
        return 0;
    }

    cart_t *cart = (cart_t *)dummy_result.p;

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->items);

    while (!ioopm_hash_table_iterator_at_end(it)) {
        elem_t current_key = ioopm_hash_table_iterator_current_key(it);
        elem_t current_value = ioopm_hash_table_iterator_current_value(it);
        int amount_in_cart = current_value.i;
        
    
        elem_t merch_result;

    if (ioopm_hash_table_lookup(db->ht_n, current_key, &merch_result)) {

    merch_t *merch = (merch_t *)merch_result.p;
    total_cost += (merch->price * amount_in_cart);

    }
    ioopm_hash_table_iterator_advance(it);
}
ioopm_hash_table_iterator_destroy(it);
return total_cost;
}


bool checkout_cart(db_t *db, int cart_id) {
    elem_t dummy_result1;
    elem_t dummy_result2;
    elem_t cart_key = { .i = cart_id };
    
    if (!ioopm_hash_table_lookup(db->carts, cart_key, &dummy_result1)) {
        return false;
    }

    cart_t *cart = (cart_t *)dummy_result1.p;
    
    ioopm_hash_table_iterator_t *it_cart = ioopm_hash_table_iterator_create(cart->items);
    
    while (!ioopm_hash_table_iterator_at_end(it_cart)) {
        elem_t current_key = ioopm_hash_table_iterator_current_key(it_cart);
        elem_t current_value = ioopm_hash_table_iterator_current_value(it_cart);
        
        ioopm_hash_table_lookup(db->ht_n, current_key, &dummy_result2);
        merch_t *merch = (merch_t *)dummy_result2.p;
        int amount_left_to_pick = current_value.i;
        
        ioopm_list_iterator_t *it_merch = ioopm_list_iterator_create(merch->location);

        while (!ioopm_list_iterator_at_end(it_merch)) {
            elem_t current_shelf_elem = ioopm_list_iterator_current(it_merch);
            shelf_t *shelf = (shelf_t *)current_shelf_elem.p;
            
            if (amount_left_to_pick > 0) {
                if (shelf->quantity >= amount_left_to_pick) {
                    shelf->quantity = shelf->quantity - amount_left_to_pick;
                    amount_left_to_pick = 0;
                } else {
                    amount_left_to_pick = amount_left_to_pick - shelf->quantity;
                    shelf->quantity = 0;
                }
            }
            ioopm_list_iterator_advance(it_merch);
        }
        
        ioopm_list_iterator_destroy(it_merch);

        ioopm_hash_table_iterator_advance(it_cart);
    }

    ioopm_hash_table_iterator_destroy(it_cart);

    elem_t removed_cart_elem;
    ioopm_hash_table_remove(db->carts, cart_key, &removed_cart_elem);
    cart_t *cart_to_remove = (cart_t *)removed_cart_elem.p;

    ioopm_hash_table_destroy(cart_to_remove->items);
    free(cart_to_remove);

    return true;
}

bool merch_in_cart(db_t *db, char *name) {
    if (ioopm_hash_table_size(db->carts) == 0) {
        return false;
    }

    ioopm_hash_table_iterator_t *cart_it = ioopm_hash_table_iterator_create(db->carts);
    
    while (!ioopm_hash_table_iterator_at_end(cart_it)) {
        cart_t *cart = (cart_t *)ioopm_hash_table_iterator_current_value(cart_it).p;
        
        elem_t dummy_val;
        if (ioopm_hash_table_lookup(cart->items, (elem_t){.s = name}, &dummy_val)) {
            ioopm_hash_table_iterator_destroy(cart_it);
            return true;
        }
        
        ioopm_hash_table_iterator_advance(cart_it);
    }
    
    ioopm_hash_table_iterator_destroy(cart_it);
    return false;
}

bool remove_merch_from_db(db_t *db, char *name) {
    merch_t *merch = get_merch(db, name);
    if (merch == NULL) {
        return false; 
    }

    if (merch_in_cart(db, name)) {
        return false;
    }

    if (ioopm_list_size(merch->location) > 0) {
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->location);
        
        while (!ioopm_list_iterator_at_end(it)) {
            elem_t current = ioopm_list_iterator_current(it);
            shelf_t *shelf = (shelf_t *)current.p;

            elem_t shelf_key = { .s = shelf->shelf_location };
            elem_t dummy_result;
            
            ioopm_hash_table_remove(db->ht_sl, shelf_key, &dummy_result);

            free(shelf->shelf_location);
            free(shelf); 

            ioopm_list_iterator_advance(it);
        }
        ioopm_list_iterator_destroy(it);
    }

    elem_t name_key = { .s = name };
    elem_t dummy_result;
    
    ioopm_hash_table_remove(db->ht_n, name_key, &dummy_result);

    free(merch->name);
    free(merch->description);

    ioopm_list_destroy(merch->location);
    free(merch);

    return true;
}