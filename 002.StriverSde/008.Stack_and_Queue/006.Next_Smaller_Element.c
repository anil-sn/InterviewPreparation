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
    You are given an array 'ARR' of integers of length N.
    Your task is to find the next smaller element for each of the array elements.
    Next Smaller Element for an array element is the first element to the right of that
    element which has a value strictly smaller than that element.
    If for any array element the next smaller element does not exist,
    you should print -1 for that array element.

    For Example:
    If the given array is [ 8, 3, 4, 2, 3, 1 ], we need to return [1, 1, -1].
    Because for  2, 1 is the Next Smaller element. For 3, 1 is the Next Smaller element and for 1,
    there is no next smaller element hence the answer for this element is -1.
*/

#define MAX 1000

typedef struct stack_ {
    int stack[MAX];
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
void push(STACK *s, int value) {
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    printf("Element %d is pushed to stack \r\n", value);
    s->stack[++s->top] = value;
    return;
}

// Peek or Top: Returns the top element of the stack.
int peek(STACK *s) {
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return -1;
    }

    return (s->stack[s->top]);
}

int size(STACK *s) {
    return ((s->top) + 1);
}


// Pop: Removes an item from the stack. The items are popped in the reversed order in which they are pushed.
// If the stack is empty, then it is said to be an Underflow condition.
int pop(STACK *s) {
    if (isStackEmpty(s)) {
        return -1;
    }

    printf("Element %d is poped out of stack \r\n", s->stack[s->top]);
    return (s->stack[s->top--]);
}

void nextSmallerElements(int nums[], int nse[], int n)
{
    STACK *st = initStack();

    for (int i = 2 * n - 1; i >= 0; i--) {

        while (false == isStackEmpty(st) && peek(st) >= nums[i % n]) {
            (void) pop(st);
        }

        if (i < n) {
            if (false == isStackEmpty(st)) {
                nse[i] = peek(st);
            }
        }

        push(st, nums[i % n]);
    }

    return;
}

int main()
{
    int v[] = {8, 3, 4, 2, 3, 1};
    int len = sizeof(v)/sizeof(v[0]);
    int res[len];
    memset(res, -1, sizeof(res));

    nextSmallerElements(v, res, len);

    printf("The next greater elements are : \r\n");
    for (int i = 0; i < len; i++) {
        printf("next greater of %d is %d \r\n", v[i], res[i]);
    }

    return 0;
}