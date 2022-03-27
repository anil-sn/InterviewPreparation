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
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

struct node {
    int data;
    struct node* next;
};

struct node* head = NULL;
struct node* sorted = NULL;

void push(int val)
{
    /* allocate node */
    struct node* newnode
        = (struct node*)malloc(sizeof(struct node));
    newnode->data = val;
    /* link the old list off the new node */
    newnode->next = head;
    /* move the head to point to the new node */
    head = newnode;
}

/*
 * function to insert a new_node in a list. Note that
 * this function expects a pointer to head_ref as this
 * can modify the head of the input linked list
 * (similar to push())
 */
void sortedInsert(struct node* newnode)
{
    /* Special case for the head end */
    if (sorted == NULL || sorted->data >= newnode->data) {
        newnode->next = sorted;
        sorted = newnode;
    }
    else {
        struct node* current = sorted;
        /* Locate the node before the point of insertion
         */
        while (current->next != NULL
               && current->next->data < newnode->data) {
            current = current->next;
        }
        newnode->next = current->next;
        current->next = newnode;
    }
}

// function to sort a singly linked list
// using insertion sort
void insertionsort()
{

    struct node* current = head;

    // Traverse the given linked list and insert every
    // node to sorted
    while (current != NULL) {

        // Store next for next iteration
        struct node* next = current->next;

        // insert current in sorted linked list
        sortedInsert(current);

        // Update current
        current = next;
    }
    // Update head to point to sorted linked list
    head = sorted;
}

/* Function to print linked list */
void printlist(struct node* head)
{
    while (head != NULL) {
        printf("%d->", head->data);
        head = head->next;
    }
    printf("NULL");
}

// Driver program to test above functions
int main()
{

    push(5);
    push(20);
    push(4);
    push(3);
    push(30);

    printf("Linked List before sorting:\n");
    printlist(head);
    printf("\n");

    insertionsort(head);

    printf("Linked List after sorting:\n");
    printlist(head);
}