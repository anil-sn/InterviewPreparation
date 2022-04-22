#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX_STACK 1000
#define MAX(a_, b_) ((a_) > (b_) ? (a_) : (b_))

typedef struct node {
    int data;
    int horizontalLevel;
    int verticalLevel;
    int index;
    struct node * left;
    struct node * right;
}NODE;

typedef struct stack_ {
    NODE * stack[MAX_STACK];
    int top;
    int capacity;
} STACK;

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

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAX_STACK;
    return stack;
}

bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

NODE * stackPeek(STACK *s)
{
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return NULL;
    }

    return (s->stack[s->top]);
}

void stackPush(STACK *s, NODE * value)
{
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    //printf("Element %d is pushed to stack \r\n", value->data);
    s->stack[++s->top] = value;
    return;
}

NODE * stackPop(STACK *s)
{
    if (isStackEmpty(s)) {
        return NULL;
    }

    return (s->stack[s->top--]);
}

void flatten(NODE* root)
{
    NODE* cur = NULL;

    if(root == NULL) {
        return;
    }

    STACK *s = initStack();

    stackPush(s, root);
    while(false == isStackEmpty(s)) {

        cur = stackPop(s);
        if(cur->right != NULL) {
            stackPush(s, cur->right);
        }

        if(cur->left != NULL) {
            stackPush(s, cur->left);
        }

        if(false == isStackEmpty(s)) {
            cur->right = stackPeek(s);
        }

        cur->left = NULL;
    }
}

int main()
{
    NODE * root = newNode(1);
    root->left = newNode(2);

    root->right = newNode(3);
    root->right->left = newNode(4);
    root->right->left->left = newNode(5);
    root->right->left->left->left = newNode(9);

    root->right->right = newNode(6);
    root->right->right->right = newNode(7);
    root->right->right->right->right = newNode(8);

    printf("Flatten the given Binary Tree : \r\n");
    flatten(root);
    NODE * itor = root;
    while (NULL != itor) {
        printf("%4d", itor->data);
        itor = itor->right;
    }
    printf("\r\n");

    return 0;
}