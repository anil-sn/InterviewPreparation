#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
    2.7 Intersection: Given two (singly) linked lists, determine if the two lists intersect. Return the
        intersecting node. Note that the intersection is defined based on reference, not value. That is, if the
        kth node of the first linked list is the exact same node (by reference) as the jth node of the second
        linked list, then they are intersecting.
    SOLUTION
        Let's draw a picture of intersecting linked lists to get a better feel for what is going on.

    We should be careful here to not inadvertently draw a special case by making the linked lists the same
    length.

    Let's first ask how we would determine if two linked lists intersect. Determining if there's an intersection.

    How would we detect if two linked lists intersect? One approach would be to use a hash table and just
    throw all the linked lists nodes into there. We would need to be careful to reference the linked lists by their
    memory location, not by their value.

    There's an easier way though. Observe that two intersecting linked lists will always have the same last node.
    Therefore, we can just traverse to the end of each linked list and compare the last nodes.

    How do we find where the intersection is, though?

    Finding the intersecting node.
    One thought is that we could traverse backwards through each linked list. When the linked lists"split'; that's
    the intersection. Of course, you can't really traverse backwards through a singly linked list.
    If the linked lists were the same length, you could just traverse through them at the same time. When they
    collide, that's your intersection.

    When they're not the same length, we'd like to just"chop off"-or ignore-those excess (gray) nodes.
    How can we do this? Well, if we know the lengths of the two linked lists, then the difference between those
    two linked lists will tell us how much to chop off.

    We can get the lengths at the same time as we get the tails of the linked lists (which we used in the first step
    to determine if there's an intersection).

    Putting it all together.
    We now have a multistep process.
    1. Run through each linked list to get the lengths and the tails.
    2. Compare the tails.If they are different (by reference,not by value),return immediately.There is no intersection.
    3. Set two pointers to the start of each linked list.
    4. On the longer linked list, advance its pointer by the difference in lengths.
    5. Now, traverse on each linked list until the pointers are the same.

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
        printf("[%p]%d -> ", node, node->data);
        node = node->next;
    }

    printf ("\r\n");
    return;
}

/* Helper function to create a new node with the given data and pushes it onto the list's front */
void
push(NODE** head, int32_t data)
{
    /* create a new linked list node from the heap */
    NODE *newNode = malloc(sizeof(NODE));

    newNode->data = data;
    newNode->next = *head;
    *head = newNode;

    return;
}

typedef struct result {
    NODE *tail;
    int32_t size;
} RESULT;

NODE *
getKthNode(NODE * head, int k) {
    NODE * current = head;
    while (k > 0 && current != NULL) {
        current = current->next;
        k--;
    }

    return current;
}

RESULT *
getTailAndSize(NODE * list) {
    if (list == NULL) {
        return NULL;
    }

    int size = 1;
    NODE * current = NULL;
    RESULT *result = malloc(sizeof(RESULT));

    current = list;
    while (current->next != NULL) {
        size++;
        current = current->next;
    }

    result->tail = current;
    result->size = size;
    return result;
}

NODE *
findintersection(NODE * list1, NODE * list2) {
    if (list1 == NULL || list2 == NULL) {
        return NULL;
    }

    /* Get tail and sizes. */
    RESULT *result1 = getTailAndSize(list1);
    RESULT *result2 = getTailAndSize(list2);

    /* If different tail nodes, then there's no intersection. */
    if (result1->tail != result2->tail) {
        return NULL;
    }

    /* Set pointers to the start of each linked list. */
    NODE *shorter = result1->size < result2->size? list1 : list2;
    NODE *longer = result1->size < result2->size ? list2 : list1;

    /* Advance the pointer for the longer linked list by difference in lengths. */
    longer = getKthNode(longer, abs(result1->size - result2->size));

    /* Move both pointers until you have a collision. */
    while (shorter != longer) {
        shorter = shorter->next;
        longer = longer->next;
    }

    /* Return either one. */
    return longer;
}

int32_t
main(void) {

    NODE *l1 = newNode(1);
    l1->next = newNode(3);
    l1->next->next = newNode(5);
    l1->next->next->next = newNode(3);
    l1->next->next->next->next = newNode(1);

    NODE *l2 = newNode(5);
    l2->next = newNode(6);
    l2->next->next = l1->next->next->next;

    printf("First Linked List: \r\n");
    printList(l1);

    printf("Second Linked List: \r\n");
    printList(l2);

    NODE *inter = findintersection(l1, l2);
    if (NULL == inter) {
        printf("The linked list has no intersection\r\n");
    }
    else {
        printf("The linked list has intersection %p [%d] \r\n", inter, inter->data);
    }

    return 0;
}



