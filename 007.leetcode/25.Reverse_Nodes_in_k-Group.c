/*
25. Reverse Nodes in k-Group

Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.



k is a positive integer and is less than or equal to the length of the linked list. If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.

You may not alter the values in the nodes, only nodes itself may be changed.

Only constant memory is allowed.


For example,
Given this linked list: 1->2->3->4->5



For k = 2, you should return: 2->1->4->3->5



For k = 3, you should return: 3->2->1->4->5
*/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

/*
    -1 -> 1 -> 2 -> 3 -> 4 -> 5
    |    |    |    |
    pre  cur  nex  tmp

    -1 -> 2 -> 1 -> 3 -> 4 -> 5
    |         |    |    |
    pre       cur  nex  tmp

    -1 -> 3 -> 2 -> 1 -> 4 -> 5
    |              |    |    |
    pre            cur  nex  tmp
    Above is how it works inside one group iteration(for example, k=3)
*/
struct ListNode *reverseKGroup (struct ListNode *head, int k)
{
    if (head == NULL || k == 1) {
        return head;
    }

    int num = 0;
    struct ListNode *preheader = calloc (1, sizeof (struct ListNode));
    preheader->val = -1;
    preheader->next = head;
    struct ListNode *cur = preheader, *nex, *pre = preheader;

    while (cur = cur->next)
    { num++; }

    while (num >= k) {
        cur = pre->next;
        nex = cur->next;

        for (int i = 1; i < k; ++i) {
            cur->next = nex->next;
            nex->next = pre->next;
            pre->next = nex;
            nex = cur->next;
        }

        pre = cur;
        num -= k;
    }

    return preheader->next;
}

/*
Difficulty:Hard
Total Accepted:99.1K
Total Submissions:322.6K


Companies Microsoft Facebook
Related Topics Linked List
Similar Questions


                    Swap Nodes in Pairs
*/
