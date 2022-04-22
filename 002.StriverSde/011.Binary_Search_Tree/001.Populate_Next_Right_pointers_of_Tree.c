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
    You are given a perfect binary tree where all leaves are on the same level, and every parent has two children. The binary tree has the following definition:

    struct Node {
    int val;
    Node *left;
    Node *right;
    Node *next;
    }
    Populate each next pointer to point to its next right node. If there is no next right node, the next pointer should be set to NULL.

    Initially, all next pointers are set to NULL.
*/


#define MAX_QUEUE 1000

typedef struct node {
    int data;
    int horizontalLevel;
    int verticalLevel;
    int index;

    bool visited;
    struct node * next;
    struct node * left;
    struct node * right;

}NODE;

NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
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

void connectLevelNodes(NODE* root)
{
    NODE* node = NULL;

    if(root == NULL) {
        return;
    }

    // Initialize a queue data structure which contains just the root of the tree
    QUEUE *q = initQueue();
    enQueue(q, root);

    root->horizontalLevel = 1;
    root->verticalLevel = 0;

    // Outer while loop which iterates over  each level
    while (false == isQueueEmpty(q)) {

        // Note the size of the queue
        int size = queueSize(q);

        // Iterate over all the nodes on the current level
        for (int i = 0; i < size; i++) {

            // Pop a node from the front of the queue
            node = deQueue(q);

            // This check is important. We don't want to establish any wrong connections.
            // The queue will contain nodes from 2 levels at most at any point in time.
            // This check ensures we only don't establish next pointers beyond the end of a level
            if (i < size - 1) {
                node->next = queuFront(q);
            }

            if(node->left != NULL) {
                node->left->horizontalLevel = node->horizontalLevel + 1;
                node->left->verticalLevel = (node->verticalLevel - 1);
                enQueue(q, node->left);
            }

            if(node->right != NULL) {
                node->right->horizontalLevel = node->horizontalLevel + 1;
                node->right->verticalLevel = (node->verticalLevel + 1);
                enQueue(q, node->right);
            }
        }
    }

    return;
}


void connectLevelNodesIntuition(NODE* root)
{
    if(root == NULL) {
        return;
    }

    // Start with the root node. There are no next pointers
    // that need to be set up on the first level
    NODE *leftmost = root;

    // Once we reach the final level, we are done
    while (leftmost->left != NULL) {

        // Iterate the "linked list" starting from the head node
        // Using the next pointers, establish the corresponding links for the next level
        NODE *head = leftmost;

        while (head != NULL) {

            // CONNECTION 1
            head->left->next = head->right;

            // CONNECTION 2
            if (head->next != NULL) {
                head->right->next = head->next->left;
            }

            // Progress along the list (nodes on the current level)
            head = head->next;
        }

        // Move onto the next level
        leftmost = leftmost->left;
    }

    return;
}

int main()
{
    int time = 0;

    NODE * root = newNode(1);

    root->left = newNode(2);
    root->right = newNode(3);

    root->left->left = newNode(4);
    root->left->right = newNode(5);

    root->right->left = newNode(6);
    root->right->right = newNode(7);

    //connectLevelNodesIntuition(root);
    connectLevelNodes(root);

    NODE * leftMost = root;
    NODE * itor = NULL;
    while (NULL != leftMost) {

        itor = leftMost;
        while(NULL != itor) {
            printf("%d->", itor->data);
            itor = itor->next;
        }
        printf("NULL \r\n");
        leftMost = leftMost->left;
    }

    return 0;
}