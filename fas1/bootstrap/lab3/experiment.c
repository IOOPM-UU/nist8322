#include "../lab2/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern char *strdup(const char *);

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;

typedef bool (check_func)(char *);
typedef answer_t (convert_func)(char *);


answer_t ask_question(char *question, check_func *check, convert_func *convert) {
    int buf_siz = 255;
    char answer[255];

    do {
        printf("%s\n", question);
        
     
        read_string(answer, buf_siz); 

    } while (check(answer) != true); 

    return convert(answer);
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
  return answer.int_value; // svaret som ett heltal
}

answer_t make_float(char *str)
{
  return (answer_t) { .float_value = atof(str) };
}

double ask_question_float(char *question)
{
  return ask_question(question, is_float, make_float).float_value;
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}



int main()
{
  
}