#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
    2.4 Partition: Write code to partition a linked list around a value x, such that all nodes less than x come
        before all nodes greater than or equal to x. If x is contained within the list the values of x only need
        to be after the elements less than x (see below).

        The partition element x can appear anywhere in the "right partition";
        it does not need to appear between the left and right partitions.

        EXAMPLE
        Input: 3 -> 5 -> 8 -> 5 -> 10 -> 2 -> 1 [partition= 5]
        Output: 3 -> 1 -> 2 -> 10 -> 5 -> 5 -> 8

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

NODE *
partition(NODE *node, int32_t x) {

    NODE *beforeStart = NULL;
    NODE *beforeEnd = NULL;
    NODE *afterStart = NULL;
    NODE *afterEnd = NULL;

    /* Partition list */
    while (node != NULL) {
        NODE *next = node->next;
        node->next = NULL;
        if (node->data < x) {
            /* Insert node into end of before list */
            if (beforeStart == NULL) {
                beforeStart = node;
                beforeEnd = beforeStart;
            } else {
                beforeEnd->next = node;
                beforeEnd = node;
            }
        } else {
            /* Insert node into end of after list */
            if (afterStart == NULL) {
                afterStart = node;
                afterEnd = afterStart;
            } else {
                afterEnd->next = node;
                afterEnd = node;
            }
        }
        node = next;
    }

    if (beforeStart == NULL) {
        return afterStart;
    }

    /* Merge before list and after list */
    beforeEnd->next = afterStart;
    return beforeStart;
}

int main () {

    /* The constructed linked list is: 15->14->13->12->10->9->8 */

    NODE *nthToLast = NULL;
    NODE *start = newNode(15);
    start->next = newNode(14);
    start->next->next = newNode(13);
    start->next->next->next = newNode(12);
    start->next->next->next->next = newNode(10);
    start->next->next->next->next->next = newNode(9);
    start->next->next->next->next->next->next = newNode(8);

    printf("Linked list before ");
    printList(start);

    NODE *sorted = partition(start, 12);

    printf("Linked list After");
    printList(sorted);

    return 0;
}
