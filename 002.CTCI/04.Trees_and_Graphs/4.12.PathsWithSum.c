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
    Chapter 04.12 - Paths with Sum
    Problem Statement:
        You are given a binary tree in which each node contains an integer value (which might be positive or negative).
        Design an algorithm to count the number of paths that sum to a given value. The path does not need to start or
        end at the root or a leaf, but it must go downwards (traveling only from parent nodes to child nodes).
    Solution:
        The brute force solution to this problem is to (1) DFS traverse the tree starting from the root and (2)
        initiate a new DFS traversal at each node. For each of the type (2) traversals, we keep a running sum of the node values
        that have been traversed. When the running sum equals the target sum, we increment a global total. To calculate the
        total runtime, we observe that that a node at depth d has d ancestors. Because of the recursive nature of the
        algorithm, those ancestors will each cause function (2) to be called on a given node. From the definitions of
        binary trees, the depth of a tree with N nodes is equal to log(N). Thus the runtime is thus O(N*log(N)) for a balanced
        tree. For an unbalanced tree (in the worst case, every node of the tree is on the same side), the depth of a tree
        with N nodes is N. Thus, the worst case runtime of this algorithm is O(N^2). Because a running sum must be stored for
        each node (along with the data associated with the node itself) space complexity is O(N).
        A more efficient solution is to only traverse the tree once using depth first search and use a hash table to
        keep track of running sums. In the hash table, the keys are running sums and the values are the numbers of times
        the keys have been encountered. By definition, a path leads to the target sum if the difference between the current
        running sum and *any previously encountered running sum* is equal to the target sum. At each node encountered during the
        traversal, we add that node's running sum to a hash table; if that running sum is already in the hash table, then
        we increment its value. Also at each node, we compute the difference between the node's running sum and the target
        sum: if that difference is equal to *any* previously observed running sum, then we've discovered a path that yields
        the target sum. Furthermore, if there exist multiple of the same running sum, then we've discovered multiple paths that
        yield the target sum. Thus, when the difference is equal to the target sum, we increment a global path total by the
        current running sum's hash table value thus computing the number of paths that lead to the target sum. One final bit
        of complexity in this solution is that hashed sums from traversing one path will not apply to a totally different
        path in the tree. We resolve this issue in the recursive implementation of depth first search: once a given node's
        left and right recursive calls are completed, we *decrement* that node's running sum in the hash table. In this way,
        we do a constant number of hash table lookups at each node in the tree yielding a O(N) worst case runtime and O(N)
        space required due to the max hash table size and the sunning sums stored for each node.
    Time complexity: O(N)
    Space complexity: O(N)
*/

typedef struct stackNode {
    int data;
    struct stackNode *next;
}StackNode;

StackNode *StackHead = NULL;
int32_t StackCount = 0;


void push(int val) {

    //create new node
    StackNode *newNode = malloc(sizeof(StackNode));
    newNode->data = val;

    //make the new node points to the head node
    newNode->next = StackHead;

    //make the new node as head node
    //so that head will always point the last inserted data
    StackHead = newNode;
    StackCount++;
}

void pop()
{
    //temp is used to free the head node
    StackNode *temp;

    if(StackHead == NULL) {
        printf("Stack is Empty\n");
    }
    else {
        //printf("Poped element = %d\n", StackHead->data);

        //backup the head node
        temp = StackHead;

        //make the head node points to the next node.
        //logically removing the node
        StackHead = StackHead->next;

        //free the poped element's memory
        free(temp);
        StackCount--;
    }
}

//print the linked list
void printStack() {
    StackNode *temp = StackHead;

    //iterate the entire linked list and print the data
    while(temp != NULL) {
         printf("%d->", temp->data);
         temp = temp->next;
    }
    printf("NULL\n");
}

#if 0
// utility function to print contents of a vector from index i to it's end
void printVector(const vector<int>& v, int i)
{
    for (int j = i; j < v.size(); j++)
        cout << v[j] << " ";
    cout << endl;
}
#endif

// binary tree node
typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;

} NODE;

// This function prints all paths that have sum k
void
printKPathUtil(NODE* root, int k) {
    // empty node
    if (NULL == root) {
        return;
    }

    /* add current node to the path */
    //path.push_back(root->data);
    push(root->data);

    /* check if there's any k sum path in the left sub-tree. */
    printKPathUtil(root->left, k);

    /* check if there's any k sum path in the right sub-tree. */
    printKPathUtil(root->right, k);

    /*  check if there's any k sum path that terminates at this node
        Traverse the entire path as there can be negative elements too */
    int f = 0;
    StackNode *temp = StackHead;

    //iterate the entire linked list and print the data
    printf("Print Stack \r\n");
    while(temp != NULL) {
        f += temp->data;
        if (f <= k) {
            printf("%d ",temp->data);
        }
        temp = temp->next;
    }
    printf("\r\n");
#if 0
    for (int j = path.size() - 1; j >= 0; j--) {
        f += path[j];

        // If path sum is k, print the path
        if (f == k) {
            printVector(path, j);
        }
    }
#endif

    // Remove the current element from the path
    //path.pop_back();
    pop(root->data);
}

#if 0
// A wrapper over printKPathUtil()
void
printKPath(Node* root, int k)
{
    printKPathUtil(root, k);
}
#endif

NODE*
newNode(int data) {

    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->data  = data;
    node->left  = NULL;
    node->right = NULL;
    return(node);
}

/* A utility function to print preorder traversal of BST */
void preOrder(NODE* node)
{
    if (node == NULL)
        return;
    preOrder(node->left);
    printf("%d ", node->data);
    preOrder(node->right);
}

#define COUNT 10

void
print2DUtil(NODE *root, int space) {
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++) {
        printf(" ");
    }
    printf("%d\n", root->data);

    // Process left child
    print2DUtil(root->left, space);
}

int
main() {
    NODE* root = newNode(1);
    root->left = newNode(3);
    root->left->left = newNode(2);
    root->left->right = newNode(1);
    root->left->right->left = newNode(1);
    root->right = newNode(-1);
    root->right->left = newNode(4);
    root->right->left->left = newNode(1);
    root->right->left->right = newNode(2);
    root->right->right = newNode(5);
    root->right->right->right = newNode(2);

    preOrder(root);
    print2DUtil(root, 0);
    printf("\r\n");

    int k = 5;
    printKPathUtil(root, k);

    return 0;
}

