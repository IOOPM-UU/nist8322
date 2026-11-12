#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include <string.h>


struct item
{
  char * name;
  char * description;
  int price;
  char * shelf;

};

typedef struct item item_t;

void print_item(item_t *item) {
    printf(" Name: %s\n Desc: %s\n Price: %d.%02d SEK\n Shelf: %s\n", 
           item->name, 
           item->description, 
           item->price / 100,
           item->price % 100,
           item->shelf);
}

item_t make_item(char * name, char * description, int price, char * shelf) {
    
    item_t i = { .name = name, .description = description, .price = price, .shelf = shelf };
    return i;
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


char *ask_question_shelf(char *question) {
    
    return ask_question(question, is_shelf, (convert_func *) strdup).string_value;
}

item_t input_item(void) {
    char * name = ask_question_string("Name:");
    char * description = ask_question_string("Desc:");
    int price = ask_question_int("Price:");
    char * shelf = ask_question_shelf("Shelf:");

    item_t new_item = { .name = name, .description = description, .price = price, .shelf = shelf };
    return new_item;
    
}

char *magick(char *array1[], char *array2[], char *array3[], int length) {
    
    char buf[255]; 

    int i1 = rand() % length;
    int i2 = rand() % length;
    int i3 = rand() % length;

    snprintf(buf, 255, "%s-%s %s", array1[i1], array2[i2], array3[i3]);

    return strdup(buf);
}

void list_db(item_t *items, int no_items) {
    for (int i = 0; i < no_items; i++) {
        
        printf("%d. %s\n", i + 1, items[i].name);
    }
}

void edit_db(item_t *db, int db_siz) {
    int index;
    
    do {
        index = ask_question_int("Vilken vara vill du editera? (Ange siffra)");
    } while (index < 1 || index > db_siz);
    
    int array_index = index - 1;
    
    printf("\nDu har valt att editera följande vara:\n");
    print_item(&db[array_index]);
    
    printf("\n Skriv in nya detaljer för varan \n");
    db[array_index] = input_item();
    
    printf("\nVaran har uppdaterats!\n");
}

void print_menu(void) {
    char *menu = "\n[L]ägga till en vara\n"
                 "[T]a bort en vara\n"
                 "[R]edigera en vara\n"
                 "Ån[g]ra senaste ändringen\n"
                 "Lista [h]ela varukatalogen\n"
                 "[A]vsluta\n";
    fprintf(stdout, "%s", menu);
}

char ask_question_menu(void) {
    char *valid_options = "LlTtRrGgHhAa";

    while(true) {
        print_menu();
    
        char *answer = ask_question_string("Välj ett alternativ: ");

        if (strlen(answer) == 1 && strchr(valid_options, answer[0]) != NULL) {
            char choice = toupper(answer[0]);
            free(answer);
            return choice;
        }
        else {
            puts("Ogiltigt val, försök igen");
        }
        free(answer);
    } 
}


void add_item_to_db(item_t *db, int *db_siz) 
{
    db[*db_siz] = input_item();
    (*db_siz)++;
    printf("\nVaran har lagts till i databasen!\n");
}

void remove_item_from_db(item_t *db, int *db_siz) {
    int index;
    
    do {
        index = ask_question_int("Vilken vara vill du ta bort? (Ange siffra)");
    } while (index < 1 || index > *db_siz);
    
    int array_index = index - 1;
    
    printf("\nDu har valt att ta bort följande vara:\n");
    print_item(&db[array_index]);
    

    for (int i = array_index; i < *db_siz - 1; i++) {
        db[i] = db[i + 1];
    }
    
    (*db_siz)--;
    
    printf("\nVaran har tagits bort!\n\n");
    
    list_db(db, *db_siz);
}


void event_loop(item_t *db, int *db_siz) {
  char val;
  
  do {

    val = ask_question_menu();

    if (val == 'L') {
      add_item_to_db(db, db_siz);
    } 
    else if (val == 'T') {
      remove_item_from_db(db, db_siz);
    }
    else if (val == 'R') {
      edit_db(db, *db_siz);
    }
    else if (val == 'G') {
      printf("Not yet implemented!");
    }
    else if (val == 'H') {
      list_db(db, *db_siz);
    }
    else if (val == 'A') {
      break;
    }

  } while (val != 'A');
}


int main(int argc, char *argv[])
{
    
    item_t db[16];
    int db_siz = 0;

    
    printf("Välkommen till databasen!\n");
    event_loop(db, &db_siz);

   
    printf("Avslutar programmet...\n");
    return 0;
}