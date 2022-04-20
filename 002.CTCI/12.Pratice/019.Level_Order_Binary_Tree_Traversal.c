#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Dont practice until you get it right
    practice until you can't get it worng
*/

// Recursive C program for level
// order traversal of Binary Tree
#include <stdio.h>
#include <stdlib.h>

#define MAX_Q_SIZE 500

/* A binary tree node has data,
   pointer to left child
   and a pointer to right child */
struct node {
    int data;
    struct node *left;
    struct node *right;
};

/* Function prototypes */
struct node** createQueue(int*, int*);
void enQueue(struct node**, int*, struct node*);
struct node* deQueue(struct node** queue, int* front);

void printCurrentLevel_Recursive(struct node* root, int level);
int height_Recursive(struct node* node);

struct node* newNode(int data);

/* Function to print level order traversal a tree*/
void printLevelOrder_Recursive(struct node* root)
{
    int h = height_Recursive(root);
    int i;
    for (i = 1; i <= h; i++)
        printCurrentLevel_Recursive(root, i);
}

/* Print nodes at a current level */
void printCurrentLevel_Recursive(struct node* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        printf("%d ", root->data);
    else if (level > 1) {
        printCurrentLevel_Recursive(root->left, level - 1);
        printCurrentLevel_Recursive(root->right, level - 1);
    }
}

/* Compute the "height_Recursive" of a tree -- the number of
    nodes along the longest path from the root node
    down to the farthest leaf node.*/
int height_Recursive(struct node* node)
{
    if (node == NULL)
        return 0;
    else {
        /* compute the height_Recursive of each subtree */
        int lheight_Recursive = height_Recursive(node->left);
        int rheight_Recursive = height_Recursive(node->right);

        /* use the larger one */
        if (lheight_Recursive > rheight_Recursive)
            return (lheight_Recursive + 1);
        else
            return (rheight_Recursive + 1);
    }
}

/* Helper function that allocates a new node with the
   given data and NULL left and right pointers. */
struct node* newNode(int data)
{
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return (node);
}
/* Given a binary tree, print its nodes in level order
   using array for implementing queue */
void printLevelOrder(struct node* root)
{
    int rear, front;
    struct node** queue = createQueue(&front, &rear);
    struct node* temp_node = root;

    while (temp_node) {
        printf("%d ", temp_node->data);

        /*Enqueue left child */
        if (temp_node->left)
            enQueue(queue, &rear, temp_node->left);

        /*Enqueue right child */
        if (temp_node->right)
            enQueue(queue, &rear, temp_node->right);

        /*Dequeue node and make it temp_node*/
        temp_node = deQueue(queue, &front);
    }
}

/*UTILITY FUNCTIONS*/
struct node** createQueue(int* front, int* rear)
{
    struct node** queue = (struct node**)malloc(sizeof(struct node*) * MAX_Q_SIZE);

    *front = *rear = 0;
    return queue;
}

void enQueue(struct node** queue, int* rear, struct node* new_node)
{
    queue[*rear] = new_node;
    (*rear)++;
}

struct node* deQueue(struct node** queue, int* front)
{
    (*front)++;
    return queue[*front - 1];
}

/* Driver program to test above functions*/
int main()
{
    struct node* root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);

    printf("\r\nRecursive Level Order traversal of binary tree is \n");
    printLevelOrder_Recursive(root);

    printf("\r\nIterative Level Order traversal of binary tree is \n");
    printLevelOrder(root);
    printf("\r\n");

    return 0;
}