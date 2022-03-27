#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*

    2.6 Palindrome: Implement a function to check if a linked list is a palindrome.
    SOLUTION
        To approach this problem, we can picture a palindrome like 0 - > 1 - > 2 - > 1 - > 0. We know that,
        since it's a palindrome, the list must be the same backwards and forwards.

    Recursive Approach :
        First, a word on notation: in this solution, when we use the notation node Kx, the variable K indicates the
        value of the node data, and x (which is either for b) indicates whether we are referring to the front node
        with that value or the back node. For example, in the below linked list node 2b would refer to the second
        (back) node with value 2.

        Now, like many linked list problems, you can approach this problem recursively. We may have some intuitive idea
        that we want to compare element 0 and element n - 1, element 1 and element n - 2, element 2 and element n-3,
        and so on, until the middle element(s). For example:

        0 ( 1 ( 2 ( 3 ) 2 ) 1 ) 0

        In order to apply this approach, we first need to know when we've reached the middle element, as this will
        form our base case. We can do this by passing in length - 2 for the length each time. When the length
        equals 0 or 1, we're at the center of the linked list. This is because the length is reduced by 2 each time.
        Once we've recursed N/2 times, length will be down to 0.

        recurse(Node n, int length) {
            if (length == 0 || length== 1) {
                return [something]; // At middle
            }
            recurse(n.next, length - 2);
            ...
        }

        This method will form the outline of the isPalindrome method. The "meat" of the algorithm though is
        comparing node i to node n - i to check if the linked list is a palindrome. How do we do that?

        Let's examine what the call stack looks like:
        v1 isPalindrome: list = 0 ( 1 ( 2 ( 3 ) 2 ) 1 ) 0. length = 7
            v2 isPalindrome: list = 1 ( 2 ( 3 ) 2 ) 1 ) 0. length = 5
                v3 = isPalindrome: list = 2 ( 3 ) 2 ) 1 ) 0. length = 3
                    v4 = is Palindrome: list = 3 ) 2 ) 1 ) 0. length = 1
                    returns v3
                returns v2
            returns v1
        returns ?

        In the above call stack, each call wants to check if the list is a palindrome by comparing its head node with
        the corresponding node from the back of the list. That is:
        • Line 1 needs to compare node 0f with node 0b
        • Line 2 needs to compare node 1 f with node lb
        • Line 3 needs to compare node 2f with node 2b
        • Line 4 needs to compare node 3f with node 3b.

        If we rewind the stack, passing nodes back as described below, we can do just that:
        • Line 4 sees that it is the middle node (since length = 1), and passes back head. next. The value head
          equals node 3, so head. next is node 2b.
        • Line 3 compares its head, node 2f, to returned_node (the value from the previous recursive call),
          which is node 2b. if the values match, it passes a reference to node lb (returned_node. next) up
          to line 2.
        • Line 2 compares its head (node 1 f) to returned_node (node lb). If the values match, it passes a
          reference to node 0b (or, returned_node. next) up to line 1.
        • Line 1 compares its head, node 0f, to returned_node, which is node 0b. If the values match, it
          returns true.

        To generalize, each call compares its head to returned_node, and then passes returned_node.next
        up the stack. In this way, every node i gets compared to node n - i. If at any point the values do not
        match, we return false, and every call up the stack checks for that value.
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

/* Helper function to create a new node with the given data and pushes it onto the list's front */
void
push(NODE** head, int32_t data)
{
    /* create a new linked list node from the heap */
    NODE *newNode = malloc(sizeof(NODE));

    newNode->data = data;
    newNode->next = *head;
    *head = newNode;

    return;
}

// Recursive function to check if the linked list is a palindrome or not
int checkPalindrome(NODE** left, NODE* right)
{
    // base case
    if (right == NULL) {
        return 1;
    }

    int result = checkPalindrome(left, right->next) && ((*left)->data == right->data);
    printf ("%d == %d\r\n", (*left)->data, right->data);
    (*left) = (*left)->next;

    return result;
}

// Function to check if the linked list is a palindrome or not
int checkPalin(NODE* head) {
    return checkPalindrome(&head, head);
}

int32_t
main(void) {

    // input keys
    int32_t keys[] = { 1, 3, 5, 3, 1};
    int32_t n = sizeof(keys) / sizeof(keys[0]);

    NODE* head = NULL;
    for (int i = n - 1; i >= 0; i--) {
        push(&head, keys[i]);
    }

    if (checkPalin(head)) {
        printf("The linked list is a palindrome");
    }
    else {
        printf("The linked list is not a palindrome");
    }

    return 0;
}


