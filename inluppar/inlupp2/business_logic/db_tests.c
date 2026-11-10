#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include <stddef.h>
#include "db_backend.h"

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_is_shelf(void) {

    CU_ASSERT_TRUE(is_shelf("A25"));
    CU_ASSERT_TRUE(is_shelf("Z999"));
    CU_ASSERT_TRUE(is_shelf("B0"));

    CU_ASSERT_FALSE(is_shelf("A"));
    CU_ASSERT_FALSE(is_shelf(""));

    CU_ASSERT_FALSE(is_shelf("125"));
    CU_ASSERT_FALSE(is_shelf("-A25"));

    CU_ASSERT_FALSE(is_shelf("AB25"));
    CU_ASSERT_FALSE(is_shelf("A25B"));
    CU_ASSERT_FALSE(is_shelf("A 25"));
}

void test_add_to_empty(void) {
    db_t *db = db_create();
    char *name = "Chair";
    char *desc = "White chair";
    int price = 100000;
    bool success = add_merch_to_db(db, name, desc, price);
    CU_ASSERT_TRUE(success);
    size_t db_size = ioopm_hash_table_size(db->ht_n);
    CU_ASSERT_EQUAL(db_size, 1);
    db_destroy(db); 
}

void test_edit_db(void) {
    db_t *db = db_create();
    char *name = "Chair";
    char *desc = "White chair";
    int price = 100000;
    bool success1 = add_merch_to_db(db, name, desc, price);
    CU_ASSERT_TRUE(success1);
    char *new_name = "Table";
    char *new_desc = "Black table";
    int new_price = 200000;
    edit_db(db, name, new_name, new_desc, new_price);
    merch_t *old_merch = get_merch(db, name);
    CU_ASSERT_PTR_NULL(old_merch);
    merch_t *edited_merch = get_merch(db, new_name);
    CU_ASSERT_PTR_NOT_NULL(edited_merch);
    if (edited_merch != NULL) {
        CU_ASSERT_STRING_EQUAL(edited_merch->description, new_desc);
        CU_ASSERT_EQUAL(edited_merch->price, new_price);
    }
    db_destroy(db);
}

void test_add_stock(void) {
    db_t *db = db_create();
    char *name = "Desk";
    add_merch_to_db(db, name, "Wooden desk", 500);
    bool stock_success = add_stock(db, name, 10, "A25");
    CU_ASSERT_TRUE(stock_success);
    int total = get_total_stock(db, name);
    CU_ASSERT_EQUAL(total, 10);
    db_destroy(db);
}

void test_add_merch(void) {
    db_t *db = db_create();
    CU_ASSERT_TRUE(add_merch_to_db(db, "Kritor", "Vita kritor", 20));
    CU_ASSERT_FALSE(add_merch_to_db(db, "Kritor", "Färgglada kritor", 35));
    merch_t *merch = get_merch(db, "Kritor");
    CU_ASSERT_PTR_NOT_NULL(merch);
    CU_ASSERT_STRING_EQUAL(merch->description, "Vita kritor");
    db_destroy(db);
}

void test_remove_merch(void) {
    db_t *db = db_create();
    add_merch_to_db(db, "Bok", "Bra bok", 100);
    add_stock(db, "Bok", 10, "A25");
    CU_ASSERT_TRUE(remove_merch_from_db(db, "Bok"));
    CU_ASSERT_PTR_NULL(get_merch(db, "Bok"));
    db_destroy(db);
}

void test_checkout_cart(void) {
    db_t *db = db_create();
    add_merch_to_db(db, "Bok", "Bra bok", 100);
    add_stock(db, "Bok", 10, "A25");
    int cart_id = create_cart(db);
    add_to_cart(db, cart_id, "Bok", 4);
    CU_ASSERT_TRUE(checkout_cart(db, cart_id));
    CU_ASSERT_EQUAL(calculate_cost(db, cart_id), 0);
    db_destroy(db);
}

void test_edit_db_edge_cases(void) {
    db_t *db = db_create();
    add_merch_to_db(db, "Apple", "Fruit", 10);
    CU_ASSERT_TRUE(edit_db(db, "Apple", "Pear", "Green", 15));
    add_merch_to_db(db, "Banana", "Yellow", 5);
    CU_ASSERT_FALSE(edit_db(db, "Pear", "Banana", "Test", 20));
    CU_ASSERT_FALSE(edit_db(db, "Ghost", "X", "Y", 1));
    db_destroy(db);
}

