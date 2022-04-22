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

void flatten(NODE* root)
{
    NODE* cur = root;
    NODE* pre = NULL;
    while (NULL != cur) {

        if(NULL != cur->left) {

            pre = cur->left;
            while(pre->right) {

                pre = pre->right;
            }

            pre->right = cur->right;
            cur->right = cur->left;
            cur->left = NULL;
        }

        cur = cur->right;
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