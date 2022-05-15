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
    Check if given Linked List is Plaindrome

    Problem Statement: Given the head of a singly linked list,
                        return true if it is a palindrome.

    Examples:
        Example 1:
            Input: head = [1,2,3,3,2,1]
            Output: true
            Explanation: If we read elements from left to right,
                        we get [1,2,3,3,2,1]. When we read elements from right to left,
                        we get [1,2,3,3,2,1]. Both ways list remains same and hence,
                        the given linked list is palindrome.

        Example 2:
            Input: head = [1,2]
            Output: false
            Explanation: When we read elements from left to right, we get [1,2].
                        Reading from right to left, we get a list as [2,1]. Both are different and hence,
                        the given linked list is not palindrome.
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

ListNode * reverse(ListNode * ptr) {
    ListNode * pre=NULL;
    ListNode * nex=NULL;
    while(ptr!=NULL) {
        nex = ptr->next;
        ptr->next = pre;
        pre = ptr;
        ptr = nex;
    }
    return pre;
}

bool isListPalindrome(ListNode * head) {
    if((head == NULL) || (head->next==NULL)) {
         return true;
    }

    ListNode * slow = head;
    ListNode * fast = head;

    while( (fast->next != NULL) && (fast->next->next != NULL)) {
        slow = slow->next;
        fast = fast->next->next;
    }

    slow->next = reverse(slow->next);
    slow = slow->next;
    ListNode * dummy = head;

    while(slow != NULL) {
        if(dummy->val != slow->val) {
            return false;
        }
        dummy = dummy->next;
        slow = slow->next;
    }

    return true;
}

int main ()
{
    ListNode *head1 = num2LinkedList(1234321);
    printLinkedList(head1);

    bool result = false;
    result = isListPalindrome(head1);
    if (result) {
        printf("The Give List is palindrome \r\n");
    } else {
        printf("The Give List is not a palindrome \r\n");
    }

    return;
}