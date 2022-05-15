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

ListNode * removeNode(ListNode *head, int k)
{
    if ((NULL == head) || (0 == k)) {
        return head;
    }

    if (1 == k) {
        return head->next;
    }

    ListNode *prev = NULL;
    ListNode *cur = head;

    while ((1 != k) && (NULL != cur)) {
        prev = cur;
        cur = cur->next;
        k--;
    }

    if ((prev != NULL) && (prev->next != NULL)) {
        prev->next = prev->next->next;
    }

    return head;
}

int main () {

    ListNode *head = addNodeToLinkedList(NULL, 10);
    addNodeToLinkedList(head, 19);
    addNodeToLinkedList(head, 18);
    addNodeToLinkedList(head, 17);
    addNodeToLinkedList(head, 16);
    addNodeToLinkedList(head, 15);
    addNodeToLinkedList(head, 14);
    addNodeToLinkedList(head, 13);
    addNodeToLinkedList(head, 12);
    addNodeToLinkedList(head, 11);

    printf("\t List \r\n");
    printLinkedList(head);

    ListNode *newHead = removeNode(head, 5);
    if (NULL != newHead) {
        printf("\t Updated List \r\n");
        printLinkedList(newHead);
    }

    return;
}