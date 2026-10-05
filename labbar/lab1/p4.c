#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int total = 0;
  int rows = atoi(argv[1]);
  int growth = atoi(argv[2]);

  for (int i = 1; i <= rows; i++) 
  {                       
    for (int j = 0; j < (i * growth); j++) {
      printf("*");
    }
    printf("\n");  
    total = total + (i * growth);
  }
  printf("Totalt: %d\n", total);
  return 0;
}