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
    4.9. BST Sequences: A binary search tree was created by traversing through an array from left to right and inserting
                        each element. Given a binary search tree with distinct elements, print all possible arrays that
                        could have led to this tree.

    Solution.

        Let's start with an example.

            4
           / \
          2   5
         / \   \
        1   3   6

        To construct this tree we must insert the node 4 first. This node is always going to be the first element in each
        of the possible solution. Lets remove this node from the tree.

          2    5
         / \    \
        1   3    6
        To continue construcing the tree from the example we now have a choice of eather inserting 2 or 5.
        Notice that both are the roots of the respective subtrees. Lets start with node 2 and remove it from the tree.

        1   3   5
                 \
                  6
        We are left with 3 subtrees. We now have a choice of removing either 1, 3 or 5.

        You can cleary see that there is an inductive step which we can use to implement our solution:

        start with a root of the tree (the only valid choice)

        for each of the current valid choices:
        - remove one of the roots (valid choices), add its children to the set of choices
        - recursively find all possible solutions for the new set of choices
        - append the root to the head of each of those solutions

        the recursion ends when there are no remaining nodes (choices) left

*/

struct BST {
    int key;
    struct BST *left;
    struct BST *right;
};

struct sequence {
    int key;
    struct sequence *next;
};

typedef struct sequence *sequencePtr;
typedef struct BST *nodePtr;

typedef struct {
  sequencePtr *sequences;
  size_t used;
  size_t size;
} array;

nodePtr root = NULL;


int insertSequence(sequencePtr *head, int key) {

    sequencePtr newPtr = malloc(sizeof(struct sequence));
    newPtr->key = key;
    newPtr->next = NULL;

    sequencePtr prevPtr, currentPtr;
    prevPtr = NULL;
    currentPtr = *head;

    while (currentPtr != NULL) {
        prevPtr = currentPtr;
        currentPtr = currentPtr->next;
    }

    if (prevPtr == NULL) {
        newPtr->next = *head;
        *head = newPtr;
    } else {
        prevPtr->next = newPtr;
        newPtr->next = currentPtr;
    }
    return 1;
}


nodePtr insertBST(nodePtr node, sequencePtr sequence) {
    nodePtr newPtr = malloc(sizeof(struct BST));
    newPtr->key = sequence->key;
    newPtr->left = NULL;
    newPtr->right = NULL;

    if (node == NULL)
        return newPtr;

    if (sequence->key < node->key)
    node->left = insertBST(node->left, sequence);
    else if (sequence->key > node->key)
    node->right = insertBST(node->right, sequence);

    return node;
}

void printSequence(sequencePtr head) {
    printf("sequence: ");
    sequencePtr currentPtr = head;
    do {
        printf("%d ", currentPtr->key);
        currentPtr = currentPtr->next;
    } while (currentPtr != NULL);
    printf("\n");
}

void printInOrder(nodePtr node) {
    if (node != NULL) {
        printInOrder(node->left);
        printf("%d ", node->key);
        printInOrder(node->right);
    }
}

void initArray(array *a, size_t initialSize) {
  a->sequences = (sequencePtr *)malloc(initialSize * sizeof(sequencePtr));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(array *a, sequencePtr seq) {
  if (a->used == a->size) {
    a->size *= 2;
    a->sequences = (sequencePtr *)realloc(a->sequences, a->size * sizeof(sequencePtr));
  }
  a->sequences[a->used++] = seq;
}

sequencePtr clone(sequencePtr src) {
    if (src == NULL)
    return NULL;
    sequencePtr result = malloc(sizeof(struct sequence));
    result->key = src->key;
    result->next = clone(src->next);
    return result;
}

void deleteSequence(sequencePtr *head, int key) {

    sequencePtr temp = *head;
    sequencePtr prev;

    if (temp != NULL && temp->key == key) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->key != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    return;

    prev->next = temp->next;
    free(temp);
}

nodePtr searchBST(nodePtr node, int key) {
    if (node == NULL || node->key == key)
        return node;

    if (node->key < key)
    return searchBST(node->right, key);

    return searchBST(node->left, key);
}

void buildSeqs(array *allSeq, sequencePtr main, sequencePtr poss, nodePtr node) {
    insertSequence(&main, node->key);
    if (node->left != NULL) {
        insertSequence(&poss, node->left->key);
    }
    if (node->right != NULL) {
        insertSequence(&poss, node->right->key);
    }
    if (poss == NULL) {
        insertArray(allSeq, main);
    }

    sequencePtr n = poss;
    while (n != NULL) {
        sequencePtr mainTemp = clone(main);
        sequencePtr possTemp = clone(poss);
        deleteSequence(&possTemp, n->key);
        buildSeqs(allSeq, mainTemp, possTemp, searchBST(root, n->key));
        n = n->next;
    }
}

void allSequences(nodePtr rootPtr) {
    array allSeq;
    initArray(&allSeq, 6);
    sequencePtr main = NULL;
    sequencePtr poss = NULL;

    buildSeqs(&allSeq, main, poss, rootPtr);

    printf("Number of sequences: %ld\n\n", allSeq.used);
    int i;
    for (i = 0; (i < 50) ? i < allSeq.used : i < 50; i++) { // This line can be changed to (i = 0; i < allSeq.used; i++) to print all the sequences
        printf("%d. ", i + 1);
        printSequence(allSeq.sequences[i]);
    }

    if (allSeq.used > 50)
    printf("\n...");
}


int main(void) {

    sequencePtr head = NULL;
    printf("Enter a sequence: ");
    int key;
    char space;
    do {
        scanf("%d%c", &key, &space);
        insertSequence(&head, key);
    } while (space == ' ');

    root = insertBST(root,head);

    sequencePtr currentPtr = head->next;
    while (currentPtr != NULL) {
        insertBST(root,currentPtr);
        currentPtr = currentPtr->next;
    }

    printf("In-order traversal of BST: ");
    printInOrder(root);
    printf("\n");

    allSequences(root);
    exit(0);
}

