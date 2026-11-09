#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include <string.h>
#include "db_backend.h"
#include "linked_list.h"
#include "db_frontend.h"
#include "hash_table_iterator.h"
#include "hash_table.h"
#include "linked_list_private.h"
#include "list_iterator.h"

char ask_question_menu(void) {

    char *valid_options = "AaLlDdEeSsPpCcRr+-=OoUuQq"; 

    while(true) {
        print_menu();
    
        char *answer = ask_question_string("\nVälj ett alternativ: ");

        if (strlen(answer) == 1 && strchr(valid_options, answer[0]) != NULL) {
            char choice = toupper(answer[0]); // toupper gör inget med +, -, =
            free(answer);
            return choice;
        }
        else {
            puts("Ogiltigt val, försök igen");
        }
        free(answer);
    } 
}


void print_menu(void) {
    char *menu = "\n  WEBSHOP MENU  \n"
                 "[A]dd Merchandise\n"
                 "[L]ist Merchandise\n"
                 "[D]elete (Remove) Merchandise\n"
                 "[E]dit Merchandise\n"
                 "[S]how Stock\n"
                 "[P]replenish (Add stock)\n"
                 "[C]reate Cart\n"
                 "[R]emove Cart\n"
                 "[+] Add to Cart\n"
                 "[-] Remove from Cart\n"
                 "[=] Calculate Cost\n"
                 "[O]ut (Checkout)\n"
                 "[Q]uit\n";
    fprintf(stdout, "%s", menu);
}

void frontend_add_merch(db_t *db) {
    char *name = ask_question_string("Enter name: ");
    char *desc = ask_question_string("Enter description: ");
    int price = ask_question_int("Enter price: ");

    if (add_merch_to_db(db, name, desc, price)) {
        puts("Merchandise added successfully!");
    } else {
        puts("Error: Merchandise with that name already exists.");
        free(name);
        free(desc);
    }

    free(name);
    free(desc);
}

// Hjälpfunktion för qsort som jämför namnen på två varor
static int cmp_merch(const void *a, const void *b) {
    merch_t *m1 = *(merch_t **)a;
    merch_t *m2 = *(merch_t **)b;
    return strcmp(m1->name, m2->name);
}

