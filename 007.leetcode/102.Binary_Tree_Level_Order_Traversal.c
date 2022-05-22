#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAXIMUM 1000

typedef struct TreeNode_ {
    int val;
    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;

typedef struct queue_ {
    TreeNode *queue[MAXIMUM];
    int front;
    int rear;
    int size;
} QUEUE;


void
initializeQueue (QUEUE *q)
{
    q->rear = -1;
    q->front = -1;
    q->size = 0;
}

bool
isQueueEmpty (QUEUE *q)
{
    if (q->front == -1 || q->front == q->rear + 1) {
        return true;
    } else {
        return false;
    }
}

bool
isQueueFull (QUEUE *q)
{
    if (q->rear == MAXIMUM - 1) {
        return true;
    } else {
        return false;
    }
}

void
enqueue (QUEUE *q, TreeNode *x)
{
    if (isQueueFull (q)) {
        printf ("Queue Overflow\n");
        return;
    }

    if (q->front == -1) {
        q->front = 0;
    }

    q->rear = q->rear + 1;
    q->queue[q->rear] = x;
    q->size++;
}

TreeNode *
dequeue (QUEUE *q)
{
    int x;

    if (isQueueEmpty (q)) {
        printf ("Queue Underflow\n");
        return -1;
    }

    x = q->queue[q->front];
    q->front = q->front + 1;
    q->size--;
    return x;
}

int
queueSize (QUEUE *q)
{
    return q->size;
}

int result[MAXIMUM];
int pos = 0;

void
levelOrder (TreeNode *root)
{
    if (root == NULL) {
        return;
    }

    QUEUE q;
    initializeQueue (&q);
    enqueue (&q, root);

    while (false == isQueueEmpty (&q)) {
        int cnt = queueSize (&q);

        for (int i = 0; i < cnt; i++) {
            TreeNode *node = dequeue (&q);
            result[pos++] = node->val;

            if (node->left != NULL) {
                enqueue (&q, node->left);
            }

            if (node->right != NULL) {
                enqueue (&q, node->right);
            }
        }
    }
}