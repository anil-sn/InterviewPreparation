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
    Next Greater Element Using Stack
    Problem Statement: Given a circular integer array A, return the next greater element
    for every element in A. The next greater element for an element x is the first element
    greater than x that we come across while traversing the array in a clockwise manner.
    If it doesn’t exist, return -1 for this element.

    Examples:
        Example 1:
            Input: N = 11, A[] = {3,10,4,2,1,2,6,1,7,2,9}
            Output: 10,-1,6,6,2,6,7,7,9,9,10
            Explanation: For the first element in A ,i.e, 3, the greater element which comes next
            to it while traversing and is closest to it is 10. Hence,10 is present on index 0 in
            the resultant array. Now for the second element,i.e, 10, there is no greater number and
            hence -1 is it’s next greater element (NGE). Similarly, we got the NGEs for all other
            elements present in A.

        Example 2:
            Input:  N = 6, A[] = {5,7,1,7,6,0}
            Output: 7,-1,7,-1,7,5
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

void nextGreaterElements(int nums[], int nge[], int n)
{
    memset(nge, -1, sizeof(nge));

    STACK *st = initStack();

    for (int i = 2 * n - 1; i >= 0; i--) {

        while (false == isStackEmpty(st) && peek(st) <= nums[i % n]) {
            (void) pop(st);
        }

        if (i < n) {
            if (false == isStackEmpty(st)) {
                nge[i] = peek(st);
            }
        }

        push(st, nums[i % n]);
    }

    return;
}

int main()
{
    int v[] = {5,7,1,2,6,0};
    int len = sizeof(v)/sizeof(v[0]);
    int res[len];

    nextGreaterElements(v, res, len);

    printf("The next greater elements are : \r\n");
    for (int i = 0; i < len; i++) {
        printf("next greater of %d is %d \r\n", v[i], res[i]);
    }

    return 0;
}