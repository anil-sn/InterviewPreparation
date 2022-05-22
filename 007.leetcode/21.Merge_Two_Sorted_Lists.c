/*
21. Merge Two Sorted Lists

Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.
*/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode *mergeTwoLists (struct ListNode *l1, struct ListNode *l2)
{
    if (l1 == NULL) {
        return l2;
    }

    if (l2 == NULL) {
        return l1;
    }

    if (l1->val < l2->val) {
        l1->next = mergeTwoLists (l1->next, l2);
        return l1;
    } else {
        l2->next = mergeTwoLists (l2->next, l1);
        return l2;
    }
}

struct ListNode *mergeTwoLists (struct ListNode *list1, struct ListNode *list2)
{
    // if list1 happen to be NULL
    // we will simply return list2.
    if (list1 == NULL)
    { return list2; }

    // if list2 happen to be NULL
    // we will simply return list1.
    if (list2 == NULL)
    { return list1; }

    struct ListNode *ptr = list1;

    if (list1 -> val > list2 -> val) {
        ptr = list2;
        list2 = list2 -> next;
    } else {
        list1 = list1 -> next;
    }

    struct ListNode *curr = ptr;

    // till one of the list doesn't reaches NULL
    while (list1 &&  list2) {
        if (list1 -> val < list2 -> val) {
            curr->next = list1;
            list1 = list1 -> next;
        } else {
            curr->next = list2;
            list2 = list2 -> next;
        }

        curr = curr -> next;
    }

    // adding remaining elements of bigger list.
    if (!list1)
    { curr -> next = list2; }
    else
    { curr -> next = list1; }

    return ptr;
}

/*
Difficulty:Easy
Total Accepted:244K
Total Submissions:624.7K


Companies Amazon LinkedIn Apple Microsoft
Related Topics Linked List
Similar Questions


                    Merge k Sorted Lists

                    Merge Sorted Array

                    Sort List

                    Shortest Word Distance II
*/
