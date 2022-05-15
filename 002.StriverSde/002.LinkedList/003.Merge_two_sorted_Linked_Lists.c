#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

//Definition for singly-linked list.
typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode * addNodeToLinkedList(ListNode *head, int value)
{
    // This one time we will not do argument validation we expect NULL head

    ListNode *temp = malloc(sizeof(ListNode));
    if (head == NULL) {
        temp->next = NULL;
        temp->val = value;
        return temp;
    }

    temp->val = value;
    temp->next = head->next;
    head->next = temp;
    return head;
}

void printLinkedList(ListNode *head)
{
    if (NULL == head) {
        return;
    }

    ListNode *temp = head;

    printf("The List values are : \r\n");
    while (NULL != temp) {
        printf("\t [%p] %d \r\n", temp, temp->val);
        temp = temp->next;
    }
    printf("\r\n");
}

ListNode * merge(ListNode * head1, ListNode * head2)
{
    if ((head1 == NULL) && (head2 == NULL)) {
        return NULL;
    }

    if ((head1 == NULL) || (head2 == NULL)) {
        if (NULL == head1) {
            return head2;
        } else {
            return head1;
        }
    }

    ListNode *l1Head = head1;
    ListNode *l2Head = head2;
    ListNode *newHead = NULL;
    ListNode *pos = NULL;

    /* l1Head point to a node with lowest value */
    if (l1Head->val > l2Head->val) {
        pos = l1Head;
        l1Head = l2Head;
        l2Head = pos;
    }

    newHead = l1Head;
    while(l1Head != NULL && l2Head != NULL) {

        ListNode* temp = NULL;

        while(l1Head != NULL && l1Head->val <= l2Head->val) {
            temp = l1Head;//storing last sorted node
            l1Head = l1Head->next;
        }

        // link previous sorted node with next larger node in list2
        temp->next = l2Head;

        pos = l1Head;
        l1Head = l2Head;
        l2Head = pos;
    }

    return newHead;
}


int main () {

    ListNode *head1 = addNodeToLinkedList(NULL, 10);
    addNodeToLinkedList(head1, 19);
    addNodeToLinkedList(head1, 18);
    addNodeToLinkedList(head1, 17);
    addNodeToLinkedList(head1, 16);
    addNodeToLinkedList(head1, 15);
    addNodeToLinkedList(head1, 14);
    addNodeToLinkedList(head1, 13);
    addNodeToLinkedList(head1, 12);
    addNodeToLinkedList(head1, 11);

    printf("\t List 1 \r\n");
    printLinkedList(head1);

    ListNode *head2 = addNodeToLinkedList(NULL, 20);
    addNodeToLinkedList(head2, 29);
    addNodeToLinkedList(head2, 28);
    addNodeToLinkedList(head2, 27);
    addNodeToLinkedList(head2, 26);
    addNodeToLinkedList(head2, 25);
    addNodeToLinkedList(head2, 24);
    addNodeToLinkedList(head2, 23);
    addNodeToLinkedList(head2, 22);
    addNodeToLinkedList(head2, 21);

    printf("\t List 2 \r\n");
    printLinkedList(head2);

    ListNode *newHead = merge(head1, head2);

    printf("\t After Merging New List \r\n");
    printLinkedList(newHead);

    return;
}