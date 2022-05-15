#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


#define MAX 5

typedef struct stack {
    int top;
    int s[MAX];
}STACK;

void
initStack(STACK *s) {
    s->top = -1;
    memset(s->s, 0, sizeof(s->s));
}

bool isEmpty(STACK *s) {
    return (0 > s->top) ? true : false;
}

bool isFull(STACK *s) {
    return (MAX > (s->top + 1)) ? false : true;
}

int push (STACK *s, int data) {
    if (isFull(s)) {
        return -1;
    }
    s->s[++s->top] = data;
    return s->s[s->top];
}

int pop (STACK *s) {
    if (isEmpty(s)) {
        return -1;
    }
    return (s->s[s->top--]);
}

int size (STACK *s) {
    return (s->top);
}

int main () {
    STACK s;

    initStack(&s);
    printf("%d \r\n", push(&s, 1));
    printf("%d \r\n", push(&s, 2));
    printf("%d \r\n", push(&s, 3));
    printf("%d \r\n", push(&s, 4));
    printf("%d \r\n", push(&s, 5));
    printf("%d \r\n", push(&s, 6));
    printf("%d \r\n", pop(&s));
    printf("%d \r\n", pop(&s));
    printf("%d \r\n", pop(&s));
    printf("%d \r\n", pop(&s));
    printf("%d \r\n", pop(&s));
    printf("%d \r\n", pop(&s));
    printf("%d \r\n", pop(&s));

    return;
}