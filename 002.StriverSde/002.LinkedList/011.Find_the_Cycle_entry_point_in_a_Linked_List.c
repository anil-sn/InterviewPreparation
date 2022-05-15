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

bool doseCycleExist(ListNode *head)
{
    bool result = false;
    ListNode *slow = head;
    ListNode *fast = head;
    ListNode *entry = head;

    if (NULL == head) {
        return result;
    }

    while(fast->next != NULL && fast->next->next != NULL) {

        fast = fast->next->next;
        slow = slow->next;

        if(fast == slow) {
            while (entry != slow) {
                slow = slow->next;
                entry = entry->next;
            }
            printf("\t Cycle Entry point [%p] %d \r\n", entry, entry->val);
            return true;
        }
    }

    return false;
}

int main ()
{
    ListNode *head1 = num2LinkedList(123456789);

    ListNode *temp;
    temp = head1;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = head1->next->next;

    int count = 18;
    temp = head1;
    printf("Cyclic List  : \r\n");
    while (count) {
        printf("\t [%p] %d \r\n", temp, temp->val);
        temp = temp->next;
        count--;
    }

    bool result = doseCycleExist(head1);
    if (result) {
        printf("There exists a cycle \r\n");
    } else {
        printf("Cycle does not exists\r\n");
    }

    return;
}