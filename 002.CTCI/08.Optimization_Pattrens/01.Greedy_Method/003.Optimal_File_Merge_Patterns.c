#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

//Heap
int pq_size = 0;

// Function to heapify the tree
/*
Algorithm for “heapify”:

heapify(array)
   Root = array[0]
   Largest = largest( array[0] , array [2 * 0 + 1]. array[2 * 0 + 2])
   if(Root != Largest)
       Swap(Root, Largest)

    Example of “heapify”:

            30(0)
        /   \
        70(1)   50(2)

    Child (70(1)) is greater than the parent (30(0))

    Swap Child (70(1)) with the parent (30(0))
            70(0)
        /   \
        30(1)   50(2)

    Heap Sort Algorithm for sorting in increasing order:
    1. Build a max heap from the input data.
    2. At this point, the largest item is stored at the root of the heap.
       Replace it with the last item of the heap followed by reducing the size of heap by 1.
       Finally, heapify the root of the tree.
    3. Repeat step 2 while the size of the heap is greater than 1.

    How to build the heap?
        Heapify procedure can be applied to a node only if its children nodes are heapified.
        So the heapification must be performed in the bottom-up order.
        Lets understand with the help of an example:

    Input data: 4, 10, 3, 5, 1
            4(0)
            /   \
        10(1)   3(2)
        /   \
    5(3)    1(4)

    The numbers in bracket represent the indices in the array
    representation of data.

    Applying heapify procedure to index 1:
            4(0)
            /   \
        10(1)    3(2)
        /   \
    5(3)    1(4)

    Applying heapify procedure to index 0:
            10(0)
            /  \
        5(1)  3(2)
        /   \
    4(3)    1(4)
    The heapify procedure calls itself recursively to build heap
    in top down manner.
*/
void max_heapify(int array[], int size, int i) {
    if (pq_size == 1) {
        printf("Single element in the heap");
    } else {
        // Find the largest among root, left child and right child
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < pq_size && array[l] > array[largest]) {
            largest = l;
        }

        if (r < pq_size && array[r] > array[largest]) {
            largest = r;
        }

        // Swap and continue heapifying if root is not largest
        if (largest != i) {
            swap(&array[i], &array[largest]);
            max_heapify(array, pq_size, largest);
        }
    }
}

// Function to heapify the tree
void min_heapify(int array[], int size, int i) {
    if (pq_size == 1) {
        printf("Single element in the heap");
    } else {
        // Find the smallest among root, left child and right child
        int smallest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < pq_size && array[l] < array[smallest]) {
            smallest = l;
        }

        if (r < pq_size && array[r] < array[smallest]) {
            smallest = r;
        }

        // Swap and continue heapifying if root is not largest
        if (smallest != i) {
            swap(&array[i], &array[smallest]);
            min_heapify(array, pq_size, smallest);
        }
    }
}

// Function to pq_insert an element into the tree
void pq_insert(int array[], int newNum) {
    if (pq_size == 0) {
        array[0] = newNum;
        pq_size += 1;
    } else {
        array[pq_size] = newNum;
        pq_size += 1;

        for (int i = pq_size / 2 - 1; i >= 0; i--) {
            min_heapify(array, pq_size, i);
        }
    }
}

// Function to delete an element from the tree
void pq_delete(int array[], int num) {
    int i;

    for (i = 0; i < pq_size; i++) {
        if (num == array[i]) {
            break;
        }
    }

    swap(&array[i], &array[pq_size - 1]);
    pq_size -= 1;

    for (int i = pq_size / 2 - 1; i >= 0; i--) {
        min_heapify(array, pq_size, i);
    }
}
/* https://www.geeksforgeeks.org/optimal-file-merge-patterns/ */

/*
    Given n number of sorted files, the task is to find the minimum computations done to reach the Optimal Merge Pattern.
    When two or more sorted files are to be merged altogether to form a single file, the minimum computations are done to reach this file are known as Optimal Merge Pattern.

    If more than 2 files need to be merged then it can be done in pairs. For example, if need to merge 4 files A, B, C, D. First Merge A with B to get X1, merge X1 with C to get X2, merge X2 with D to get X3 as the output file.

    If we have two files of sizes m and n, the total computation time will be m+n. Here, we use the greedy strategy by merging the two smallest size files among all the files present.

    Examples:
        Given 3 files with sizes 2, 3, 4 units. Find an optimal way to combine these files

    Input: n = 3, size = {2, 3, 4}
    Output: 14

    Explanation: There are different ways to combine these files:
        Method 1: Optimal method
        2,3,4
        2 + 3 = 5
                5 + 4 = 9
        cost is 5 + 9 = 14

        Method 2:
        3 + 4 = 7
                7 + 2 = 9
        cost is 7 + 9 = 16

        Method 3:
        1 + 4 = 6
                6 + 3 = 9
        cost is 6 + 9 = 15

    Approach:

        Node represents a file with a given size also given nodes are greater than 2

        Add all the nodes in a priority queue (Min Heap).{node.weight = file size}
        Initialize count = 0 // variable to store file computations.
        Repeat while (size of priority Queue is greater than 1)
        create a new node
        new node = pq.poll().weight+pq.poll().weight;//pq denotes priority queue, remove 1st smallest and 2nd smallest element and add their weights to get a new node
        count += node.weight
        add this new node to priority queue;

        count is the final answer
*/

// Function to find minimum computation
int minComputation(int size, int files[])
{
    // Create a min heap
    int priority_queue[size+1];

    for (int i = 0; i < size; i++) {

        // Add sizes to priorityQueue
        pq_insert(priority_queue, files[i]);
    }

    // Variable to count total Computation
    int count = 0;

    while (pq_size > 1) {

        // pop two smallest size element
        // from the min heap
        int first_smallest = priority_queue[0];
        pq_delete(priority_queue, first_smallest);

        int second_smallest = priority_queue[0];
        pq_delete(priority_queue, second_smallest);

        int temp = first_smallest + second_smallest;

        // Add the current computations
        // with the previous one's
        count += temp;

        // Add new combined file size
        // to priority queue or min heap
        pq_insert(priority_queue, temp);
    }

    return count;
}

// Driver code
int main()
{
    // No of files
    int n = 6;

    // 6 files with their sizes
    int files[] = { 2, 3, 4, 5, 6, 7 };

    // Total no of computations
    // do be done final answer
    printf ("Minimum Computations = %d\r\n", minComputation(n, files));

    return 0;
}