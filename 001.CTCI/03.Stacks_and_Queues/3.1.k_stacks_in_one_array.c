#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"

/*
    3.1 Three in One: Describe how you could use a single array to implement three stacks.
    SOLUTION
        Like many problems, this one somewhat depends on how well we'd like to support these stacks. If we're
        okay with simply allocating a fixed amount of space for each stack, we can do that. This may mean though
        that one stack runs out of space, while the others are nearly empty. Alternatively, we can be flexible in
        our space allocation, but this significantly increases the complexity of the problem.

        Create a data structure kStacks that represents k stacks. Implementation of kStacks should use only one array,
        i.e., k stacks should use the same array for storing elements. Following functions must be supported by kStacks.

        push(int x, int sn) –> pushes x to stack number ‘sn’ where sn is from 0 to k-1
        pop(int sn) –> pops an element from stack number ‘sn’ where sn is from 0 to k-1

        Method 1 (Divide the array in slots of size n/k)
        A simple way to implement k stacks is to divide the array in k slots of size n/k each,
        and fix the slots for different stacks, i.e., use arr[0] to arr[n/k-1] for first stack,
        and arr[n/k] to arr[2n/k-1] for stack2 where arr[] is the array to be used to implement
        two stacks and size of array be n.

        The problem with this method is inefficient use of array space. A stack push operation may result in stack
        overflow even if there is space available in arr[]. For example, say the k is 2 and array size (n) is 6
        and we push 3 elements to first and do not push anything to second second stack. When we push 4th element to first,
        there will be overflow even if we have space for 3 more elements in array.

        Method 2 (A space efficient implementation)
        The idea is to use two extra arrays for efficient implementation of k stacks in an array.
        This may not make much sense for integer stacks, but stack items can be large for example stacks of employees,
        students, etc where every item is of hundreds of bytes. For such large stacks,
        the extra space used is comparatively very less as we use two integer arrays as extra space.

        Following are the two extra arrays are used:
        1) top[]: This is of size k and stores indexes of top elements in all stacks.
        2) next[]: This is of size n and stores indexes of next item for the items in array arr[].
                   Here arr[] is actual array that stores k stacks.

        Together with k stacks, a stack of free slots in arr[] is also maintained.
        The top of this stack is stored in a variable ‘free’.

        All entries in top[] are initialized as -1 to indicate that all stacks are empty.
        All entries next[i] are initialized as i+1 because all slots are free initially and pointing to next slot.
        Top of free stack, ‘free’ is initialized as 0.

*/

/* Global Variables */
u_int32_t *arr;   // Array of size n to store actual content to be stored in stacks
u_int32_t *top;   // Array of size k to store indexes of top elements of stacks
u_int32_t *next;  // Array of size n to store next entry in all stacks and free list
u_int32_t n, k;
u_int32_t freeList;   // To store beginning index of free list

/* Function to print nodes in a given linked list */
void
printArray() {
    for (int i=0; i < n; i++) {
        printf("[%d]->%d ", i, arr[i]);
    }
    printf ("\r\n");

    for (int i=0; i < k; i++) {
        printf("Stack %d Top %d ", i, top[i]);
    }
    printf ("\r\n\r\n");

    return;
}


// A utility function to check if there is space available
bool isFull() {
    return (freeList == -1);
}

// To check whether stack number 'sn' is empty or not
bool isEmpty(u_int32_t sn)  {
    return (top[sn] == -1);
}

//create k stacks in an array of size n
void
Initilize_kStacks (u_int32_t k1, u_int32_t n1) {

    // Initialize n and k, and allocate memory for all arrays
    k = k1, n = n1;
    arr = malloc(sizeof(u_int32_t) * n);
    top = malloc(sizeof(u_int32_t) * k);
    next = malloc(sizeof(u_int32_t) * n);

    // Initialize all stacks as empty
    for (int i = 0; i < k; i++) {
        top[i] = -1;
    }

    // Initialize all spaces as free
    freeList = 0;
    for (int i=0; i<n-1; i++) {
        next[i] = i+1;
    }
    next[n-1] = -1;  // -1 is used to indicate end of free list

    return;
}

// To push an item in stack number 'sn' where sn is from 0 to k-1
void
push(u_int32_t item, u_int32_t sn) {

    // Overflow check
    if (isFull()) {
        printf("Stack Overflow\n");
        return;
    }

    int i = freeList;      // Store index of first free slot

    // Update index of free slot to index of next slot in free list
    freeList = next[i];

    // Update next of top and then top for stack number 'sn'
    next[i] = top[sn];
    top[sn] = i;

    // Put the item in array
    arr[i] = item;

    return;
}

// To push an item in stack number 'sn' where sn is from 0 to k-1
u_int32_t
pop(u_int32_t sn) {

    // Underflow check
    if (isEmpty(sn))
    {
         printf("\nStack Underflow\n");
         return UINT_MAX;
    }


    // Find index of top item in stack number 'sn'
    u_int32_t i = top[sn];

    top[sn] = next[i];  // Change top to store next of previous top

    // Attach the previous top to the beginning of free list
    next[i] = freeList;
    freeList = i;

    // Return the previous top item
    return arr[i];

}

int
main () {

    // Let us create 3 stacks in an array of size 10
    u_int32_t k = 3, n = 10;
    Initilize_kStacks(k, n);

    // Let us put some items in stack number 2
    printArray();

    printf("Push element 15 to stack 2\r\n");
    push(15, 2);
    printArray();

    printf("Push element 45 to stack 2\r\n");
    push(45, 2);
    printArray();

    // Let us put some items in stack number 1
    printf("Push element 17 to stack 1\r\n");
    push(17, 1);
    printArray();

    printf ("Stack 2: 15 -> 45 -> 17\r\n");

    printf("Push element 49 to stack 1\r\n");
    push(49, 1);
    printArray();

    printf("Push element 39 to stack 1\r\n");
    push(39, 1);
    printArray();

    printf ("Stack 1: 49 -> 39 \r\n");

    // Let us put some items in stack number 0
    printf("Push element 11 to stack 0\r\n");
    push(11, 0);
    printArray();

        printf("Push element 9 to stack 0\r\n");
    push(9, 0);
    printArray();

    printf("Push element 7 to stack 0\r\n");
    push(7, 0);
    printArray();

    printf ("Stack 0: 11 -> 9 -> 7\r\n");

    printf("Popped element from stack 2 is %d\r\n", pop(2));
    printArray();
    printf("Popped element from stack 1 is %d\r\n", pop(1));
    printArray();
    printf("Popped element from stack 0 is %d\r\n", pop(0));
    printArray();

    return 0;
}
