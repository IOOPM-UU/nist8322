#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

int main(void) {
    srandom(time(NULL));
    int T = random() % 1024;
    
    
    char *name = ask_question_string("Vad heter du? ");
   
    
    char question_buf[255];
    snprintf(question_buf, 255, "Du %s, jag tänker på ett tal kan du gissa vilket? ", name);
    
    int guess = ask_question_int(question_buf);
    int counter = 1;

    while (guess != T) {
        if (guess < T) {
            guess = ask_question_int("För litet! Gissa igen: ");
        } else if (guess > T) {
            guess = ask_question_int("För stort! Gissa igen: ");
        }
        counter++;
    }
    
    printf("Bingo!\n");
    printf("Det tog %s %d gissningar att komma fram till %d\n", name, counter, T);
    
    free(name);
    
    return 0;
}