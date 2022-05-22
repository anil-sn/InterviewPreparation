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
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *partition (struct ListNode *head, int x)
{
    struct ListNode node1;
    node1.val = 0;
    struct ListNode node2;
    node2.val = 0;
    struct ListNode *p1 = &node1;
    struct ListNode *p2 = &node2;

    while (head) {
        if (head->val < x) {
            p1->next = head;
            p1 = p1->next;
        } else {
            p2->next = head;
            p2 = p2->next;
        }

        head = head->next;
    }

    p2->next = NULL;
    p1->next = node2.next;
    return node1.next;
}