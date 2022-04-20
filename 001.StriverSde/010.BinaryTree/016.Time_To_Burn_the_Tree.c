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

typedef struct node {
    int data;
    int horizontalLevel;
    int verticalLevel;
    int index;

    bool visited;
    struct node * parent;
    struct node * left;
    struct node * right;

}NODE;

NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->visited = false;

    node->horizontalLevel = 0;
    node->verticalLevel = 0;
    node->index = 0;

    return (node);
}

typedef struct Queue_ {
    NODE * queue[MAX_QUEUE];
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

int findMaxDistance(NODE *root, NODE *target)
{
    QUEUE *q = initQueue();

    enQueue(q, target);
    target->visited = true;
    int maxi = 0;
    NODE *node = NULL;

    while(false == isQueueEmpty(q)) {

        int sz = queueSize(q);
        int flag = false;

        for(int i = 0; i < sz; i++) {

            node = deQueue(q);

            if((NULL != node->left) && (false == node->left->visited)) {
                flag = true;
                node->left->visited = true;
                enQueue(q, node->left);
            }

            if((NULL != node->right) && (false == node->right->visited)) {
                flag = true;
                node->right->visited = true;
                enQueue(q, node->right);
            }

            if((NULL != node->parent) && (false == node->parent->visited)) {
                flag = true;
                node->parent->visited = true;
                enQueue(q, node->parent);
            }
        }

        if(true == flag) {
            maxi++;
        }
    }

    return maxi;
}

NODE * bfsToMapParents(NODE *root, int start)
{
    NODE *node = NULL;
    NODE *result = NULL;
    QUEUE *q = initQueue();
    enQueue(q, root);

    while(false == isQueueEmpty(q)) {

        node = deQueue(q);
        if(node->data == start) {
            result = node;
        }

        if(node->left) {
            node->left->parent = node;
            enQueue(q, node->left);
        }
        if(node->right) {
            node->right->parent = node;
            enQueue(q, node->right);
        }
    }

    return result;
}

int timeToBurnTree(NODE *root, int start)
{
    NODE *target = bfsToMapParents(root, start);
    int maxi = findMaxDistance(root, target);
    return maxi;
}

int main()
{
    int time = 0;

    NODE * root = newNode(1);
    root->left = newNode(2);

    root->right = newNode(3);
    root->right->left = newNode(4);
    root->right->left->left = newNode(5);
    root->right->left->left->left = newNode(9);

    root->right->right = newNode(6);
    root->right->right->right = newNode(7);
    root->right->right->right->right = newNode(8);

    printf("\r\nTime to burn given Binary Tree is : %d\r\n", timeToBurnTree(root, 8));

    return 0;
}