#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

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

void insertBST(NODE *root, int data)
{
    NODE * new = newNode(data);

    while (root != NULL && root->data != data) {

        if (data <= root->data) {
            if (NULL != root->left) {
                root = root->left;
            } else {
                root->left = new;
                break;
            }
        } else {
            if (NULL != root->right) {
                root = root->right;
            } else {
                root->right = new;
                break;
            }
        }
    }

    return;
}

int Ceil(NODE *root, int key)
{
    int ceil = -1;

    while (NULL != root) {

        if (root->data == key) {
            ceil = root->data;
            return ceil;
        }

        if (key > root->data) {
            root = root->right;
        }
        else {
            ceil = root->data;
            root = root->left;
        }
    }

    return ceil;
}

int Floor(NODE *root, int key)
{
    int floor = -1;

    while (NULL != root) {

        if (root->data == key) {
            floor = root->data;
            return floor;
        }

        if (key > root->data) {
            floor = root->data;
            root = root->right;
        }
        else {
            root = root->left;
        }
    }

    return floor;
}

int main()
{
    int time = 0;

    NODE * root = newNode(10);

    insertBST(root, 5);
    insertBST(root, 7);
    insertBST(root, 2);
    insertBST(root, 8);
    insertBST(root, 1);
    insertBST(root, 9);

    insertBST(root, 15);
    insertBST(root, 17);
    insertBST(root, 12);
    insertBST(root, 18);
    insertBST(root, 11);
    insertBST(root, 19);

    printf("For key %d The Ceil value is %d, Floor value is %d \r\n",6, Ceil(root, 6), Floor(root,6));

    return 0;
}