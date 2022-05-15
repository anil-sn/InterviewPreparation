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
    Find intersection of Two Linked Lists
    Problem Statement: Given the heads of two singly linked-lists headA and headB,
                       return the node at which the two lists intersect. If the two linked lists have no
                       intersection at all, return null.

    Examples:
        Example 1:
            Input: List 1 = [1,3,1,2,4], List 2 = [3,2,4]
            Output: 2
            Explanation: Here, both lists intersecting nodes start from node 2.

        Example 2:
            Input: List1 = [1,2,7], List 2 = [2,8,1]
            Output: Null
            Explanation: Here, both lists do not intersect and thus no intersection node is present.
*/

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode * getNewNode(int value)
{
    ListNode *temp = malloc(sizeof(ListNode));
    temp->next = NULL;
    temp->val = value;
    return temp;
}

ListNode * num2LinkedList(int n)
{
    ListNode *head = NULL;
    ListNode *curr = NULL;
    ListNode *temp = NULL;
    int remainder = 0;

    while (0 != n) {
        remainder = n % 10;
        temp = getNewNode(remainder);
        if (NULL == head) {
            head = temp;
            curr = temp;
        }

        curr->next = temp;
        curr = curr->next;
        n = n / 10;
    }

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

ListNode * findInterSection(ListNode *head1, ListNode *head2) {
    ListNode *l1 = head1;
    ListNode *l2 = head2;

    while(l1 != l2) {
        l1 = (l1 == NULL) ? (head2) : (l1->next);
        l2 = (l2 == NULL) ? (head1) : (l2->next);
    }

    return l1;
}

int main ()
{
    ListNode *head1 = num2LinkedList(3213457);
    ListNode *head2 = num2LinkedList(98234);
    ListNode *head3 = num2LinkedList(45676);

    ListNode *temp;
    temp = head1;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = head3;

    temp = head2;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = head3;

    printf("List1 : \r\n");
    printLinkedList(head1);
    printf("List2 : \r\n");
    printLinkedList(head2);

    temp = findInterSection(head1, head2);
    if (temp) {
        printf("Intersection Node is %p : %d\r\n", temp, temp->val);
    } else {
        printf("Intersection Node not found : \r\n");
    }

    return;
}