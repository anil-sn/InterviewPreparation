#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define ARRAY_SIZE 10

typedef struct userData {
    u_int32_t data;
    u_int32_t nextMinimum;
} UserData;

/* Stack data structure This pointer will pointing at the actual field (of void * pointers) that represents the stack */
void **array;

/* The current capacity of the stack */
int max = ARRAY_SIZE;

/* Counter variable for counting the elements of the stack. */
int counter = 0;

/* offset address points at the top element of the stack. */
int offset = -1;

/* Minumum value of the stack element of the stack. */
int stackMin = UINT_MAX;

void
initStack() {

    /* increases the element-pointer */
    array = malloc(sizeof(void *) * max);

    assert(array);
}

/* size: gets the number of elements of the stack. */
int
size() {
    return counter;
}

/* isEmpty(): returns 1 if stack is empty otherwise 0. */
int
isEmpty() {
    return (counter == 0);
}

/* top: returns the top element from the stack without removing it. */
void *
top() {

    /* offset address points to the top element */
    return array[offset];
}

/* grow: increases the stack by 10 elements. This utility function isn't part of the public interface */
void
grow() {

    /* increases the capacity */
    max += ARRAY_SIZE;

    int i;
    void **tmp = malloc(sizeof(void *) * max);

    /* copies the elements from the origin array in the new one. */
    for (i = 0; i < max - ARRAY_SIZE; i++) {
        *(tmp + i) = *(array + i);
    }

    /*free the memory */
    free(array);
    array = tmp;
}

/* push: pushs the argument onto the stack */
void
push(void *object)
{
    /* tests whether pointer isn't null */
    assert(object);

    if (((UserData *)object)->data < stackMin) {
        stackMin = ((UserData *)object)->data;
    }

    ((UserData *)object)->nextMinimum = stackMin;

    if (counter < max) {

        /* increases the element-pointer */
        offset++;

        /* moves pointer by the offset address pushs the object onto stack */
        *(array + offset) = object;

        /* increases the inner counter */
        counter++;
    }
    else /* stack is full */
    {
        grow();       /* lets grow stack */
        push(object); /* recursive call */
    }
}

/* pop: pops the top element of the stack from the stack. */
void *
pop() {
    void *top = *(array + offset);

    /* check pointers */
    assert(top);

    /* if use the pop-function, stack must not empty. */
    assert(!isEmpty());

    if (((UserData *)top)->data == stackMin) {
        stackMin = ((UserData *)(*(array + (offset - 1))))->nextMinimum;
    }

    /* decreases the offset address for pointing of the new top element */
    offset--;

    /* decreases the inner counter */
    counter--;

    return top;
}


/*
    3.2 Stack Min: How would you design a stack which, in addition to push and pop, has a function min
                   which returns the minimum element? Push, pop and min should all operate in 0(1) time.
    SOLUTION
        The thing with minimums is that they don't change very often. They only change when a smaller element is added.

        One solution is to have just a single int value, minValue, that's a member of the Stack class. When
        minValue is popped from the stack, we search through the stack to find the new minimum. Unfortunately,
        this would break the constraint that push and pop operate in 0( 1) time.

    To further understand this question, let's walk through it with a short example:
        push(5); // stack is {5}, min is 5
        push(6); // stack is {6, 5}, min is 5
        push(3); // stack is {3, 6, 5}, min is 3
        push(7); // stack is {7, 3, 6, 5}, min is 3
        pop(); // pops 7. stack is {3, 6, 5}, min is 3
        pop(); // pops 3. stack is {6, 5}. min is 5.

    Observe how once the stack goes back to a prior state ( { 6, 5} ), the minimum also goes back to its prior
    state (5). This leads us to our second solution. If we kept track of the minimum at each state,
    we would be able to easily know the minimum. We can do this by having each node record what the minimum beneath
    itself is. Then, to find the min, you just look at what the top element thinks is the min.
    When you push an element onto the stack, the element is given the current minimum. It sets its "localmin"to be the min.

*/

/* Function to print nodes in a given linked list */
void
printArray() {

    int i = 0;
    UserData *ptr = NULL;

    for (i = 0; i < counter; i++) {
        ptr = (UserData *)(*(array + i));
        printf("[%d]->%d,%d ", i, ptr->data, ptr->nextMinimum);
    }

    printf ("Minimum : %d \r\n", stackMin);
    return;
}

int
main () {

    UserData *ptr = NULL;
    UserData *delPtr = NULL;

    /* Initilize the stack */
    initStack();

    ptr = malloc(sizeof(UserData));
    printf("Push element 25 to stack \r\n");
    ptr->data = 25;
    ptr->nextMinimum = UINT_MAX;
    push((void *)ptr);
    printArray();

    ptr = malloc(sizeof(UserData));
    printf("Push element 20 to stack \r\n");
    ptr->data = 20;
    ptr->nextMinimum = UINT_MAX;
    push((void *)ptr);
    printArray();

    ptr = malloc(sizeof(UserData));
    printf("Push element 30 to stack \r\n");
    ptr->data = 30;
    ptr->nextMinimum = UINT_MAX;
    push((void *)ptr);
    printArray();

    delPtr = malloc(sizeof(UserData));
    printf("Push element 15 to stack \r\n");
    delPtr->data = 15;
    delPtr->nextMinimum = UINT_MAX;
    push((void *)delPtr);
    printArray();

    ptr = malloc(sizeof(UserData));
    printf("Push element 50 to stack \r\n");
    ptr->data = 50;
    ptr->nextMinimum = UINT_MAX;
    push((void *)ptr);
    printArray();

    printf("POP element 50 from stack \r\n");
    pop();
    printArray();

    printf("POP element 15 from stack \r\n");
    pop();
    printArray();

    return 0;
}
