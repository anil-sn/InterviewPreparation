#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
    2.1 Remove Dups: Write code to remove duplicates from an unsorted linked list.
        FOLLOW UP
        How would you solve this problem if a temporary buffer is not allowed?

    SOLUTION
        In order to remove duplicates from a linked list, we need to be able to track duplicates.
        A simple hash table will work well here. In the below solution, we simply iterate through the linked list,
        adding each element to a hash table. When we discover a duplicate element,
        we remove the element and continue iterating. We can do this all in one pass since we are using a linked list.

        Follow Up: No Buffer Allowed
        if we don't have a buffer, we can iterate with two pointers: current which iterates through the linked list,
        and runner which checks all subsequent nodes for duplicates.
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

/* Function to remove duplicates from a unsorted linked list */
void removeDuplicates(NODE *start)
{
    NODE *ptr1, *ptr2, *dup;
    ptr1 = start;

    /* Pick elements one by one */
    while (ptr1 != NULL && ptr1->next != NULL)
    {
        ptr2 = ptr1;

        /* Compare the picked element with rest of the elements */
        while (ptr2->next != NULL)
        {
            /* If duplicate then delete it */
            if (ptr1->data == ptr2->next->data)
            {
                /* sequence of steps is important here */
                dup = ptr2->next;
                ptr2->next = ptr2->next->next;
                free(dup);
            }
            else /* This is tricky */
                ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }

    return;
}

int main () {

    /* The constructed linked list is: 10->12->11->11->12->11->10 */
    NODE *start = newNode(10);
    start->next = newNode(12);
    start->next->next = newNode(11);
    start->next->next->next = newNode(11);
    start->next->next->next->next = newNode(12);
    start->next->next->next->next->next = newNode(11);
    start->next->next->next->next->next->next = newNode(10);

    printf("Linked list before removing duplicates ");
    printList(start);

    removeDuplicates(start);

    printf("Linked list after removing duplicates ");
    printList(start);

    return 0;
}
