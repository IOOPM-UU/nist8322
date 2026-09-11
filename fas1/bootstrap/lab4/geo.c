#include <stdio.h>
#include <stdbool.h>

struct point
{
  int x;
  int y;
};



typedef struct rectangle rectangle_t;

typedef struct point point_t;

struct rectangle
{
    point_t upper_left;
    point_t lower_right;
};

void translate(point_t *p1, point_t *p2)
{
  p1->x += p2->x;
  p1->y += p2->y;
}

void print_point(point_t *p) 
{
    printf("(%d,%d)\n", p->x, p->y);
}


point_t make_point(int x, int y)
{
    point_t p = { .x = x, .y = y };
    return p;
}

void print_rect(rectangle_t *r)
{
    printf("rectangle(upper_left=");
    print_point(&(r->upper_left));
    printf(", lower_right=");
    print_point(&(r->lower_right));
    printf(")\n");
}

rectangle_t make_rect(int x1, int y1, int x2, int y2)
{
    return (rectangle_t){
        .upper_left = make_point(x1, y1),
        .lower_right = make_point(x2, y2)
    };
}

int area_rect(rectangle_t *r)
{
    int width = r->lower_right.x - r->upper_left.x;
    int height = r->upper_left.y - r->lower_right.y;
    
    return width * height;
}

bool intersects_rect(rectangle_t *r1, rectangle_t *r2)
{
    
    if (r1->lower_right.x < r2->upper_left.x || r1->upper_left.x > r2->lower_right.x) {
        return false;
    }

    if (r1->lower_right.y > r2->upper_left.y || r1->upper_left.y < r2->lower_right.y) {
        return false;
    }

    return true;
}

rectangle_t intersection_rect(rectangle_t *r1, rectangle_t *r2)
{

    int x1 = (r1->upper_left.x > r2->upper_left.x) ? r1->upper_left.x : r2->upper_left.x;
    
    int y1 = (r1->upper_left.y < r2->upper_left.y) ? r1->upper_left.y : r2->upper_left.y;
    
    int x2 = (r1->lower_right.x < r2->lower_right.x) ? r1->lower_right.x : r2->lower_right.x;
    
    int y2 = (r1->lower_right.y > r2->lower_right.y) ? r1->lower_right.y : r2->lower_right.y;

    return make_rect(x1, y1, x2, y2);
}

int main(void) {
    point_t p1 = { .x = 10, .y = 5 };
    point_t p2 = { .x = 2, .y = 3 };

    
    printf("Innan flytt:\n");
    print_point(&p1);

    translate(&p1, &p2);

    printf("Efter flytt:\n");
    print_point(&p1);

    return 0;
}