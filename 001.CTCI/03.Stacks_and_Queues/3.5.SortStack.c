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
    3.5 Sort Stack: Write a program to sort a stack such that the smallest items are on the top. You can use
        an additional temporary stack, but you may not copy the elements into any other data structure
        (such as an array). The stack supports the following operations: push, pop, peek, and is Empty.

        Create a temporary stack say tmpStack. While input stack is NOT empty do this:
        Pop an element from input stack call it temp
        while temporary stack is NOT empty and top of temporary stack is greater than temp,
        pop from temporary stack and push it to the input stack
        push temp in temporary stack
        The sorted numbers are in tmpStack
        Here is a dry run of the above pseudo code.

        input: [34, 3, 31, 98, 92, 23]

        Element taken out: 23
        input: [34, 3, 31, 98, 92]
        tmpStack: [23]

        Element taken out: 92
        input: [34, 3, 31, 98]
        tmpStack: [23, 92]

        Element taken out: 98
        input: [34, 3, 31]
        tmpStack: [23, 92, 98]

        Element taken out: 31
        input: [34, 3, 98, 92]
        tmpStack: [23, 31]

        Element taken out: 92
        input: [34, 3, 98]
        tmpStack: [23, 31, 92]

        Element taken out: 98
        input: [34, 3]
        tmpStack: [23, 31, 92, 98]

        Element taken out: 3
        input: [34, 98, 92, 31, 23]
        tmpStack: [3]

        Element taken out: 23
        input: [34, 98, 92, 31]
        tmpStack: [3, 23]

        Element taken out: 31
        input: [34, 98, 92]
        tmpStack: [3, 23, 31]

        Element taken out: 92
        input: [34, 98]
        tmpStack: [3, 23, 31, 92]

        Element taken out: 98
        input: [34]
        tmpStack: [3, 23, 31, 92, 98]

        Element taken out: 34
        input: [98, 92]
        tmpStack: [3, 23, 31, 34]

        Element taken out: 92
        input: [98]
        tmpStack: [3, 23, 31, 34, 92]

        Element taken out: 98
        input: []
        tmpStack: [3, 23, 31, 34, 92, 98]

        final sorted list: [3, 23, 31, 34, 92, 98]

*/

typedef struct Node {
    int data;
    struct Node* next;
} NODE;

/* Function to push an item to stack*/
void
push(NODE** top_ref, int new_data) {
    /* allocate node */
    NODE* new_node = (NODE*)malloc(sizeof(NODE));
    if (new_node == NULL) {
        printf("Stack overflow \n");
        getchar();
        exit(0);
    }

    /* put in the data */
    new_node->data = new_data;

    /* link the old list off the new node */
    new_node->next = (*top_ref);

    /* move the head to point to the new node */
    (*top_ref) = new_node;
}

/* Function to pop an item from stack*/
int
pop(NODE** top_ref) {
    int res;
    NODE* top;

    /*If stack is empty then error */
    if (*top_ref == NULL) {
        printf("Stack underflow \n");
        getchar();
        exit(0);
    }
    else {
        top = *top_ref;
        res = top->data;
        *top_ref = top->next;
        free(top);
        return res;
    }
}

/* Function to deQueue an item from queue */
void
PrintStack(NODE* q) {

    int x = 0;
    NODE* temp = q;

    printf("Stack :");
    while (temp != NULL) {
        printf("[%d]%d ", x, temp->data);
        temp = temp->next;
        x++;
    }

    printf("\r\n");

    return;
}

// This function return the sorted stack
NODE *
sortStack(NODE **stack)
{
    NODE *tmpStack = NULL;
    NODE **preserve = stack;

    while (NULL != *stack) {

        // pop out the first element
        int tmp = pop(stack);

        // while temporary stack is not empty and top of stack is greater than temp
        while ((NULL != tmpStack) && (tmpStack->data > tmp)) {

            // pop from temporary stack and push
            // it to the input stack
            push(stack, pop(&tmpStack));
        }

        // push temp in temporary of stack
        push(&tmpStack, tmp);
    }

    PrintStack(tmpStack);
    return tmpStack;
}

void
main () {

    NODE *Stack = NULL;
    NODE *sortedStack = NULL;
    push(&Stack, 34);
    push(&Stack, 3);
    push(&Stack, 31);
    push(&Stack, 98);
    push(&Stack, 92);
    push(&Stack, 23);
    push(&Stack, 1);
    push(&Stack, 20);

    PrintStack(Stack);
    (void)sortStack(&Stack);
    return;
}
