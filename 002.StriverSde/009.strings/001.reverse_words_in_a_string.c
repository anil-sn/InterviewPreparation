#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX 10000

typedef struct stack_ {
    char stack[MAX];
    int top;
    int capacity;
} STACK;

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAX;
    return stack;
}

bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

void push(STACK *s, char value) {
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    printf("Element %d is pushed to stack \r\n", value);
    s->stack[++s->top] = value;
    return;
}

char peek(STACK *s) {
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return -1;
    }

    return (s->stack[s->top]);
}

char pop(STACK *s) {
    if (isStackEmpty(s)) {
        return -1;
    }

    return (s->stack[s->top--]);
}

int main(void)
{
    char s[]="TUF is great for interview preparation";
    int len = strlen(s);

    int left = 0;
    int right = len-1;
    
    char temp[MAX] = {'\0'};
    char result[MAX] = {'\0'};
    char ans[MAX] = {'\0'};
    int pos = 0;
    
     printf("%s\r\n", s);
    //Iterate the string and keep on adding to form a word
    //If empty space is encountered then add the current word to the result
    while (left <= right) {
        char ch= s[left];
        if (ch != ' ') {
            temp[pos++] = ch;
        } else if (ch == ' ') {
            temp[pos++] = '\0';
            sprintf((char *)result, (const char *)"%s %s", temp, ans);
            pos = 0;
            sprintf((char *)ans, (const char *)"%s", result);
        }
        left++;
    }
    
    //If not empty string then add to the result(Last word is added)
    if (pos != 0) {
        temp[pos++] = '\0';
        sprintf((char *)result, (const char *)"%s %s", temp, ans);
        sprintf((char *)ans, (const char *)"%s", result);
    }
    
    printf("%s\r\n", ans);
    return 0;
}