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


typedef struct TreeNode_ {
    int val;
    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;

typedef struct stack_ {
    TreeNode *stack[MAX];
    int top;
} STACK;

typedef struct queue_ {
    TreeNode *queue[MAX];
    int front;
    int rear;
} QUEUE;

void
initializeStack (STACK *s)
{
    s->top = -1;
}

bool
isStackEmpty (STACK *s)
{
    if (s->top == -1) {
        return true;
    } else {
        return false;
    }
}

bool
isStackFull (STACK *s)
{
    if (s->top == MAX - 1) {
        return true;
    } else {
        return false;
    }
}

void
push (STACK *s, TreeNode *x)
{
    if (isStackFull (s)) {
        printf ("Stack Overflow\n");
        return;
    }

    s->top = s->top + 1;
    s->stack[s->top] = x;
}

TreeNode *
pop (STACK *s)
{
    int x;

    if (isStackEmpty (s)) {
        printf ("Stack Underflow\n");
        return -1;
    }

    x = s->stack[s->top];
    s->top = s->top - 1;
    return x;
}


void
initializeQueue (QUEUE *q)
{
    q->rear = -1;
    q->front = -1;
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
    if (q->rear == MAX - 1) {
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
    return x;
}


/*  Lets start with straightforward - recursive DFS - it's easy to write and pretty much concise.
    The above solution is correct, but it is also bound to the application stack,
    which means that it's no so much scalable - (you can find the problem size that will overflow
    the stack and crash your application), so more robust solution would be to use stack data structure.
*/
TreeNode *
invertTree (TreeNode *root)
{
    if (root == NULL) {
        return NULL;
    }

    TreeNode *left = root->left;
    TreeNode *right = root->right;
    root->left = invertTree (right);
    root->right = invertTree (left);
    return root;
}


// DFS
TreeNode *
invertTree (TreeNode *root)
{
    if (root == NULL) {
        return NULL;
    }

    STACK s;
    initializeStack (&s);
    push (&s, root);

    while (false == isStackEmpty (&s)) {
        TreeNode *node = pop (&s);
        // Swap Left and Right
        TreeNode *left = node->left;
        node->left = node->right;
        node->right = left;

        if (node->left != NULL) {
            push (&s, node->left);
        }

        if (node->right != NULL) {
            push (&s, node->right);
        }
    }

    return root;
}

// Finally we can easly convert the above solution to BFS - or so called level order traversal.
TreeNode *
invertTree (TreeNode *root)
{
    if (root == NULL) {
        return NULL;
    }

    QUEUE q;
    initializeStack (&q);
    enqueue (&q, root);

    while (false == isQueueEmpty (&q)) {
        TreeNode *node = dequeue (&q);
        // Swap
        TreeNode *left = node->left;
        node->left = node->right;
        node->right = left;

        if (node->left != NULL) {
            enqueue (&q, node->left);
        }

        if (node->right != NULL) {
            enqueue (&q, node->right);
        }
    }

    return root;
}