void frontend_list_merch(db_t *db) {
    // Hur många varor som finns i databasen
    int count = 0;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(db->ht_n);
    while (!ioopm_hash_table_iterator_at_end(it)) {
        count++;
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    if (count == 0) {
        puts("Det finns inga varor i databasen.");
        return;
    }

    // Temporär array och med pekare till alla varor
    merch_t **merches = calloc(count, sizeof(merch_t *));
    it = ioopm_hash_table_iterator_create(db->ht_n);
    
    for (int i = 0; i < count; i++) {
        elem_t val = ioopm_hash_table_iterator_current_value(it);
        merches[i] = (merch_t *)val.p;
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    // Sortera arrayen i alfabetisk ordning
    qsort(merches, count, sizeof(merch_t *), cmp_merch);

    for (int i = 0; i < count; i++) {

        printf("%d. %s (%d kr)\n", i + 1, merches[i]->name, merches[i]->price);

        // Modulo (%) 20 kollar om vi har skrivit ut en multipel av 20 varor.
        if ((i + 1) % 20 == 0 && i < count - 1) {
            char *ans = ask_question_string("Tryck Enter (eller vad som helst) för mer, eller 'N' för att avbryta: ");
            
            if (strlen(ans) > 0 && toupper(ans[0]) == 'N') {
                free(ans);
                break; 
            }
            free(ans);
        }
    }

    free(merches);
}

void frontend_remove_merch(db_t *db) {
    char *name = ask_question_string("Vilken vara vill du ta bort?: ");
    char *confirmation = ask_question_string("Är du säker?: [Y/N] ");
    
    if (strlen(confirmation) > 0 && toupper(confirmation[0]) == 'Y') {
    
        if (remove_merch_from_db(db, name)) {
            puts("Varan har tagits bort!");
        } else {
            puts("Fel: Varan fanns inte i databasen");
        }
    } else {
        puts("Avbryter borttagning");
    }

    free(name);
    free(confirmation);
}

void frontend_edit_merch(db_t *db) {
    char *old_name = ask_question_string("Vilken vara vill du redigera?: ");
    char *new_name = ask_question_string("Nytt namn: ");
    char *new_desc = ask_question_string("Ny beskrivning: ");
    int new_price = ask_question_int("Nytt pris: ");
    char *confirmation = ask_question_string("Är du säker? [Y/N] ");

    if (strlen(confirmation) > 0 && toupper(confirmation[0]) == 'Y') {
        if (edit_db(db, old_name, new_name, new_desc, new_price)) {
            puts("Varan har redigerats!");
        } else {
            puts("Fel: Varan fanns inte i databasen");
        }
    } else {
        puts("Avbryter redigering");
    }

    free(old_name);
    free(confirmation);
    free(new_name);
    free(new_desc);
}

void frontend_show_stock(db_t *db) {
    elem_t result;
    char *name = ask_question_string("Vilken vara vill du visa stock för?: ");
    bool exists = ioopm_hash_table_lookup(db->ht_n, ptr_elem(name), &result);

    if (exists) {
        merch_t *merch = (merch_t *)result.p;
        size_t len = ioopm_list_size(merch->location);
        if (len == 0) {
            puts("Varan finns inte i lager");
        } else {
            ioopm_list_iterator_t *it_l = ioopm_list_iterator_create(merch->location);
            while (!ioopm_list_iterator_at_end(it_l)) {
                elem_t current = ioopm_list_iterator_current(it_l);
                shelf_t *shelf_elem = (shelf_t *)current.p;

                printf("Hylla: %s, Antal: %d\n", shelf_elem->shelf_location, shelf_elem->quantity);

                ioopm_list_iterator_advance(it_l);
            }
            ioopm_list_iterator_destroy(it_l);
        }

    } else {
        puts("Fel: Varan finns inte.");
    }

    free(name);
}

void frontend_replenish(db_t *db) {
    char *shelf = ask_question_string("Vilken hylla vill du fylla på? (t.ex A20): ");
    char *name = ask_question_string("Vilken vara vill du fylla på?: ");
    int amount = ask_question_int("Hur många vill du lägga till?: ");

    if (amount > 0) {
        if (add_stock(db, name, amount, shelf)) {
            puts("Lagret har fyllts på!");
    
        } else {
            puts("Fel: Kunde inte fylla på lagret (finns varan?)");
            free(shelf);
        }
    } else {
        puts("Fel: Antalet måste vara minst 1");
        free(shelf);
    }

    free(name);
    free(shelf);
}

void frontend_create_cart(db_t *db) {
    int cart_id = create_cart(db); 
    printf("Kundvagn skapad med ID: %d\n", cart_id);
}

void frontend_add_to_cart(db_t *db) {
    int cart_id = ask_question_int("Vilket kundvagns-ID vill du lägga till i?: ");
    char *name = ask_question_string("Vilken vara vill du lägga till?: ");
    int amount = ask_question_int("Hur många vill du lägga till?: ");

    if (amount > 0) {

        if (add_to_cart(db, cart_id, name, amount)) {
            puts("Varan har lagts till i kundvagnen!");
        } else {
            puts("Fel: Kunde inte lägga till varan (finns vagnen, varan, och lagersaldot?).");
        }
    } else {
        puts("Fel: Antalet måste vara minst 1.");
    }

    free(name); 
}

void frontend_remove_from_cart(db_t *db) {
    int cart_id = ask_question_int("Vilket kundvagns-ID vill du ta bort från?: ");
    char *name = ask_question_string("Vilken vara vill du ta bort?: ");
    int amount = ask_question_int("Hur många vill du ta bort?: ");

    if (amount > 0) {

        if (remove_from_cart(db, cart_id, name, amount)) {
            puts("Varan har tagits bort från kundvagnen!");
        } else {
            puts("Fel: Kunde inte ta bort varan.");
        }
    } else {
        puts("Fel: Antalet måste vara minst 1.");
    }

    free(name); 
}

void frontend_remove_cart(db_t *db) {
    int cart_id = ask_question_int("Vilket kundvagns-ID vill du ta bort?: ");

    if (remove_cart(db, cart_id)) {
        puts("Kundvagnen har tagits bort!");
    } else {
        puts("Fel: Kundvagnen fanns inte.");
    }
}

void frontend_calculate_cost(db_t *db) {
    int cart_id = ask_question_int("Vilket kundvagns-ID vill du räkna ut totalkostnaden för?: ");

    elem_t dummy_result;
    elem_t cart_key = { .i = cart_id };
    bool exists = ioopm_hash_table_lookup(db->carts, cart_key, &dummy_result);

    if(exists) {
        int cost = calculate_cost(db, cart_id);
        printf("Den totala kostnaden för vagnen är %d kr\n", cost);
    } else {
        puts("Vagnen finns ej!");
    }
} 

void frontend_checkout(db_t *db) {
    int cart_id = ask_question_int("Vilket kundvagns-ID vill du checka-ut?: ");
    if (checkout_cart(db, cart_id)) {
        puts("Köp genomfört! Vagnen är nu borttagen");
        }
        else {
            puts("Fel: Köpet misslyckades. Kontrollera lagersaldo eller om vagnen finns");
        }

}

void frontend_quit(bool *running) {
    char *confirm = ask_question_string("Are you sure you want to quit? (Y/N): ");
    if (toupper(confirm[0]) == 'Y') {
        *running = false;
    }
    free(confirm);
}

void event_loop(db_t *db) {
    bool running = true;
    
    while (running) {
        char val = ask_question_menu(); 

        if (val == 'A') {
            frontend_add_merch(db);
        } 
        else if (val == 'L') {
            frontend_list_merch(db);
        }
        else if (val == 'D') {
            frontend_remove_merch(db);
        }
        else if (val == 'E') {
            frontend_edit_merch(db);
        }
        else if (val == 'S') {
            frontend_show_stock(db);
        }
        else if (val == 'P') {
            frontend_replenish(db);
        }
        else if (val == 'C') {
            frontend_create_cart(db);
        }
        else if (val == 'R') {
            frontend_remove_cart(db);
        }
        else if (val == '+') {
            frontend_add_to_cart(db);
        }
        else if (val == '-') {
            frontend_remove_from_cart(db);
        }
        else if (val == '=') {
            frontend_calculate_cost(db);
        }
        else if (val == 'O') {
            frontend_checkout(db);
        }
        else if (val == 'Q') {
            frontend_quit(&running);
        }
    }
}