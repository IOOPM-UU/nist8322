#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"


int read_string(char *buf, int buf_siz)
{
    int count = 0;
    
    
    int c = getchar(); 

    
    while (c != '\n' && c != EOF) {
        
        
        if (count < buf_siz - 1) {
            buf[count] = c; 
            count++;        
        }
        
        
        c = getchar();
    }

    
    buf[count] = '\0';

    
    return count;
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



bool is_float(char *str) {
    if (str == NULL || *str == '\0') {
        return false;
    }

    int i = 0;
    bool has_dot = false;
    bool has_digits = false;

    
    if (str[i] == '-') {
        i++;
    }

    while (str[i] != '\0') {
        
        if (str[i] == '\n') {
            break;
        }

        if (str[i] == '.') {
            if (has_dot) {
                return false;
            }
            has_dot = true;
        } else if (str[i] >= '0' && str[i] <= '9') {
            has_digits = true;
        } else {
            return false;
        }
        i++;
    }

    return has_digits;
}

extern char *strdup(const char *);

answer_t make_float(char *str)
{
  return (answer_t) { .float_value = atof(str) };
}

bool not_empty(char *str) {
    
    if (str == NULL || str[0] == '\0') {
        return false;
    }
    
    
    if (str[0] == '\n' && str[1] == '\0') {
        return false;
    }
    
    return true;
}


bool is_number(char *str) {
    if (str == NULL || str[0] == '\0') {
        return false;
    }

    int i = 0;
    bool has_digits = false;

    
    if (str[i] == '-') {
        i++;
    }

    
    while (str[i] != '\0') {
        
        if (str[i] == '\n') {
            break;
        }

        if (str[i] >= '0' && str[i] <= '9') {
            has_digits = true;
        } else {
            return false;
        }
        i++;
    }

    return has_digits;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert) {
    int buf_siz = 255;
    char answer[255];

    do {
        printf("%s\n", question);
        
     
        read_string(answer, buf_siz); 

    } while (check(answer) != true); 

    return convert(answer);
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
  return answer.int_value; // svaret som ett heltal
}

double ask_question_float(char *question)
{
    return ask_question(question, is_float, make_float).float_value;
}