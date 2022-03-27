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
    3.4 Queue via Stacks: Implement a MyQueue class which implements a queue using two stacks.

    SOLUTION
        Since the major difference between a queue and a stack is the order (first-in first-out vs. last-in first-out),
        we know that we need to modify peek() and pop() to go in reverse order. We can use our second stack to
        reverse the order of the elements (by popping sl and pushing the elements on to s2). In such an imple­
        mentation, on each peek() and pop() operation, we would pop everything from sl onto s2, perform
        the peek/ pop operation, and then push everything back.
        This will work, but if two pop/ peeks are performed back-to-back, we're needlessly moving elements. We
        can implement a "lazy" approach where we let the elements sit in s2 until we absolutely must reverse the
        elements.
        In this approach, stackNewest has the newest elements on top and stackOldest has the oldest
        elements on top. When we dequeue an element, we want to remove the oldest element first, and so we
        dequeue from stackOldest. If stackOldest is empty, then we want to transfer all elements from
        stackNewest into this stack in reverse order. To insert an element, we push onto stackNewest, since it
        has the newest elements on top.

        During your actual interview, you may find that you forget the exact API calls. Don't stress too much if that
        happens to you. Most interviewers are okay with your asking for them to refresh your memory on little
        details. They're much more concerned with your big picture understanding.

*/

/* structure of a stack node */

typedef struct Node {
    int data;
    struct Node* next;
} NODE;


/* structure of queue having two stacks */
typedef struct queue {
    NODE* stack1;
    NODE* stack2;
} QUEUE;

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

/* Function to enqueue an item to queue */
void
enQueue(QUEUE* q, int x) {
    push(&q->stack1, x);
}

/* Function to deQueue an item from queue */
int
deQueue(QUEUE* q) {
    int x;

    /* If both stacks are empty then error */
    if (q->stack1 == NULL && q->stack2 == NULL) {
        printf("Q is empty");
        getchar();
        exit(0);
    }

    /* Move elements from stack1 to stack 2 only if
       stack2 is empty */
    if (q->stack2 == NULL) {
        while (q->stack1 != NULL) {
            x = pop(&q->stack1);
            push(&q->stack2, x);
        }
    }

    x = pop(&q->stack2);
    return x;
}


/* Function to deQueue an item from queue */
void
PrintQueue(QUEUE* q) {
    int x = 0;
    NODE* temp = NULL;

    /* If both stacks are empty then error */
    if (q->stack1 == NULL && q->stack2 == NULL) {
        printf("Q is empty \r\n");
    }

    x = 0;
    temp = q->stack1;
    printf("Stack 1 :");
    while (temp != NULL) {
        printf("[%d]%d ", x, temp->data);
        temp = temp->next;
        x++;
    }
    printf("\r\n");

    x = 0;
    temp = q->stack2;
    printf("Stack 2 :");
    while (temp != NULL) {
        printf("[%d]%d ", x, temp->data);
        temp = temp->next;
        x++;
    }

    printf("\r\n\r\n");
    return;
}


/* Driver function to test anove functions */
int
main() {
    QUEUE* q = (QUEUE*)malloc(sizeof(QUEUE));
    q->stack1 = NULL;
    q->stack2 = NULL;

    printf("EnQueue : %d ", 1);
    enQueue(q, 1);
    PrintQueue(q);

    printf("EnQueue : %d ", 2);
    enQueue(q, 2);
    PrintQueue(q);

    printf("EnQueue : %d ", 3);
    enQueue(q, 3);
    PrintQueue(q);

    printf("EnQueue : %d ", 4);
    enQueue(q, 4);
    PrintQueue(q);

    printf("EnQueue : %d ", 5);
    enQueue(q, 5);
    PrintQueue(q);

    printf("EnQueue : %d ", 10);
    enQueue(q, 10);
    PrintQueue(q);

    printf("EnQueue : %d ", 11);
    enQueue(q, 11);
    PrintQueue(q);

    /* Dequeue items */
    printf("DeQueue : %d ", deQueue(q));
    PrintQueue(q);
    printf("DeQueue : %d ", deQueue(q));
    PrintQueue(q);
    printf("DeQueue : %d ", deQueue(q));
    PrintQueue(q);

    printf("EnQueue : %d ", 6);
    enQueue(q, 6);
    PrintQueue(q);

    printf("EnQueue : %d ", 7);
    enQueue(q, 7);
    PrintQueue(q);

    printf("EnQueue : %d ", 8);
    enQueue(q, 8);
    PrintQueue(q);

    printf("EnQueue : %d ", 9);
    enQueue(q, 9);
    PrintQueue(q);

    return 0;
}
