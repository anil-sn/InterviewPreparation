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

NODE* buildSubTree(int preorder[],
                    int preStart,
                    int preEnd,
                    int inorder[],
                    int inStart,
                    int inEnd,
                    int inMap[])
{
    if ((preStart > preEnd) || (inStart > inEnd)) {
        return NULL;
    }

    NODE* root = newNode(preorder[preStart]);
    int inRoot = inMap[root->data];
    int numsLeft = inRoot - inStart;

    root->left = buildSubTree(preorder,
                                preStart + 1,
                                preStart + numsLeft,
                                inorder,
                                inStart,
                                inRoot - 1,
                                inMap);

    root->right = buildSubTree(preorder,
                                preStart + numsLeft + 1,
                                preEnd,
                                inorder,
                                inRoot + 1,
                                inEnd,
                                inMap);

    return root;
}


NODE* buildTree(int preorder[], int preLen, int inorder[], int inLen)
{
    int inMap[MAX_MAP] = {-1};

    for(int i = 0; i < preLen; i++) {
        inMap[inorder[i]] = i;
    }

    NODE* root = buildSubTree (preorder,
                                0,
                                preLen - 1,
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
    int preorder[] = {10,20,40,50,30,60};
    int inorder[] = {40,20,50,10,60,30};

    int preLen = sizeof(preorder)/sizeof(preorder[0]);
    int inLen = sizeof(inorder)/sizeof(inorder[0]);

    NODE * root = buildTree(preorder, preLen, inorder, inLen);

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