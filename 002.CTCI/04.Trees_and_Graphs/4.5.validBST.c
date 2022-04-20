#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "limits.h"
#include "math.h"
#include "sys/types.h"
#include "assert.h"

/*
    4.5 Validate BST: Implement a function to check if a binary tree is a binary search tree.

    SOLUTION
        We can implement this solution in two different ways. The first leverages the in-order traversal, and the
        second builds off the property that left <= c urrent < right.

    Solution #1: In-Order Traversal
        Our first thought might be to do an in-order traversal, copy the elements to an array, and then check to see
        if the array is sorted. This solution takes up a bit of extra memory, but it works-mostly.
        The only problem is that it can't handle duplicate values in the tree properly. For example, the algorithm
        cannot distinguish between the two trees below (one of which is invalid) since they have the same in-order
        traversal.
        However, if we assume that the tree cannot have duplicate values, then this approach works.

        Note that it is necessary to keep track of the logical "end" of the array, since it would be allocated to hold all
        the elements.

        When we examine this solution, we find that the array is not actually necessary. We never use it other than
        to compare an element to the previous element. So why not just track the last element we saw and compare
        it as we go?

    Solution #2: The Min / Max Solution
        In the second solution, we leverage the definition of the binary search tree.
        What does it mean for a tree to be a binary search tree? We know that it must, of course, satisfy the condition
        left. data <= c urrent. data < right. data for each node, but this isn't quite sufficient.

        More precisely, the condition is that all left nodes must be less than or equal to the current node, which
        must be less than all the right nodes.
        Using this thought, we can approach the problem by passing down the min and max values. As we iterate
        through the tree, we verify against progressively narrower ranges.

        We start with a range of (min = NULL, max = NULL), which the root obviously meets. (NULL indicates
        that there is no min or max.) We then branch left, checking that these nodes are within the range ( min =
        NULL, max = 20). Then, we branch right, checking that the nodes are within the range ( min = 20, max = NULL).
        We proceed through the tree with this approach. When we branch left, the max gets updated. When we
        branch right, the min gets updated. If anything fails these checks, we stop and return false.

        The time complexity for this solution is O(N), where N is the number of nodes in the tree. We can prove that
        this is the best we can do, since any algorithm must touch all N nodes.
        Due to the use of recursion, the space complexity is O ( log N) on a balanced tree. There are up to O ( log
        N) recursive calls on the stack since we may recurse up to the depth of the tree.
*/

/* A binary tree node has data, pointer to left child and a pointer to right child */
typedef struct node {
    int data;
    struct node* left;
    struct node* right;
} NODE;

/* Returns true if the given tree is a BST and its values are >= min and <= max. */
bool
isBSTUtil(NODE* node, int min, int max) {

    /* an empty tree is BST */
    if (node==NULL) {
        return true;
    }

    /* false if this node violates the min/max constraint */
    if (node->data < min || node->data > max) {
        return false;
    }

    /* otherwise check the subtrees recursively, tightening the min or max constraint */
    return (isBSTUtil(node->left, min, node->data-1) &&  // Allow only distinct values
            isBSTUtil(node->right, node->data+1, max));  // Allow only distinct values
}

/* Returns true if given tree is BST. */
bool
isBSTPtr(NODE* root, NODE* l, NODE* r) {

    /* Base condition */
    if (root == NULL) {
        return true;
    }

    /* if left node exist then check it has correct data or not i.e. left node's data should be less than root's data */
    if (l != NULL && root->data <= l->data) {
        return false;
    }

    /* if right node exist then check it has correct data or not i.e. right node's data should be greater than root's data */
    if (r != NULL && root->data >= r->data) {
        return false;
    }

    // check recursively for every node.
    return (isBSTPtr(root->left, l, root) &&
           isBSTPtr(root->right, root, r));
}

/* Returns true if the given tree is a binary search tree (efficient version). */
int
isBST(NODE* node) {
    return(isBSTUtil(node, INT_MIN, INT_MAX));
}

/* Helper function that allocates a new node with the given data and NULL left and right pointers. */
NODE*
newNode(int data) {
    NODE* node = (NODE*) malloc(sizeof(NODE));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return(node);
}

/* Driver program to test above functions*/
int main()
{
    NODE *root = newNode(4);
    root->left        = newNode(2);
    root->right       = newNode(5);
    root->left->left  = newNode(1);
    root->left->right = newNode(3);

    if(isBST(root)) {
        printf("Is BST \r\n");
    } else {
        printf("Not a BST \r\n");
    }

    if (isBSTPtr(root, NULL, NULL))
        printf("[PTR] Is BST \r\n");
    else
        printf("[PTR] Not a BST \r\n");

    return 0;
}

