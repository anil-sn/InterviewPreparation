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

// C implementation BST in iterative way

// Structure of the node
typedef struct treeNode {
    int data;
    struct treeNode* left;
    struct treeNode* right;
} NODE;


// A structure to represent a stack
typedef struct stack_s {
    int top;
    unsigned capacity;
    NODE** array;
} STACK;

// A utility function to create a new BST NODE
NODE* newNode(int item)
{
    NODE* temp = malloc(sizeof(NODE));
    temp->data = item;
    temp->left = temp->right = NULL;
    return temp;
}

// function to create a stack of given capacity. It initializes size of stack as 0
STACK* createStack(unsigned capacity)
{
    STACK* stack = (STACK*)malloc(sizeof(STACK));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (NODE **)malloc(stack->capacity * sizeof(NODE *));
    return stack;
}

// Stack is full when top is equal to the last index
int isFull(STACK* stack)
{
    return stack->top == stack->capacity - 1;
}

// Stack is empty when top is equal to -1
int isEmpty(STACK* stack)
{
    return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1
void push(STACK* stack, NODE *item)
{
    if (isFull(stack)) {
        return;
    }

    stack->array[++stack->top] = item;
    //printf("%d pushed to stack\n", item->data);
    return;
}

// Function to remove an item from stack.  It decreases top by 1
NODE * pop(STACK* stack)
{
    if (isEmpty(stack)) {
        return NULL;
    }

    return stack->array[stack->top--];
}

// Function to return the top from stack without removing it
NODE * peek(STACK* stack)
{
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->array[stack->top];
}

/* Iterative function for inorder tree
   traversal */
void inOrder(NODE *root)
{
    STACK* s = createStack(100);
    NODE *curr = root;

    while (curr != NULL || (isEmpty(s)) == false)
    {
        /* Reach the left most NODE of the Curr NODE */
        while (curr !=  NULL)
        {
            /* place pointer to a tree node on the stack before traversing the node's left subtree */
            push(s, curr);
            curr = curr->left;
        }

        /* Current must be NULL at this point */
        curr = pop(s);

        printf("Visit %d -> ", curr->data);

        /* we have visited the node and its left subtree.  Now, it's right subtree's turn */
        curr = curr->right;

    } /* end of while */

    printf ("\r\n");
}

// Function to check the given key exist or not
bool iterativeSearch(NODE* root, int key)
{
    // Traverse until root reaches to dead end
    while (root != NULL) {

        // pass right subtree as new tree
        if (key > root->data)
            root = root->right;

        // pass left subtree as new tree
        else if (key < root->data)
            root = root->left;
        else
            return true; // if the key is found return 1
    }
    return false;
}

// A utility function to insert a new
// NODE with given key in BST
NODE* insert(NODE* root, int key)
{
    // Create a new NODE containing
    // the new element
    NODE *newnode = newNode(key);

    // Pointer to start traversing from root and traverses downward path to search
    // where the new node to be inserted
    NODE *x = root;

    // Pointer y maintains the trailing pointer of x
    NODE *y = NULL;

    while (x != NULL) {
        y = x;
        if (key < x->data)
            x = x->left;
        else
            x = x->right;
    }

    // If the root is NULL i.e the tree is empty
    // The new node is the root node
    if (y == NULL) {
        y = newnode;
    } else if (key < y->data) {
        // If the new key is less then the leaf node key
        // Assign the new node to be its left child
        y->left = newnode;
    } else {
        // else assign the new node its right child
        y->right = newnode;
    }

    // Returns the pointer where the
    // new node is inserted
    return y;
}

// Iterative Function to delete 'key' from the BST.
NODE* deleteIterative(NODE* root, int key)
{
    NODE* curr = root;
    NODE* prev = NULL;

    // Check if the key is actually
    // present in the BST.
    // the variable prev points to
    // the parent of the key to be deleted.
    while (curr != NULL && curr->data != key) {
        prev = curr;
        if (key < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if (curr == NULL) {
        printf("Key %d not found in the provided BST.\n", key);
        return root;
    }

    // Check if the node to be deleted has atmost one child.
    if (curr->left == NULL || curr->right == NULL) {

        // newCurr will replace
        // the node to be deleted.
        NODE* newCurr;

        // if the left child does not exist.
        if (curr->left == NULL)
            newCurr = curr->right;
        else
            newCurr = curr->left;

        // check if the node to
        // be deleted is the root.
        if (prev == NULL) {
            return newCurr;
        }

        // check if the node to be deleted
        // is prev's left or right child
        // and then replace this with newCurr
        if (curr == prev->left)
            prev->left = newCurr;
        else
            prev->right = newCurr;

        // free memory of the
        // node to be deleted.
        free(curr);
    } else { // node to be deleted has two children.
        NODE* p = NULL;
        NODE* temp;

        // Compute the inorder successor
        temp = curr->right;
        while (temp->left != NULL) {
            p = temp;
            temp = temp->left;
        }

        // check if the parent of the inorder
        // successor is the curr or not(i.e. curr=
        // the node which has the same data as
        // the given data by the user to be
        // deleted). if it isn't, then make the
        // the left child of its parent equal to
        // the inorder successor'd right child.
        if (p != NULL)
            p->left = temp->right;

        // if the inorder successor was the
        // curr (i.e. curr = the node which has the
        // same data as the given data by the
        // user to be deleted), then make the
        // right child of the node to be
        // deleted equal to the right child of
        // the inorder successor.
        else
            curr->right = temp->right;

        curr->data = temp->data;
        free(temp);
    }

    return root;
}

// Driver Code
int main()
{
    int key = 0;
    /*
         10
        /  \
       7    15
      / \   / \
      5  8 11 18

    */
    NODE* root = NULL;

    key = 10;
    root = insert(root, key);
    printf("After %d is Inserted BST:\n", key);
    inOrder(root);

    key = 7;
    root = insert(root, key);
    printf("After %d is Inserted BST:\n", key);
    inOrder(root);

    key = 5;
    root = insert(root, key);
    printf("After %d is Inserted BST:\n", key);
    inOrder(root);

    key = 8;
    root = insert(root, key);
    printf("After %d is Inserted BST:\n", key);
    inOrder(root);

    key = 15;
    root = insert(root, key);
    printf("After %d is Inserted BST:\n", key);
    inOrder(root);

    key = 11;
    root = insert(root, key);
    printf("After %d is Inserted BST:\n", key);
    inOrder(root);

    key = 18;
    root = insert(root, key);
    printf("After %d is Inserted BST:\n", key);
    inOrder(root);

    printf("Inorder traversal of original BST:\n");
    inOrder(root);

    key = 7;
    printf("Search %d in BST successful : %s\n", key, iterativeSearch(root, key) ? "TRUE" : "FALSE");

    // delete node with data value 11 (leaf)
    key = 11;
    printf("delete node with data value %d (leaf) \n", key);
    root = deleteIterative(root, key);
    printf("\nDeletion of %d\n", key);
    printf("Inorder traversal post deletion:\n");
    inOrder(root);

    // delete node with data value 15 (internal node with one child)
    key = 15;
    printf("delete node with data value %d (internal node with one child) \n", key);
    root = deleteIterative(root, key);
    printf("\nDeletion of %d\n", key);
    printf("Inorder traversal post deletion:\n");
    inOrder(root);

    // delete node with data value 10  (root, two children)
    key = 10;
    printf("delete node with data value %d (root, two children) \n", key);
    root = deleteIterative(root, key);
    printf("\nDeletion of %d\n", key);
    printf("Inorder traversal post deletion:\n");
    inOrder(root);

    return 0;
}