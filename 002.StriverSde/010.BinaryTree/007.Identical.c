#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX_QUEUE 1000
#define MAX(a_, b_) ((a_) > (b_) ? (a_) : (b_))

typedef struct node {
    int data;
    int horizontalLevel;
    int verticalLevel;
    int index;
    struct node * left;
    struct node * right;
}NODE;

typedef struct Queue_ {
    NODE * queue[MAX_QUEUE];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->horizontalLevel = 0;
    node->verticalLevel = 0;
    node->index = 0;

    return (node);
}

QUEUE* initQueue()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->start = -1;
    queue->end = -1;
    queue->currSize = 0;
    queue->maxSize = MAX_QUEUE;
    return queue;
}

int queueSize(QUEUE *q)
{
    return (q->currSize);
}

bool isQueueFull(QUEUE *q)
{
    return (q->currSize >= (q->maxSize - 1)) ? true : false;
}

bool isQueueEmpty(QUEUE *q)
{
    return (q->start < 0) ? true : false;
}

NODE * queuFront(QUEUE *q)
{
    NODE * node = NULL;
    if (isQueueEmpty(q)) {
        printf("Queue is Empty \r\nExiting...\r\n");
        return NULL;
    }

    return q->queue[q->start];
}

void enQueue(QUEUE *q, NODE * val)
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
    //printf("Element %d is enqued \r\n", val->data);
    q->currSize++;
}

NODE * deQueue(QUEUE *q)
{
    NODE * val;
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

bool isIdentical(NODE * p, NODE * q)
{
    if((NULL == p) || (NULL == q)) {
        return (p == q);
    }

    if ((p->data == q->data) &&
        (isIdentical(p->left, q->left)) &&
        (isIdentical(p->right, q->right))) {
            return true;
    }

    return false;
}

int main()
{
    NODE * root1 = newNode(1);
    NODE * root2 = newNode(1);

    root1->left = newNode(2);
    root2->left = newNode(2);

    root1->right = newNode(3);
    root2->right = newNode(3);

    root1->right->left = newNode(4);
    root2->right->left = newNode(4);

    root1->right->left->left = newNode(5);
    root2->right->left->left = newNode(5);

    root1->right->left->left->left = newNode(9);
    root2->right->left->left->left = newNode(10);

    root1->right->right = newNode(6);
    root2->right->right = newNode(6);

    root1->right->right->right = newNode(7);
    root2->right->right->right = newNode(7);

    root1->right->right->right->right = newNode(8);
    root2->right->right->right->right = newNode(8);

    printf("The Binary Trees are : %s \r\n", isIdentical(root1, root2) ? "Identical" : "Not Identical");

    root2->right->left->left->left->data = 9;
    printf("The Binary Trees are : %s \r\n", isIdentical(root1, root2) ? "Identical" : "Not Identical");

    return 0;
}