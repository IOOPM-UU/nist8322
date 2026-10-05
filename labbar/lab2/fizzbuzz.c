#include <stdio.h>
#include <stdlib.h>

void print_number(int num) {
    if (num % 3 == 0 && num % 5 == 0) {
        printf("Fizz Buzz");
    } else if (num % 3 == 0) {
        printf("Fizz");
    } else if (num % 5 == 0) {
        printf("Buzz");
    } else {
        printf("%d", num);
    }
}

int main(int argc, char *argv[])
{
    int T = atoi(argv[1]);

    for (int i = 0; i < T - 1; i++) {
        int current_number = i + 1;
        
        
        print_number(current_number); 
        
        
        printf(", "); 
    }
    
    
    print_number(T);
    printf("\n");
}