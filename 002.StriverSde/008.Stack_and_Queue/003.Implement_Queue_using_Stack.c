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
    Implement Queue using Stack
    Problem Statement: Given a Stack having some elements stored in it. Can you implement a
    Queue using the given Stack?

    Queue: A Queue is a linear data structure that works on the basis of FIFO(First in First out). This means the element added at first will be removed first from the Queue.
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

    //printf("Element %d is pushed to stack \r\n", value);
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

// Pop: Removes an item from the stack. The items are popped in the reversed order in which they are pushed.
// If the stack is empty, then it is said to be an Underflow condition.
int pop(STACK *s) {
    if (isStackEmpty(s)) {
        return -1;
    }

    return (s->stack[s->top--]);
}

int size(STACK *s) {
    return ((s->top)+1);
}

typedef struct Queue_ {
    STACK *input;
    STACK *output;
} QUEUE;

QUEUE* initQueue()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->input = initStack();
    queue->output = initStack();

    return queue;
}

void enQueue(QUEUE* q, int data)
{
    while (false == isStackEmpty(q->input)) {
        push(q->output, pop(q->input));
    }

    // Insert the desired element in the stack input
    printf("The element pushed is %d \r\n", data);
    push(q->input, data);

    // Pop out elements from the stack output and push them into the stack input
    while (false == isStackEmpty(q->output)) {
        push(q->input, pop(q->output));
    }
}

// Pop the element from the Queue
int deQueue(QUEUE *q) {
    if (isStackEmpty(q->input)) {
        printf("Stack is empty \r\n");
    }

    return pop(q->input);
}

// Return the Topmost element from the Queue
int Top(QUEUE *q) {

    if (isStackEmpty(q->input)) {
        printf("Stack is empty \r\n");
    }

    return peek(q->input);
}

// Return the size of the Queue
int Size(QUEUE *q) {
    return size(q->input);
}

int main() {

    QUEUE *q = initQueue();

    enQueue(q, 3);
    enQueue(q, 4);
    printf("The element poped is %d \r\n", deQueue(q));
    enQueue(q, 5);
    printf("The top of the queue is %d \r\n", Top(q));
    printf("The size of the queue is %d \r\n", Size(q));

    return 0;
}