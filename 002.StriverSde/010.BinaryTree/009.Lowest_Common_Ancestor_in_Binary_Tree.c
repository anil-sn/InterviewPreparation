#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

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

NODE * lowestCommonAncestor(NODE * root, NODE * p, NODE * q)
{
    //base case
    if (root == NULL) {
        return NULL;
    }

    if (root == p || root == q) {
        return root;
    }

    NODE* left = lowestCommonAncestor(root->left, p, q);
    NODE* right = lowestCommonAncestor(root->right, p, q);

    //result
    if(left == NULL) {
        return right;
    }
    else if(right == NULL) {
        return left;
    }
    else {
        return root;
    }
}

int main()
{
    NODE * root = newNode(1);

    root->left = newNode(2);
    root->left->left = newNode(4);
    root->left->right = newNode(5);
    root->left->right->left = newNode(6);
    root->left->right->right = newNode(7);

    root->right = newNode(3);
    root->right->left = newNode(8);
    root->right->right = newNode(9);

    NODE *result = lowestCommonAncestor(root, root->left->left, root->left->right->right);
    printf("Lowest Common Ancestor (4, 7) in Binary Tree is : %d \r\n", result->data);

    result = lowestCommonAncestor(root, root->left->right, root->right->right);
    printf("Lowest Common Ancestor (5, 9) in Binary Tree is : %d \r\n", result->data);

    return 0;
}