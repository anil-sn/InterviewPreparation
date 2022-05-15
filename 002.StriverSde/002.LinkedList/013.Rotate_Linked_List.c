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
    Rotate a Linked List

    Problem Statement: Given the head of a linked list, rotate the list to the right by k places.

    Examples:

        Example 1:
            Input: head = [1,2,3,4,5] k = 2
            Output: head = [4,5,1,2,3]

        Example 2:
            Input: head = [1,2,3] k = 4
            Output: head = [3,1,2]
*/

#define MAX_DISPLAY 100

typedef struct ListNode {
    int val;
    struct ListNode *next;
    struct ListNode *bottom;
} ListNode;

ListNode * getNewNode(int value)
{
    ListNode *temp = malloc(sizeof(ListNode));
    temp->next = NULL;
    temp->bottom = NULL;
    temp->val = value;
    return temp;
}

ListNode * uint64num2LinkedList(u_int64_t n)
{
    ListNode *head = NULL;
    ListNode *curr = NULL;
    ListNode *temp = NULL;
    int remainder = 0;

    while (0 != n) {
        remainder = n % 100;
        temp = getNewNode(remainder);
        if (NULL == head) {
            head = temp;
            curr = temp;
        }

        curr->next = temp;
        curr = curr->next;
        n = n / 100;
    }

    return head;
}

void printLinkedList(ListNode *head)
{
    if (NULL == head) {
        return;
    }

    int count = MAX_DISPLAY;
    ListNode *temp = head;
    ListNode *itor = NULL;

    printf("\r\n The List values are : ");
    while (NULL != temp) {
        count--;
        if (0 == count) {
            break;
        }
        printf("\r\n \t [%p] %d ", temp, temp->val);
        if ((NULL != temp) && (NULL != temp->bottom)) {
            itor = temp->bottom;
            while (NULL != itor) {
                count--;
                if (0 == count) {
                    break;
                }
                printf("\t [%p] %d ", itor, itor->val);
                itor = itor->next;
            }
        }
        if (0 == count) {
            break;
        }
        temp = temp->next;
    }

    if (0 == count) {
        printf("Infinet Loop \r\n");
        return;
    }

    printf("\r\n");
    return;
}

ListNode * rotateLinkedList(ListNode *main, int pos)
{
    if ((NULL == main) || (NULL == main->next) || (0 == pos)) {
        return main;
    }

    ListNode *itor = main;
    int len = 1;
    while (NULL != itor->next) {
        len++;
        itor = itor->next;
    }

    //when k is more than length of list
    pos = pos % len;

    //to get end of the list
    int end = len - pos;

    itor->next = main;
    while(end)  {
        end --;
        itor = itor->next;
    }

    //breaking last node link and pointing to NULL
    ListNode *head = itor->next;
    itor->next = NULL;

    return head;
}


int main ()
{
    ListNode *main = uint64num2LinkedList(102030405060708090);

    printLinkedList(main);

    printf("\r\n\r\n The Final Result after %d rotation: \r\n", 4);
    ListNode *newHead = rotateLinkedList(main, 4);
    printLinkedList(newHead);

    return;
}