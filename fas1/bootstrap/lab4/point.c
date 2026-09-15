#include <stdio.h>

typedef struct point point_t;

struct point

{
  int x;
  int y;
};

int main(void)
{
    point_t p = { .x = 10, .y = -42};

    printf("point(x=%d,y=%d)\n", p.x, p.y);
    
    return 0;
}