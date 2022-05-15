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

// {8,5,1,7,10,12}
NODE* build(int preOrder[], int len, int *i, int bound)
{
    if ((*i == len) || (preOrder[*i] > bound)) {
        return NULL;
    }

    NODE* root = newNode(preOrder[(*i)++]);
    root->left = build(preOrder,  len, i, root->data);
    root->right = build(preOrder, len, i, bound);
    return root;
}

NODE* bstFromPreorder(int preOrder[], int len)
{
    int i = 0;
    return build(preOrder, len, &i, INT_MAX);
}

void inOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    inOrderTrav(curr->left);
    printf("%5d ", curr->data);
    inOrderTrav(curr->right);
}

void preOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    printf("%5d ", curr->data);
    preOrderTrav(curr->left);
    preOrderTrav(curr->right);
}

void postOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    postOrderTrav(curr->left);
    postOrderTrav(curr->right);
    printf("%5d ", curr->data);
}

bool isValid(NODE *root, int minVal, int maxVal)
{
    if (root == NULL) {
        return true;
    }

    if (root->data >= maxVal || root->data <= minVal) {
        return false;
    }

    return isValid(root->left, minVal, root->data) && isValid(root->right, root->data, maxVal);
}

bool isValidBST(NODE *root)
{
    return isValid(root, INT_MIN, INT_MAX);
}

NODE* searchBST (NODE* root, int val)
{
    while(root != NULL && root->data != val) {

        root = (val < root->data) ? (root->left) : (root->right);
    }

    return root;
}

NODE * kthSmallest(NODE *root, int k)
{
    STACK *s = initStack();
    NODE *node = root;

    int cnt = 0;
    while(true) {

        if(node != NULL){
            stackPush(s, node);
            node = node->left;
        } else {
            if(true == isStackEmpty(s)){
                break;
            }

            node = stackPop(s);

            // inorder.add(node.val);
            cnt++;
            if(cnt == k) {
                return node;
            }

            node = node->right;
        }
    }

    return NULL;
}

int main ()
{
    int preOrder[] = {8,5,1,7,10,12};
    int len = sizeof(preOrder)/sizeof(preOrder[0]);

    NODE* root = bstFromPreorder(preOrder, len);

    printf("Pre Order : \r\n");
    preOrderTrav(root);
    printf("\r\n");

    printf("In Order : \r\n");
    inOrderTrav(root);
    printf("\r\n");

    printf("Post Order : \r\n");
    postOrderTrav(root);
    printf("\r\n\r\n");

    printf("Is the constructed BST is valid : %s \r\n", isValidBST(root) ? "TRUE" : "FALSE");

    NODE* kth = kthSmallest(root, 3);
    printf("The 3rd smallest element is : %d \r\n", kth ? kth->data : -1);

    return;
}