void test_add_stock_edge_cases(void) {
    db_t *db = db_create();
    add_merch_to_db(db, "Apple", "Fruit", 10);
    CU_ASSERT_TRUE(add_stock(db, "Apple", 10, "A25"));
    CU_ASSERT_TRUE(add_stock(db, "Apple", 5, "A25"));
    add_merch_to_db(db, "Banana", "Yellow", 5);
    CU_ASSERT_FALSE(add_stock(db, "Banana", 10, "A25"));
    db_destroy(db);
}

void test_checkout_cart_edge_cases(void) {
    db_t *db = db_create();
    add_merch_to_db(db, "Apple", "Fruit", 10);
    add_stock(db, "Apple", 10, "A25");
    int cart_id = create_cart(db); 
    CU_ASSERT_TRUE(add_to_cart(db, cart_id, "Apple", 5));
    CU_ASSERT_TRUE(checkout_cart(db, cart_id));
    db_destroy(db);
}

void test_remove_merch_edge_cases(void) {
    db_t *db = db_create();
    add_merch_to_db(db, "Apple", "Fruit", 10);
    CU_ASSERT_FALSE(remove_merch_from_db(db, "Ghost"));
    int cart_id = create_cart(db);
    add_stock(db, "Apple", 10, "A25");
    add_to_cart(db, cart_id, "Apple", 5);
    CU_ASSERT_FALSE(remove_merch_from_db(db, "Apple")); 
    db_destroy(db);
}

void test_remove_from_cart(void) {
    db_t *db = db_create();
    
    add_merch_to_db(db, "cola", "kolsyrad dryck", 12);
    add_stock(db, "cola", 10, "A12");
    int cart_id = create_cart(db);
    
    add_to_cart(db, cart_id, "cola", 5);
    CU_ASSERT_EQUAL(get_cart_reserved_stock(db, "cola"), 5);
    
    CU_ASSERT_TRUE(remove_from_cart(db, cart_id, "cola", 2));
    CU_ASSERT_EQUAL(get_cart_reserved_stock(db, "cola"), 3);
    
    CU_ASSERT_FALSE(remove_from_cart(db, cart_id, "cola", 10));
    CU_ASSERT_EQUAL(get_cart_reserved_stock(db, "cola"), 3);

    CU_ASSERT_TRUE(remove_from_cart(db, cart_id, "cola", 3));
    CU_ASSERT_EQUAL(get_cart_reserved_stock(db, "cola"), 0);

    CU_ASSERT_FALSE(remove_from_cart(db, cart_id, "cola", 1));
    
    db_destroy(db);
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite db_test_suite = CU_add_suite("Data Base Tests", init_suite, clean_suite);
    if (db_test_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (CU_add_test(db_test_suite, "Add merch to empty DB", test_add_to_empty) == NULL) ||
        (CU_add_test(db_test_suite, "Edit DB", test_edit_db) == NULL) ||
        (CU_add_test(db_test_suite, "Add stock", test_add_stock) == NULL) ||
        (CU_add_test(db_test_suite, "Add merch", test_add_merch) == NULL) ||
        (CU_add_test(db_test_suite, "Remove merch", test_remove_merch) == NULL) ||
        (CU_add_test(db_test_suite, "Checkout cart", test_checkout_cart) == NULL) ||
        (CU_add_test(db_test_suite, "Edit DB edge cases", test_edit_db_edge_cases) == NULL) ||
        (CU_add_test(db_test_suite, "Add stock edge cases", test_add_stock_edge_cases) == NULL) ||
        (CU_add_test(db_test_suite, "Checkout cart edge cases", test_checkout_cart_edge_cases) == NULL) ||
        (CU_add_test(db_test_suite, "Remove merch edge cases", test_remove_merch_edge_cases) == NULL) ||
        (CU_add_test(db_test_suite, "Remove merch from cart", test_remove_from_cart) == NULL) ||
        (CU_add_test(db_test_suite, "Is shelf", test_is_shelf) == NULL)
    ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}