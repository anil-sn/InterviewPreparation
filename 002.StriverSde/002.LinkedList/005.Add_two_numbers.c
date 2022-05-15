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
    Add two numbers represented as Linked Lists

    Problem Statement: Given the heads of two non-empty linked lists representing two
    non-negative integers. The digits are stored in reverse order, and each of their
    nodes contains a single digit.
    Add the two numbers and return the sum as a linked list.

    Examples:

    Input Format:
        (Pointer/Access to the head of the two linked lists)

        num1  = 342, num2 = 564

        l1 = [2,4,3]
        l2 = [5,6,4]

        Result: sum = 807; L = [7,0,8]

        Explanation: Since the digits are stored in reverse order,
        reverse the numbers first to get the or original number and then add them
        as → 342 + 465 = 807

        2 --> 4 --> 3
        5 --> 6 --> 4
    -----------------
        7 --> 0 --> 8
    -----------------
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

ListNode * addTwoLists(ListNode *a, ListNode *b)
{
    ListNode * newHead = NULL;
    ListNode * temp = NULL;
    ListNode * lastNode = NULL;
    ListNode * l1 = a;
    ListNode * l2 = b;

    int carry = 0;
    int sum = 0;
    int remainder = 0;

    while ((NULL != l1) || (NULL != l2) || (carry == 1)) {

        if ((NULL != l1) && (NULL != l2)) {
            sum = l1->val + l2->val + carry;
            l1 = l1->next;
            l2 = l2->next;
        } else if (NULL != l1) {
            sum = l1->val + carry;
            l1 = l1->next;
        } else if (NULL != l2) {
            sum = l2->val + carry;
            l2 = l2->next;
        } else {
            sum = carry;
        }

        remainder = sum % 10;
        carry = sum / 10;
        temp = getNewNode(remainder);
        if (NULL == newHead) {
            newHead = temp;
            lastNode = newHead;
        } else {
            lastNode->next = temp;
            lastNode = lastNode->next;
        }
    }

    return newHead;
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

int main ()
{
    int a = 342;
    int b = 564;

    ListNode *listA = num2LinkedList(a);
    printf("List A: %d \r\n", a);
    printLinkedList(listA);

    ListNode *listB = num2LinkedList(b);
    printf("List B: %d \r\n", b);
    printLinkedList(listB);

    ListNode *listAplusB = addTwoLists(listA, listB);
    printf("List A + B: %d \r\n", a + b);
    printLinkedList(listAplusB);

    return;
}