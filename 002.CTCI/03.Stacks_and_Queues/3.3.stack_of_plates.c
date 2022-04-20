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
    3.3 Stack of Plates:
        Imagine a (literal) stack of plates. If the stack gets too high, it might topple.
        Therefore, in real life, we would likely start a new stack when the previous stack exceeds some
        threshold. Implement a data structure SetOfStacks that mimics this. SetOfStacks should be
        composed of several stacks and should create a new stack once the previous one exceeds capacity.
        SetOfStacks. push() and SetOfStacks. pop() should behave identically to a single stack
        (that is, pop () should return the same values as it would if there were just a single stack).
    FOLLOW UP
        Implement a function popAt(int index) which performs a pop operation on a specific sub­
        stack.

*/

#define MAX_STACK_SIZE 5
#define STACK_SET_SIZE 5

typedef struct userData {
    u_int32_t data;
} UserData;

typedef struct stack {

    /* Stack data structure This pointer will pointing at the actual field (of void * pointers) that represents the stack */
    void **array;

    /* The Max capacity of the stack */
    int max;

    /* Counter variable for counting the elements of the stack. */
    int counter;

    /* offset address points at the top element of the stack. */
    int offset;
} STACK;

typedef struct stackSet {

    STACK *set[STACK_SET_SIZE];

} STACK_SET;

void
initStack(STACK **inStack) {

    STACK *stack = NULL;

    stack = malloc(sizeof(STACK));
    assert(stack);

    stack->counter = 0;
    stack->max = MAX_STACK_SIZE;
    stack->offset = -1;

    /* increases the element-pointer */
    stack->array = malloc(sizeof(void *) * MAX_STACK_SIZE);
    assert(stack->array);

    *inStack = stack;
    return;
}

void
initStackSet(STACK_SET **stackSet) {

    STACK *inStack = NULL;

    /* increases the element-pointer */
    *stackSet = malloc(sizeof(STACK_SET));
    assert(*stackSet);

    initStack(&inStack);
    (*stackSet)->set[0] = inStack;

    initStack(&inStack);
    (*stackSet)->set[1] = inStack;

    initStack(&inStack);
    (*stackSet)->set[2] = inStack;

    initStack(&inStack);
    (*stackSet)->set[3] = inStack;

    initStack(&inStack);
    (*stackSet)->set[4] = inStack;

    return;
}

/* size: gets the number of elements of the stack. */
int
getSize(STACK_SET *stackSet) {

    int size = 0;

    size += stackSet->set[0]->counter;
    size += stackSet->set[1]->counter;
    size += stackSet->set[2]->counter;
    size += stackSet->set[3]->counter;
    size += stackSet->set[4]->counter;

    return size;
}

/* isEmpty(): returns 1 if stack is empty otherwise 0. */
int
isEmpty(STACK_SET *stackSet) {

    int size = getSize(stackSet);

    return (0 == size);
}

/* top: returns the top element from the stack without removing it. */
void *
top(STACK_SET *stackSet) {

    int i;
    void *top = NULL;
    for (i = 0; i < STACK_SET_SIZE; i ++) {
        if (stackSet->set[i]->array[stackSet->set[i]->offset]) {
            top = stackSet->set[i]->array[stackSet->set[i]->offset];
        }
    }

    /* offset address points to the top element */
    return top;
}

/* top: returns the top element from the stack without removing it. */
int
GetNextSlotToPush(STACK_SET *stackSet) {

    int i = 0;
    void *top = NULL;
    for (i = 0; i < STACK_SET_SIZE; i ++) {
        if (stackSet->set[i]->counter < stackSet->set[i]->max) {
            return i;
        }
    }

    /** Grow Stack Set */
    assert(0);
    return i;
}

/* top: returns the top element from the stack without removing it. */
int
GetNextSlotToPop(STACK_SET *stackSet) {

    int prev = 0;
    int i = 0;
    void *top = NULL;
    for (i = 0; i < STACK_SET_SIZE; i ++) {
        if (stackSet->set[i]->counter == stackSet->set[i]->max) {
            prev = i;
        }

        if (stackSet->set[i]->counter == 0) {
            return prev;
        }

        if (stackSet->set[i]->counter < stackSet->set[i]->max) {
            return i;
        }
    }

    /** Grow Stack Set */
    assert(0);
    return i;
}

/* push: pushs the argument onto the stack */
void
push(STACK_SET *stackSet, void *object)
{
    int i = 0;

    /* tests whether pointer isn't null */
    assert(object);

    i = GetNextSlotToPush(stackSet);
    if (stackSet->set[i]->counter < stackSet->set[i]->max) {

        /* increases the element-pointer */
        stackSet->set[i]->offset++;

        /* moves pointer by the offset address pushs the object onto stack */
        *(stackSet->set[i]->array + stackSet->set[i]->offset) = object;

        /* increases the inner counter */
        stackSet->set[i]->counter++;
    }
    else /* stack is full */
    {
        assert(0);
    }
}

/* pop: pops the top element of the stack from the stack. */
void *
pop(STACK_SET *stackSet) {

    int i = 0;
    i = GetNextSlotToPop(stackSet);

    void *top = *(stackSet->set[i]->array + stackSet->set[i]->offset);

    /* check pointers */
    assert(top);

    /* if use the pop-function, stack must not empty. */
    assert(!isEmpty(stackSet));

    /* decreases the offset address for pointing of the new top element */
    stackSet->set[i]->offset--;

    /* decreases the inner counter */
    stackSet->set[i]->counter--;

    return top;
}

void
printArray(STACK_SET *stackSet) {

    int i = 0;
    int j = 0;
    void *top = NULL;
    UserData *ptr = NULL;

    for (i = 0; i < STACK_SET_SIZE; i ++) {
        printf("Stack Number %d \r\n ", i);
        for (j = 0; j < stackSet->set[i]->counter; j++) {
            ptr = (UserData *)(*(stackSet->set[i]->array + j));
            printf("[%d]->%d ", j, ptr->data);
        }
    }

    printf("\r\n");
    return;
}

int
main () {

    UserData *ptr = NULL;
    STACK_SET *stackSet = NULL;

    /* Initilize the stack */
    initStackSet(&stackSet);

    ptr = malloc(sizeof(UserData));
    printf("Push element 25 to stack \r\n");
    ptr->data = 25;
    push(stackSet, (void *)ptr);
    printArray(stackSet);

    ptr = malloc(sizeof(UserData));
    printf("Push element 20 to stack \r\n");
    ptr->data = 20;
    push(stackSet, (void *)ptr);
    printArray(stackSet);

    ptr = malloc(sizeof(UserData));
    printf("Push element 30 to stack \r\n");
    ptr->data = 30;
    push(stackSet, (void *)ptr);
    printArray(stackSet);

    ptr = malloc(sizeof(UserData));
    printf("Push element 15 to stack \r\n");
    ptr->data = 15;
    push(stackSet, (void *)ptr);
    printArray(stackSet);

    ptr = malloc(sizeof(UserData));
    printf("Push element 50 to stack \r\n");
    ptr->data = 50;
    push(stackSet, (void *)ptr);
    printArray(stackSet);

    ptr = malloc(sizeof(UserData));
    printf("Push element 60 to stack \r\n");
    ptr->data = 60;
    push(stackSet, (void *)ptr);
    printArray(stackSet);


    printf("POP element 60 from stack \r\n");
    pop(stackSet);
    printArray(stackSet);

    printf("POP element 50 from stack \r\n");
    pop(stackSet);
    printArray(stackSet);

    return 0;
}

