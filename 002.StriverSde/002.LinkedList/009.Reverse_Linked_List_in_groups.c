#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

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

int lengthOfLinkedList(ListNode *head)
{
    int count = 0;
    ListNode *tmp = head;

    while (NULL != tmp) {
        tmp = tmp->next;
        count++;
    }

    printf("Total Number of nodes in the list : %d \r\n", count);
    return count;
}

/* lets use three pointer techniq */
ListNode * reverseLinkedListGroups(ListNode *head, int groupCount)
{
    if ((head == NULL) || (head->next == NULL)) {
        return head;
    }

    int length = lengthOfLinkedList(head);

    ListNode * dummyHead = getNewNode(0);
    dummyHead->next = head;

    ListNode * prev = dummyHead;
    ListNode * current;
    ListNode * next;

    while(length >= groupCount) {

        current = prev->next;
        next = current->next;

        for(int i = 1; i < groupCount; i++) {
            current->next = next->next;
            next->next = prev->next;
            prev->next = next;
            next = current->next;
        }

        prev = current;
        length -= groupCount;
    }

    return dummyHead->next;
}

int main ()
{
    ListNode *head1 = num2LinkedList(123456789);
    printLinkedList(head1);

    ListNode *newHead1 = NULL;
    newHead1 = reverseLinkedListGroups(head1, 3);
    printLinkedList(newHead1);

    return;
}