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
    Reverse a Linked List
    Problem Statement: Given the head of a singly linked list, write a program to reverse the linked list,
    and return the head pointer to the reversed list.

    Examples:
        Input Format:
        head = [3,6,8,10]
        This means the given linked list is 3->6->8->10 with head pointer at node 3.

    Result:
        Output = [10,6,8,3]
        This means, after reversal, the list should be 10->6->8->3 with the head pointer at node 10.
    Explanation:
        This is how a linked list looks for a given input.
        the head is a reference that points to the initial or starting node of the list.
        To reverse it, we need to invert linking between nodes.
        That is, node D should point to node C, node C to node B and node B to node A.
        Then head points to node D and node A has NULL.

    Input Format:
        head = []
        This means the given linked list is empty.

    Result:
        Output = []

    Explanation:
        The linked list is empty. That is, no nodes are present in the list.
        Thus, even reversing will give the list as empty.
*/

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

ListNode * middleNode(ListNode *head)
{
    if ((NULL == head) || (NULL == head->next)) {
        return head;
    }

    ListNode *Hare = head;
    ListNode *Tortoise = head;

    while ((Hare != NULL) && (Hare->next != NULL)) {
        Hare = Hare->next->next;
        Tortoise = Tortoise->next;
    }

    return Tortoise;
}

int main () {

    ListNode *head = addNodeToLinkedList(NULL, 0);
    addNodeToLinkedList(head, 9);
    addNodeToLinkedList(head, 8);
    addNodeToLinkedList(head, 7);
    addNodeToLinkedList(head, 6);
    addNodeToLinkedList(head, 5);
    addNodeToLinkedList(head, 4);
    addNodeToLinkedList(head, 3);
    addNodeToLinkedList(head, 2);
    addNodeToLinkedList(head, 1);

    printLinkedList(head);

    ListNode *middle = middleNode(head);
    printf("\t The Middle of the linked list is [%p] %d \r\n", middle, middle->val);

    return;
}