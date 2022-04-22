#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX 1000
typedef struct Queue_ {
    int queue[MAX];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

QUEUE* initQueue()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->start = -1;
    queue->end = -1;
    queue->currSize = 0;
    queue->maxSize = MAX;
    return queue;
}

bool isQueueFull(QUEUE *q)
{
    return (q->currSize >= (q->maxSize - 1)) ? true : false;
}

bool isQueueEmpty(QUEUE *q)
{
    return (q->start < 0) ? true : false;
}

void enQueue(QUEUE *q, int val)
{
    if (isQueueFull(q)) {
        printf("Queue is full\r\nExiting...\r\n");
    }

    if (q->end == -1) {
        q->end = 0;
        q->start = 0;
    } else {
        q->end = (q->end + 1) % q->maxSize;
    }

    q->queue[q->end] = val;
    printf("Element %d is enqued \r\n", val);
    q->currSize++;
}

int deQueue(QUEUE *q)
{
    int val;
    if (isQueueEmpty(q)) {
        printf("Queue is Empty \r\nExiting...\r\n");
    }

    val = q->queue[q->start];
    if (q->currSize == 1) {
        q->end = -1;
        q->start = -1;
    } else {
        q->start = (q->start + 1) % q->maxSize;
    }

    q->currSize--;
    return val;
}

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

// Pop: Removes an item from the stack. The items are popped in the reversed order in which they are pushed.
// If the stack is empty, then it is said to be an Underflow condition.
int pop(STACK *s) {
    if (isStackEmpty(s)) {
        return -1;
    }

    return (s->stack[s->top--]);
}

int main()
{
    STACK* stack = initStack();

    printf("\r\n");
    push(stack, 10);
    push(stack, 20);
    push(stack, 30);

    printf("\r\n");
    printf("Element %d popped from stack \n", pop(stack));
    printf("Element %d popped from stack \n", pop(stack));
    printf("Element %d popped from stack \n", pop(stack));

    QUEUE* queue = initQueue();

    printf("\r\n");
    enQueue(queue, 10);
    enQueue(queue, 20);
    enQueue(queue, 30);

    printf("\r\n");
    printf("Element %d Dequeue from queue \n", deQueue(queue));
    printf("Element %d Dequeue from queue \n", deQueue(queue));
    printf("Element %d Dequeue from queue \n", deQueue(queue));
    printf("\r\n");

    return 0;
}