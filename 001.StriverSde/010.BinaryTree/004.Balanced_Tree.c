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
#define MAX(a_, b_) (a_ > b_ ? a_ : b_)

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

int dfsHeight (NODE *root)
{
    if (root == NULL) {
        return 0;
    }

    int leftHeight = dfsHeight (root->left);
    if (leftHeight == -1) {
        return -1;
    }

    int rightHeight = dfsHeight (root->right);
    if (rightHeight == -1) {
        return -1;
    }

    if (abs(leftHeight - rightHeight) > 1) {
        return -1;
    }

    return MAX(leftHeight, rightHeight) + 1;
}

bool isBalanced(NODE * root)
{
    return dfsHeight (root) != -1;
}

int main()
{
    NODE * root = newNode(3);
    root->left = newNode(9);

    root->right = newNode(20);
    root->right->left = newNode(15);
    root->right->right = newNode(7);

     ;
    printf("The Binary Tree is : %s \r\n", isBalanced(root) ? "Balanced" : "Not Balanced");

    return 0;
}