#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Check for Balanced Parentheses
    Problem Statement: Check Balanced Parentheses.
    Given string str containing just the characters ‘(‘, ‘)’, ‘{‘, ‘}’, ‘[‘ and ‘]’,
    check if the input string is valid and return true if the string is balanced otherwise return false.

    Note: string str is valid if:

    Open brackets must be closed by the same type of brackets.
    Open brackets must be closed in the correct order.
    Example 1:
        Input: str = “( )[ { } ( ) ]”
        Output: True

        Explanation: As every open bracket has its corresponding
        close bracket. Match parentheses are in correct order
        hence they are balanced.
    Example 2:
    Input: str = “[ ( )”
    Output: False

        Explanation: As ‘[‘ does not have ‘]’ hence it is
        not valid and will return false.
*/


#define MAX 1000

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

//isEmpty: Returns true if the stack is empty, else false.
bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

//Push: Adds an item in the stack. If the stack is full, then it is said to be an Overflow condition.
void push(STACK *s, char value) {
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    printf("Element %c is pushed to stack \r\n", value);
    s->stack[++s->top] = value;
    return;
}

// Peek or Top: Returns the top element of the stack.
char peek(STACK *s) {
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return -1;
    }

    return (s->stack[s->top]);
}

char size(STACK *s) {
    return ((s->top) + 1);
}


// Pop: Removes an item from the stack. The items are popped in the reversed order in which they are pushed.
// If the stack is empty, then it is said to be an Underflow condition.
char pop(STACK *s) {
    if (isStackEmpty(s)) {
        return -1;
    }

    printf("Element %c is poped out of stack \r\n", s->stack[s->top]);
    return (s->stack[s->top--]);
}

bool isValid(char expr[]) {

    int len = strlen(expr);
    bool result = false;
    STACK *s =  initStack();

    for (int i = 0; i < len; i++) {
        if(expr[i] == '(' || expr[i] == '{' || expr[i] == '[') {
            push(s, expr[i]);
        } else {
            if(size(s) == 0) {
                 return false;
            }
            char ch = pop(s);
            if((expr[i] == ')' && ch == '(') ||
               (expr[i] == ']' && ch == '[') ||
               (expr[i] == '}' && ch == '{')) {
                   continue;
            } else {
                return false;
            }
        }
    }

}

int main()
{
    char s[] = "()[{}()]";

    printf("The Give expression : %s is %s \r\n", s, isValid(s) ? "True" : "False");
    return 0;
}