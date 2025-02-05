#include <stdio.h>
#include <stdlib.h>

static struct test_struct {
    int a;
    char *b;
} *test ;


void main(){
    test = (struct test_struct *)malloc(sizeof(struct test_struct));
    test->a = 1;
    test->b = "hello";
    printf("test->a = %d\n", test->a);
    printf("test->b = %s\n", test->b);
    struct test_struct *p = test;
    p->a = 10;
    p->b = "world";
    printf("test->a = %d\n", test->a);
    printf("test->b = %s\n", test->b);
}