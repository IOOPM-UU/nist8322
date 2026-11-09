#pragma once
#include "db_backend.h"

/// @brief Prompts the user with the main menu and retrieves their choice
/// @return a character representing the user's selected menu option
char ask_question_menu(void);

/// @brief Displays the available main menu options to the user in the terminal
void print_menu(void);

/// @brief Starts the main event loop for the interactive webstore interface
/// @param db the database operated upon
void event_loop(db_t *db);

/// @brief Prints the details (name, description, price, stock) of a specific merchandise item
/// @param merch pointer to the merchandise item to be printed
void print_merch(merch_t *merch);

/// @brief Lists a specified number of merchandise items in the terminal
/// @param merchs an array of merchandise items to display
/// @param no_merchs the number of merchandise items to display from the array
void list_db(merch_t *merchs, int no_merchs);

/// @brief Prompts the user for a shelf location and validates its format (e.g., a letter followed by numbers)
/// @param question the prompt string displayed to the user
/// @return a dynamically allocated string containing the validated shelf location
char *ask_question_shelf(char *question);