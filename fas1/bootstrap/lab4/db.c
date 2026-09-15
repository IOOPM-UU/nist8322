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

int main(int argc, char *argv[])
{
  char *array1[] = {"Kaffe", "Skruvmejsel", "Vattenfall"};
  char *array2[] = {"Klocka", "Banan", "Tegelsten"};
  char *array3[] = {"Algoritm", "Flygplan", "Kantarell"};

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16];
    int db_siz = 0;

    int items = atoi(argv[1]);

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1;
    }

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3, 3);
        char *desc = magick(array1, array2, array3, 3);
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, strdup(shelf));

        db[db_siz] = item;
        ++db_siz;
      }


    list_db(db, db_siz);

    edit_db(db, db_siz);


    printf("\n Uppdaterad lista \n");
    list_db(db, db_siz);

  }
  return 0;
}   