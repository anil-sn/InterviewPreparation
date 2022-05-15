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

typedef struct node {
    int data;
    struct node * left;
    struct node * right;
}NODE;

NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

typedef struct stack_ {
    NODE * stack[MAX];
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


/** whether we have a next smallest number */
bool BST_ItorHasNext(STACK *s)
{
    return (false == isStackEmpty(s));
}

void BST_ItorPushAll(STACK *s, NODE *node)
{
    while (NULL != node){
        stackPush(s, node);
        node = node->left;
    }
}

/** the next smallest number */
int BST_Itornext(STACK *s)
{
    NODE *tmpNode = stackPop(s);
    if (tmpNode) {
        BST_ItorPushAll(s, tmpNode->right);
        return tmpNode->data;
    }

    return (-1);
}

void BST_ItorInit(STACK *s, NODE *root) {
    BST_ItorPushAll(s, root);
}

int main ()
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

    STACK *s = initStack();
    BST_ItorInit(s, root);

    printf("Itor Next : %d \r\n", BST_Itornext(s));

    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");
    printf("Itor Next : %d \r\n", BST_Itornext(s));
    printf("Has Next : %s \r\n", BST_ItorHasNext(s) ? "TRUE" : "FALSE");

    return;
}