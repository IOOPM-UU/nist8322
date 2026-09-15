#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int string_length(char *str) {
    int counter = 0;

    while (str[counter] != '\0') {
        counter = counter + 1;
    }
    
    return counter;
}

void print(char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        putchar(s[i]);
    }
}

void println(char *s) {
    print(s);
    putchar('\n');
}

/* int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s words or string", argv[0]);
  }
  else
  {
    for (int i = 1; i < argc; ++i)
    {
      int expected = strlen(argv[i]);
      int actual   = string_length(argv[i]);
      printf("strlen(\"%s\")=%d\t\tstring_length(\"%s\")=%d\n",
             argv[i], expected, argv[i], actual);
    }
  }
  return 0;
}
*/

int main() {
    // Testar den inbyggda funktionen
    puts("Detta skrivs ut med inbyggda puts");
    
    // Testar din egen funktion
    println("Detta skrivs ut med min egen println");
    
    return 0;
}