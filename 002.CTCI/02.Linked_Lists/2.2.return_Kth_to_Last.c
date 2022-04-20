#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
    2.2 Return Kth to Last: Implement an algorithm to find the kth to last element of a singly linked list.

    SOLUTION
    We will approach this problem both recursively and non-recursively. Remember that recursive solutions are
    often cleaner but less optimal. For example, in this problem, the recursive implementation is about half the
    length of the iterative solution but also takes 0(n) space, where n is the number of elements in the linked
    list. Note that for this solution, we have defined k such that passing in k = 1 would return the last element, k =
    2 would return to the second to last element, and so on. It is equally acceptable to define k such that k = 0
    would return the last element.

    Solution #1: If linked list size is known
    If the size of the linked list is known, then the kth to last element is the ( length - k)th element. We can
    just iterate through the linked list to find this element. Because this solution is so trivial, we can almost be
    sure that this is not what the interviewer intended.

    Solution #2: Recursive
    This algorithm recurses through the linked list. When it hits the end, the method passes back a counter set
    to 0. Each parent call adds 1 to this counter. When the counter equals k, we know we have reached the kth
    to last element of the linked list.

    Implementing this is short and sweet-provided we have a way of "passing back" an integer value through
    the stack. Unfortunately, we can't pass back a node and a counter using normal return statements. So how
    do we handle this?

    Iterative
    A more optimal, but less straightforward, solution is to implement this iteratively. We can use two pointers,
    pl and p2. We place them k nodes apart in the linked list by putting p2 at the beginning and moving pl
    k nodes into the list. Then, when we move them at the same pace, pl will hit the end of the linked list after
    LENGTH - k steps. At that point, p2 will be LENGTH - k nodes into the list, or k nodes from the end.


*/


/* A linked list node */
typedef struct node {
    int32_t data;
    struct node *next;
} NODE;

/* Utility function to create a new Node */
NODE *
newNode(int32_t data) {
   NODE *temp = malloc(sizeof(NODE));
   temp->data = data;
   temp->next = NULL;
   return temp;
}

/* Function to print nodes in a given linked list */
void
printList(NODE *node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }

    printf ("\r\n");
    return;
}

int32_t
printKthToLast(NODE *head, int32_t k) {

    int32_t index = 0;
    if (head == NULL) {
        return 0;
    }

    //printf("Data : %d, K : %d\r\n", head->data, k);
    index = printKthToLast(head->next, k) + 1;
    //printf("Index: %d\r\n", index);

    if (index == k) {
        printf("Recursive Method : %dth to last node is %d\r\n", k, head->data);
        return index;
    }
}

NODE*
nthTolast(NODE *head, int k) {
    NODE *p1 = head;
    NODE *p2 = head;

    /* Move p1 k nodes into the list.*/
    for (int i= 0; i < k; i++) {
        if (p1 == NULL) {
            return NULL; // Out of bounds
        }
        p1 = p1->next;
    }

    /* Move them at the same pace. When pl hits the end, p2 will be at the right element. */
    while (p1 != NULL) {
        p1 = p1->next;
        p2 = p2->next;
    }

    return p2;
}

int main () {

    /* The constructed linked list is: 10->12->11->11->12->11->10 */
    NODE *nthToLast = NULL;
    NODE *start = newNode(10);
    start->next = newNode(12);
    start->next->next = newNode(11);
    start->next->next->next = newNode(11);
    start->next->next->next->next = newNode(12);
    start->next->next->next->next->next = newNode(11);
    start->next->next->next->next->next->next = newNode(10);

    printf("Linked list before removing duplicates ");
    printList(start);

    printKthToLast(start, 3);

    nthToLast = nthTolast(start, 3);
    printf("Iterative Method : %dth to last node is %d\r\n", 3, nthToLast->data);

    return 0;
}
