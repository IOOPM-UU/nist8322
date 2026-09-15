#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int tal = atoi(argv[1]);
    int ar_primtal = 1; 

    for (int a = 1; a <= tal - 1; a++) 
    {
        for (int b = 1; b <= tal - 1; b++) 
        {
            if (a * b == tal) {
                ar_primtal = 0;
            }
        }
    }

    
    if (ar_primtal == 0) {
        puts("inte primtal");
    } else {
        puts("primtal");
    }

    return 0;
}