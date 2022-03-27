#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

// Swap two integers
void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}
/* a node of the doubly linked list */
typedef struct Node
{
    int data;
    struct Node *next;
    struct Node *prev;
}NODE;

// A utility function to find last node of linked list
NODE *lastNode(NODE *root)
{
    while (root && root->next)
        root = root->next;
    return root;
}

/* Considers last element as pivot, places the
pivot element at its correct position in sorted array,
and places all smaller (smaller than pivot) to left
of pivot and all greater elements to right of pivot */
NODE* partition(NODE *l, NODE *h)
{
    // set pivot as h element
    int x = h->data;

    // similar to i = l-1 for array implementation
    NODE *i = l->prev;

    // Similar to "for (int j = l; j <= h- 1; j++)"
    for (NODE *j = l; j != h; j = j->next)
    {
        if (j->data <= x)
        {
            // Similar to i++ for array
            i = (i == NULL) ? l : i->next;

            swap(&(i->data), &(j->data));
        }
    }
    i = (i == NULL) ? l : i->next; // Similar to i++
    swap(&(i->data), &(h->data));
    return i;
}

/* A recursive implementation of quicksort for linked list */
void _quickSort(NODE* l, NODE *h)
{
    if (h != NULL && l != h && l != h->next)
    {
        NODE *p = partition(l, h);
        _quickSort(l, p->prev);
        _quickSort(p->next, h);
    }
}

// The main function to sort a linked list.
// It mainly calls _quickSort()
void quickSort(NODE *head)
{
    // Find last node
    NODE *h = lastNode(head);

    // Call the recursive QuickSort
    _quickSort(head, h);
}

// A utility function to print contents of arr
void printList(NODE *head)
{
    while (head)
    {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

/* Function to insert a node at the
beginning of the Doubly Linked List */
void push(NODE** head_ref, int new_data)
{
    NODE* new_node = (NODE*)
               malloc(sizeof(NODE)); /* allocate node */
    new_node->data = new_data;

    /* since we are adding at the beginning,
    prev is always NULL */
    new_node->prev = NULL;

    /* link the old list off the new node */
    new_node->next = (*head_ref);

    /* change prev of head node to new node */
    if ((*head_ref) != NULL) (*head_ref)->prev = new_node ;

    /* move the head to point to the new node */
    (*head_ref) = new_node;
}

// Driver Code
int main(int argc, char **argv)
{
    NODE *a = NULL;
    push(&a, 5);
    push(&a, 20);
    push(&a, 4);
    push(&a, 3);
    push(&a, 30);

    printf("Linked List before sorting \n");
    printList(a);

    quickSort(a);

    printf("Linked List after sorting \n");
    printList(a);

    return 0;
}