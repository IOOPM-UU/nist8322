#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


bool is_positive(char *str) {
    if (str[0] == '-') {
        return false;
    }
    return true;
}


bool is_number(char *str) {
    int start = 0;
    
    // om första tecknet är ett minustecken, hoppa över det när vi kollar siffror
    if (str[0] == '-') {
        start = 1; 
    }

    if (strlen(str) == 0)  {
        return false;
    }
    
    
    
    // om strängen bara är ett minustecken
    if (strlen(str) == 1 && start == 1) {
        return false; 
    }


    for (int i = start; i < strlen(str); i++) {
        if (isdigit(str[i]) == false) {
            return false;
        }
    }
    return true;
}


int gcd(int a, int b) {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}


int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        printf("Användning: %s <tal1> <tal2>\n", argv[0]);
        return 1;
    }

    
    for (int i = 1; i <= 2; i++) {
        if (!is_number(argv[i])) {
            printf("Fel: '%s' är inte ett giltigt tal.\n", argv[i]);
            return 1;
        }
        if (!is_positive(argv[i])) {
            printf("Fel: '%s' är negativt, negativa tal är inte tillåtna.\n", argv[i]);
            return 1;
        }
        
    }

    
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

   
    int result = gcd(a, b);
    printf("gcd(%d, %d) = %d\n", a, b, result);

    return 0;
}