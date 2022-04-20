#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*

    2.5 Sum Lists: You have two numbers represented by a linked list, where each node contains a single
        digit. The digits are stored in reverse order, such that the 1 's digit is at the head of the list. Write a
        function that adds the two numbers and returns the sum as a linked list.

    EXAMPLE
        Input: (7-> 1 -> 6) + (5 -> 9 -> 2).That is,617 + 295.
        Output: 2 -> 1 -> 9. That is, 912.

    FOLLOW UP
        Suppose the digits are stored in forward order. Repeat the above problem.
        Input: (6 -> 1 -> 7) + (2 -> 9 -> 5).That is,617 + 295.
        Output: 9 -> 1 -> 2. That is, 912.

    SOLUTION
    It's useful to remember in this problem how exactly addition works.

    Imagine the problem:
        6 1 7 + 2 9 5
    First, we add 7 and 5 to get 12. The digit 2 becomes the last digit of the number, and 1 gets carried over to
    the next step.
    Second, we add 1, 1, and 9 to get 11. The 1 becomes the second digit, and the other 1 gets
    carried over the final step.
    Third and finally, we add 1, 6 and 2 to get 9. So, our value becomes 912.

    We can mimic this process recursively by adding node by node, carrying over any "excess" data to the next
    node.

    Let's walk through this for the below linked list:
    7 -> 1 -> 6  +  5 -> 9 -> 2

    We do the following:
    1. We add 7 and 5 first, getting a result of 12. 2 becomes the first node in our linked list, and we "carry" the
        1 to the next sum.
    List: 2 -> ?

    2. We then add 1 and 9, as well as the "carry;' getting a result of 11. 1 becomes the second element of our
        linked list, and we carry the 1 to the next sum.
    List: 2 -> 1 ->?

    3. Finally, we add 6, 2 and our"carrY:'to get 9. This becomes the final element of our linked list.
    List: 2 -> 1 -> 9.
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

NODE *
addlists(NODE * l1, NODE * l2, int carry) {

    int32_t value = carry;

    if (l1 == NULL && l2 == NULL && carry == 0) {
        return NULL;
    }

    NODE *result = newNode(0);
    if (l1 != NULL) {
        value += l1->data;
    }
    if (l2 != NULL) {
        value += l2->data;
    }

    printf ("(carry %d) + %d + %d = %d\r\n", carry, l1->data, l2->data, value);

    result->data = value % 10; /* Second digit of number */

    /*Recurse */
    if (l1 != NULL || l2 != NULL) {
        NODE *more = addlists(l1 == NULL ? NULL : l1->next,
                              l2 == NULL ? NULL : l2->next,
                              value >= 10 ? (1) : 0);

        result->next = more;
    }

    return result;
}

int main () {

    /* The constructed linked list is: 7->1->6 */
    /* The constructed linked list is: 5->9->2 */

    NODE *l1 = newNode(7);
    l1->next = newNode(1);
    l1->next->next = newNode(6);

    NODE *l2 = newNode(5);
    l2->next = newNode(9);
    l2->next->next = newNode(2);

    NODE *sum = addlists(l1, l2, 0);

    printf("Linked list After : ");
    printList(sum);

    return 0;
}

