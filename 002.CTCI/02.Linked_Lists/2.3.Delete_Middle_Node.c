#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
    2.3 Delete Middle Node: Implement an algorithm to delete a node in the middle (i.e., any node but
        the first and last node, not necessarily the exact middle) of a singly linked list, given only access to
        that node.
    EXAMPLE
        Input:the node c from the linked list a->b->c->d->e->f
        Result: nothing is returned, but the new linked list looks like a ->b->d- >e- >f

    SOLUTION
        In this problem, you are not given access to the head of the linked list. You only have access to that node.
        The solution is simply to copy the data from the next node over to the current node, and then to delete the
        next node.

    Note that this problem cannot be solved if the node to be deleted is the last node in the linked list. That's
    okay-your interviewer wants you to point that out, and to discuss how to handle this case. You could, for
    example, consider marking the node as dummy.

*/


/* A linked list node */
typedef struct node {
    int32_t data;
    struct node *next;
} NODE;

/* Utility function to create a new Node */
NODE *
newNode(int32_t data) {
   NODE *temp = malloc(sizeof(NODE));
   temp->data = data;
   temp->next = NULL;
   return temp;
}

/* Function to print nodes in a given linked list */
void
printList(NODE *node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }

    printf ("\r\n");
    return;
}

void
deleteNode(NODE *n) {

    NODE *next = NULL;

    if (n == NULL || n->next == NULL) {
        return; // Failure
    }

    next = n->next;
    n->data = next->data;
    n->next = next->next;

    return;
}


int main () {

    /* The constructed linked list is: 10->12->11->11->12->11->10 */
    NODE *nthToLast = NULL;
    NODE *start = newNode(10);
    start->next = newNode(12);
    start->next->next = newNode(11);
    start->next->next->next = newNode(11);

    start->next->next->next->next = newNode(12);
    NODE *DelNode = start->next->next->next->next;

    start->next->next->next->next->next = newNode(11);
    start->next->next->next->next->next->next = newNode(10);

    printf("Linked list before ");
    printList(start);

    deleteNode(DelNode);

    printf("Linked list After");
    printList(start);

    return 0;
}

