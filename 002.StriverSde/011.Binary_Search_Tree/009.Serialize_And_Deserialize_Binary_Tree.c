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
    Given a binary tree root, return the maximum sum of all keys of any sub-tree which is also a Binary Search Tree (BST).

    Assume a BST is defined as follows:

    The left subtree of a node contains only nodes with keys less than the node's key.
    The right subtree of a node contains only nodes with keys greater than the node's key.
    Both the left and right subtrees must also be binary search trees.
*/

#define QUEUE_MAX 1000
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)

typedef struct node {
    int data;
    struct node * left;
    struct node * right;
}NODE;

typedef struct Queue_ {
    NODE * queue[QUEUE_MAX];
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
    return (node);
}

QUEUE* initQueue()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->start = -1;
    queue->end = -1;
    queue->currSize = 0;
    queue->maxSize = QUEUE_MAX;
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

// Encodes a tree to a single string.
void serialize(NODE* root, char serial[], int *len)
{
    NODE* curNode = NULL;

    if(NULL == root) {
        *len = 0;
        serial[*len] = '\0';
        return;
    }

    QUEUE *q = initQueue();

    enQueue(q, root);

    while(false == isQueueEmpty(q)) {

        curNode = deQueue(q);

        if (NULL == curNode) {
            (*len) += sprintf(serial+(*len), "%s", "#,");
        } else {
            (*len) += sprintf(serial+(*len), "%d,", curNode->data);
        }

        if(curNode != NULL){
            enQueue(q, curNode->left);
            enQueue(q, curNode->right);
        }
    }

    return;
}

#if 1
// Decodes your encoded data to tree.
NODE* deserialize(char serial[], int len)
{
    NODE *node = NULL;
    const char s[2] = ",";
    char *token;

    if(len == 0) {
        return NULL;
    }

    token = strtok(serial, s);

    QUEUE *q = initQueue();
    NODE *root = newNode(atoi(token));
    enQueue(q, root);

    while(false == isQueueEmpty(q)) {

        node = deQueue(q);

        token = strtok(NULL, s);
        if (NULL == token) {
            continue;
        }

        if(*token == '#') {
            node->left = NULL;
        }
        else {
            NODE* leftNode = newNode(atoi(token));
            node->left = leftNode;
            enQueue(q, leftNode);
        }

        token = strtok(NULL, s);
        if(*token == '#') {
            node->right = NULL;
        }
        else {
            NODE* rightNode = newNode(atoi(token));
            node->right = rightNode;
            enQueue(q, rightNode);
        }
    }

    return root;
}
#endif

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

int main ()
{
    char serial[QUEUE_MAX] = {'\0'};
    int len = 0;

    NODE *root = newNode(20);

    root->left = newNode(15);
    root->left->left = newNode(14);
    root->left->left->right = newNode(17);

    root->left->right = newNode(18);
    root->left->right->left = newNode(16);
    root->left->right->right = newNode(19);

    root->right = newNode(40);
    root->right->left = newNode(30);
    root->right->right = newNode(60);
    root->right->right->left = newNode(50);

    serialize(root, serial, &len);
    printf("Serilized BST is : %s len : %d \r\n", serial, len);

    NODE *root2 = deserialize(serial, len);

    printf("The Binary Trees are : %s \r\n", isIdentical(root, root2) ? "Identical" : "Not Identical");

    return;
}