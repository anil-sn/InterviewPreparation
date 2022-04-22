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
    Implement Stack using single Queue
        Problem Statement: Implement a Stack using a single Queue.
            Note: Stack is a data structure that follows the Last In First Out (LIFO) rule.
            Note: Queue is a data structure that follows the First In First Out (FIFO) rule.

        Explanation:
            push(): Insert the element in the stack.
            pop(): Remove and return the topmost element of the stack.
            top(): Return the topmost element of the stack
            size(): Return the size of the stack

        Solution:
            Intuition: As we know stack follows last in first out,
            which means we get the most recently inserted element whenever we remove an element
            from the stack. But queue follows first in first out, it means we get that element
            which we inserted in the starting at each deletion,
            it means if we want to use the queue like a stack we have to arrange elements
            in the queue such that we get the most recent element at each deletion.

        Approach:
            Take a single queue.
                push(x): Push the element in the queue.
                    Use a for loop of size()-1, remove element from queue and again push back to the queue, hence the most recent element becomes the most former element and vice versa.
                pop(): remove the element from the queue.
                top(): show the element at the top of the queue.
                size(): size of the current queue.
                Repeat step3 at every insertion of the element.
*/

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

int queueFront(QUEUE *q)
{
    int val;
    if (isQueueEmpty(q)) {
        printf("Queue is Empty \r\nExiting...\r\n");
    }

    val = q->queue[q->start];
    return val;
}

int queueSize(QUEUE *q)
{
    return q->currSize;
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
    //printf("Element %d is enqued \r\n", val);
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
    QUEUE *q;
} STACK;


STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->q = initQueue();

    return stack;
}

int Pop(STACK *s) {
    int n = deQueue(s->q);
    return n;
}

int Top(STACK *s) {
    return queueFront(s->q);
}
int Size(STACK *s) {
    return queueSize(s->q);
}

void Push(STACK *s, int x)
{
    int size = queueSize(s->q);
    enQueue(s->q, x);
    for (int i = 0; i < size; i++) {
        enQueue(s->q, deQueue(s->q));
    }
}

int main() {
    STACK *s = initStack();

    Push(s, 3);
    Push(s, 4);
    Push(s, 2);
    Push(s, 1);

    printf("Top of the stack: %d \r\n", Top(s));
    printf("Size of the stack before removing element:  %d \r\n", Size(s));
    printf("The deleted element is:  %d \r\n", Pop(s));
    printf("Top of the stack after removing element:  %d \r\n", Top(s));
    printf("Size of the stack after removing element:  %d \r\n", Size(s));

  return 0;
}