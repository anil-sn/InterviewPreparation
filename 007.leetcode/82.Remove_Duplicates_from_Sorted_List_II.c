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

struct ListNode *deleteDuplicates (struct ListNode *head)
{
    struct ListNode *itor;
    struct ListNode *result_ind;
    struct ListNode *result_head;
    int prev_val;
    result_head = NULL;
    result_ind = NULL;

    if (head == NULL) {
        return NULL;
    }

    prev_val = head->val - 1;

    for (itor = head; itor != NULL; itor = itor->next) {
        if ((itor->next != NULL && itor->val == itor->next->val) || itor->val == prev_val) {
            prev_val = itor->val;
            continue;
        } else {
            prev_val = itor->val;

            if (result_head == NULL) {
                result_head = itor;
                result_ind = result_head;
            } else {
                result_ind->next = itor;
                result_ind = result_ind->next;
            }
        }
    }

    if (result_ind == NULL) {
        return NULL;
    }

    result_ind->next = NULL;
    return result_head;
}