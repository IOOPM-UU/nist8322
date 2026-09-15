#include <stdio.h>
#include <stdlib.h>


typedef int (int_fold_func)(int, int);

int add(int a, int b)
{
  return a + b;
}


int foldl_int_int(int numbers[], int numbers_siz, int_fold_func *f)
{
  int result = 0;

  for (int i = 0; i < numbers_siz; ++i)
  {
    result = f(result, numbers[i]);
  }

  return result;
}

int sum(int numbers[], int numbers_siz)
{
  return foldl_int_int(numbers, numbers_siz, add);
}

int main()
{
  // Skapa en test-array
  int test_numbers[] = {1, 2, 3, 4, 5};
  
  // Arrayens storlek är 5
  int total_sum = sum(test_numbers, 5);
  
  // Skriv ut resultatet
  printf("Summan av arrayen är: %d\n", total_sum);
  
  return 0;
}