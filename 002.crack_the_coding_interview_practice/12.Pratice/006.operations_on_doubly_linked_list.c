
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
    Dont practice until you get it right
    practice until you can't get it worng
*/

/*
    C program for the all operations in the Doubly Linked List
    Operations to be performed:

    traverse(): To see the contents of the linked list, it is necessary to traverse the given doubly linked list. The given traverse() function traverses and prints the content of the doubly linked list.
    insertAtFront(): This function simply inserts an element at the front/beginning of the doubly linked list.
    insertAtEnd(): This function inserts an element at the end of the doubly linked list.
    insertAtPosition(): This function inserts an element at a specified position in the doubly linked list.
    deleteFirst(): This function simply deletes an element from the front/beginning of the doubly linked list.
    deleteEnd(): This function simply deletes an element from the end of the doubly linked list.
    deletePosition(): This function deletes an element from a specified position in the doubly linked list.
    Below is the implementation of the above operations:
*/

// Linked List Node
struct node {
    int info;
    struct node *prev, *next;
};

struct node* start = NULL;

// Function to traverse the linked list
void traverse() {
    int index = 1;

    // List is empty
    if (start == NULL) {
        printf("\nList is empty\n");
        return;
    }
    printf("\n");
    // Else print the Data
    struct node* temp;
    temp = start;
    while (temp != NULL) {
        if (temp->next) {
            printf("[%d]%d <-> ", index, temp->info);
        } else {
            printf("[%d]%d -> NULL", index, temp->info);
        }
        temp = temp->next;
        index++;
    }
    printf("\n\n");
}

// Function to insert at the front
// of the linked list
void insertAtFront() {
    int data;
    struct node* temp;
    temp = (struct node*)malloc(sizeof(struct node));
    printf("\nEnter number to be inserted: ");
    scanf("%d", &data);
    temp->info = data;
    temp->prev = NULL;

    // Pointer of temp will be
    // assigned to start
    temp->next = start;
    start = temp;
}

// Function to insert at the end of
// the linked list
void insertAtEnd() {
    int data;
    struct node *temp, *trav;

    printf("\nEnter number to be inserted: ");
    scanf("%d", &data);

    temp = (struct node*)malloc(sizeof(struct node));
    temp->prev = NULL;
    temp->next = NULL;

    temp->info = data;
    temp->next = NULL;

    trav = start;

    // If start is NULL
    if (start == NULL) {
        start = temp;
    }

    // Changes Links
    else {
        while (trav->next != NULL) {
            trav = trav->next;
        }
        temp->prev = trav;
        trav->next = temp;
    }
}

// Function to insert at any specified
// position in the linked list
void insertAtPosition() {
    int data, pos, i = 1;
    struct node *temp, *newnode;

    // Enter the position and data
    printf("\nEnter position : ");
    scanf("%d", &pos);

    newnode = malloc(sizeof(struct node));
    newnode->next = NULL;
    newnode->prev = NULL;

    // If start==NULL,
    if (start == NULL) {
        start = newnode;
        newnode->prev = NULL;
        newnode->next = NULL;
    }

    // If position==1,
    else if (pos == 1) {
        insertAtFront();
    }

    // Change links
    else {
        printf("\nEnter number to be inserted: ");
        scanf("%d", &data);
        newnode->info = data;
        temp = start;
        while (i < pos - 1) {
            temp = temp->next;
            i++;
        }
        newnode->next = temp->next;
        newnode->prev = temp;
        temp->next = newnode;
        temp->next->prev = newnode;
    }
}

// Function to delete from the front
// of the linked list
void deleteFirst() {
    struct node* temp;
    if (start == NULL)
        printf("\nList is empty\n");
    else {
        temp = start;
        start = start->next;
        if (start != NULL)
            start->prev = NULL;
        free(temp);
    }
}

// Function to delete from the end
// of the linked list
void deleteEnd() {
    struct node* temp;
    if (start == NULL)
        printf("\nList is empty\n");
    temp = start;
    while (temp->next != NULL)
        temp = temp->next;
    if (start->next == NULL)
        start = NULL;
    else {
        temp->prev->next = NULL;
        free(temp);
    }
}

// Function to delete from any specified
// position from the linked list
void deletePosition() {
    int pos, i = 1;
    struct node *temp, *position;
    temp = start;

    // If DLL is empty
    if (start == NULL)
        printf("\nList is empty\n");

    // Otherwise
    else {
        // Position to be deleted
        printf("\nEnter position : ");
        scanf("%d", &pos);

        // If the position is the first node
        if (pos == 1) {
            deleteFirst(); // im,proved by Jay Ghughriwala on GeeksforGeeks
            if (start != NULL) {
                start->prev = NULL;
            }
            free(position);
            return;
        }

        // Traverse till position
        while (i < pos - 1) {
            temp = temp->next;
            i++;
        }
        // Change Links
        position = temp->next;
        if (position->next != NULL)
            position->next->prev = temp;
        temp->next = position->next;

        // Free memory
        free(position);
    }
}

// Driver Code
int main() {
    int choice;
    while (1) {

        printf("Press 1 To see list\n");
        printf("Press 2 For insertion at starting\n");
        printf("Press 3 For insertion at end\n");
        printf("Press 4 For insertion at any position\n");
        printf("Press 5 For deletion of first element\n");
        printf("Press 6 For deletion of last element\n");
        printf("Press 7 For deletion of element at any position\n");
        printf("Press 8 To exit\n");
        printf("Enter Choice :\n\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            traverse();
            break;
        case 2:
            insertAtFront();
            break;
        case 3:
            insertAtEnd();
            break;
        case 4:
            insertAtPosition();
            break;
        case 5:
            deleteFirst();
            break;
        case 6:
            deleteEnd();
            break;
        case 7:
            deletePosition();
            break;

        case 8:
            exit(1);
            break;
        default:
            printf("Incorrect Choice. Try Again \n");
            continue;
        }
    }
    return 0;
}