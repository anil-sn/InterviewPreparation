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
    Clone Linked List with Random Pointer

    Problem Statement
    Given a linked list having two pointers in each node. The first one points to the next node of the list, however,
    the other pointer is random and can point to any node of the list or null.

    The task is to create a deep copy of the given linked list and return its head.
    We will validate whether the linked list is a copy of the original linked list or not.

    A deep copy of a Linked List means we do not copy the references of the nodes of the original Linked List rather for each node
    in the original Linked List, a new node is created.

*/

#define MAX_DISPLAY 100

typedef struct ListNode {
    int val;
    struct ListNode *next;
    struct ListNode *random;
} ListNode;

ListNode * getNewNode(int value)
{
    ListNode *temp = malloc(sizeof(ListNode));
    temp->next = NULL;
    temp->bottom = NULL;
    temp->val = value;
    return temp;
}

ListNode * copyRandomList(ListNode * head) {

    if (head == NULL) {
      return NULL;
    }

    // Creating a new weaved list of original and copied nodes.
    ListNode * itor = head;
    while (itor != null) {

        // Cloned node
        ListNode * newNode = getNewNode(itor->val);

        // Inserting the cloned node just next to the original node.
        // If A->B->C is the original linked list,
        // Linked list after weaving cloned nodes would be A->A'->B->B'->C->C'
        newNode->next = itor->next;
        itor->next = newNode;
        itor = newNode->next;
    }

    itor = head;

    // Now link the random pointers of the new nodes created.
    // Iterate the newly created list and use the original nodes' random pointers,
    // to assign references to random pointers for cloned nodes.
    while (itor != NULL) {
      itor->next->random = (itor->random != NULL) ? itor->random->next : NULL;
      itor = itor->next->next;
    }

    // Unweave the linked list to get back the original linked list and the cloned list.
    // i.e. A->A'->B->B'->C->C' would be broken to A->B->C and A'->B'->C'
    ListNode * oldList = head; // A->B->C
    ListNode * newList = head->next; // A'->B'->C'
    ListNode * head_old = head->next;

    while (oldList != null) {
        oldList->next = oldList->next->next;
        newList->next = (newList->next != null) ? newList->next->next : null;
        oldList = oldList->next;
        newList = newList->next;
    }

    return head_old;
}