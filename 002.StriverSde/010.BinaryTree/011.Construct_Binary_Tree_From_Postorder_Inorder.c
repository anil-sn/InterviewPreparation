#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX_MAP 1000
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

NODE* buildSubTree(int postorder[],
                    int postStart,
                    int postEnd,
                    int inorder[],
                    int inStart,
                    int inEnd,
                    int inMap[])
{
    if ((postStart > postEnd) || (inStart > inEnd)) {
        return NULL;
    }

    NODE* root = newNode(postorder[postEnd]);

    int rightIndex = inMap[postorder[postEnd]];

    root->left = buildSubTree(postorder,
                                postStart,
                                postStart + rightIndex - inStart - 1,
                                inorder,
                                inStart,
                                rightIndex - 1,
                                inMap);

    root->right = buildSubTree(postorder,
                                postStart + rightIndex - inStart,
                                postEnd - 1,
                                inorder,
                                rightIndex + 1,
                                inEnd,
                                inMap);

    return root;
}


NODE* buildTree(int postorder[], int postLen, int inorder[], int inLen)
{
    int inMap[MAX_MAP] = {-1};

    for(int i = 0; i < inLen; i++) {
        inMap[inorder[i]] = i;
    }

    NODE* root = buildSubTree (postorder,
                                0,
                                postLen - 1,
                                inorder,
                                0,
                                inLen - 1,
                                inMap);
    return root;
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

int main()
{
    int postorder[] = {40,50,20,60,30,10};
    int inorder[] = {40,20,50,10,60,30};

    int postLen = sizeof(postorder)/sizeof(postorder[0]);
    int inLen = sizeof(inorder)/sizeof(inorder[0]);

    NODE * root = buildTree(postorder, postLen, inorder, inLen);

    printf("Pre Order : \r\n");
    preOrderTrav(root);
    printf("\r\n");

    printf("In Order : \r\n");
    inOrderTrav(root);
    printf("\r\n");

    printf("Post Order : \r\n");
    postOrderTrav(root);
    printf("\r\n");

    return 0;
}