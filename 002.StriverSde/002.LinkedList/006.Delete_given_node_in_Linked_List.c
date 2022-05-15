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
    Delete given node in a Linked List : O(1) approach

    Problem Statement: Write a function to delete a node in a singly-linked list.
                       You will not be given access to the head of the list instead,
                       you will be given access to the node to be deleted directly.
                       It is guaranteed that the node to be deleted is not a tail node in the list.

    Examples:
        Example 1:
            Input:
            Linked list: [1,4,2,3]
                Node = 2
            Output:
            Linked list: [1,4,3]
            Explanation: Access is given to node 2. After deleting nodes, the linked list will be modified to [1,4,3].

        Example 2:
            Input:
            Linked list: [1,2,3,4]
                Node = 1
            Output: Linked list: [2,3,4]
            Explanation:
            Access is given to node 1. After deleting nodes, the linked list will be modified to [2,3,4].
*/

//Definition for singly-linked list.
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

ListNode * searchAndDelete(ListNode *head, int key)
{
    if (NULL == head) {
        return NULL;
    }

    if (key == head->val) {
        return head->next;
    }

    ListNode *prev = head;
    ListNode *itor = head->next;

    while (NULL != itor) {
        if (itor->val == key) {
            if (NULL != prev->next) {
                prev->next = prev->next->next;
            }
        }
        prev = itor;
        itor = itor->next;
    }

    return head;
}


int main ()
{
    int key = 7;
    ListNode *head = num2LinkedList(987654321);

    printf("In List Delete Key : %d \r\n", key);
    printLinkedList(head);

    ListNode *newHead = searchAndDelete(head, key);
    printf("Updated List : \r\n");
    printLinkedList(newHead);

    return;
}