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
    Flattening a Linked List
    https://takeuforward.org/data-structure/flattening-a-linked-list/
*/

#define STACK_MAX 1000
#define MAX_DISPLAY 100

typedef struct stack_ {
    ListNode* val[STACK_MAX];
    int top;
}STACK;

STACK * initilizeStack()
{
    STACK *s = calloc(1, sizeof(STACK));
    s->top = -1;
    return s;
}

bool isStackFull(STACK * s)
{
    if (s->top >= STACK_MAX) {
        return true;
    }
    return false;
}

bool isStackEmpty(STACK * s)
{
    if (s->top < 0 ) {
        return true;
    }
    return false;
}

ListNode* pop(STACK * s)
{
    if (isStackEmpty(s)) {
        return NULL;
    }
    return (s->val[(s->top)--]);
}

void push(STACK * s, ListNode* data)
{
    if (isStackFull(s)) {
        return;
    }
    s->val[++(s->top)] = data;
    return;
}

typedef struct ListNode {
    int val;
    struct ListNode *next;
    struct ListNode *bottom;
} ListNode;

ListNode * getNewNode(int value)
{
    ListNode *temp = malloc(sizeof(ListNode));
    temp->next = NULL;
    temp->bottom = NULL;
    temp->val = value;
    return temp;
}

ListNode * uint64num2LinkedList(u_int64_t n)
{
    ListNode *head = NULL;
    ListNode *curr = NULL;
    ListNode *temp = NULL;
    int remainder = 0;

    while (0 != n) {
        remainder = n % 100;
        temp = getNewNode(remainder);
        if (NULL == head) {
            head = temp;
            curr = temp;
        }

        curr->next = temp;
        curr = curr->next;
        n = n / 100;
    }

    return head;
}

ListNode * uint32num2LinkedList(u_int32_t n)
{
    ListNode *head = NULL;
    ListNode *curr = NULL;
    ListNode *temp = NULL;
    int remainder = 0;

    while (0 != n) {
        remainder = n % 10;
        temp = getNewNode(remainder);
        if (NULL == head) {
            head = temp;
            curr = temp;
        }

        curr->next = temp;
        curr = curr->next;
        n = n / 10;
    }

    return head;
}

void printLinkedList(ListNode *head)
{
    if (NULL == head) {
        return;
    }

    int count = MAX_DISPLAY;
    ListNode *temp = head;
    ListNode *itor = NULL;

    printf("\r\n The List values are : ");
    while (NULL != temp) {
        count--;
        if (0 == count) {
            break;
        }
        printf("\r\n \t [%p] %d ", temp, temp->val);
        if ((NULL != temp) && (NULL != temp->bottom)) {
            itor = temp->bottom;
            while (NULL != itor) {
                count--;
                if (0 == count) {
                    break;
                }
                printf("\t [%p] %d ", itor, itor->val);
                itor = itor->next;
            }
        }
        if (0 == count) {
            break;
        }
        temp = temp->next;
    }

    if (0 == count) {
        printf("Infinet Loop \r\n");
        return;
    }

    printf("\r\n");
    return;
}

void linkBottomList(ListNode *root, ListNode *child, int key)
{
    if (root == NULL || child == NULL) {
        return;
    }

    ListNode *temp = root;
    while (NULL != temp) {
        if (key == temp->val) {
            temp->bottom = child;
            break;
        }
        temp = temp->next;
    }

    return;
}

ListNode* merge(ListNode * a, ListNode * b)
{
    if ((a == NULL) && (b == NULL)) {
        return NULL;
    }

    if (a == NULL) {
        b->next = b->bottom;
        b->bottom = NULL;
        return b;
    }

    if (b == NULL) {
        a->next = a->bottom;
        a->bottom = NULL;
        return a;
    }

    a->next = a->bottom;
    a->bottom = NULL;

    ListNode *resultHead = NULL;
    ListNode *swap = NULL;
    ListNode* lastSorted = NULL;

    /* a point to a node with lowest value */
    if (a->val > b->val) {
        swap = a;
        a = b;
        b = swap;
    }

    resultHead = a;
    while(a != NULL && b != NULL) {

        lastSorted = NULL;
        while(a != NULL && a->val <= b->val) {
            lastSorted = a; //storing last sorted node
            a = a->next;
        }

        // link previous sorted node with next larger node in list2
        lastSorted->next = b;

        swap = a;
        a = b;
        b = swap;
    }

    return resultHead;
}

ListNode * mergeTwoLists(ListNode * a, ListNode * b) {

    ListNode *newBorn = NULL;
    ListNode *newBornTail = NULL;

    if ((NULL == a) && (NULL == b)) {
        return NULL;
    }

    newBorn = merge(a, b);

    newBornTail = newBorn;
    while ((NULL != newBornTail) && (NULL != newBornTail->next)) {
        newBornTail = newBornTail->next;
    }

    return newBorn;
}


ListNode *flatten(ListNode *root)
{
    ListNode * result;
    ListNode * itor;
    STACK *s = initilizeStack();

    if (root == NULL || root->next == NULL)
        return root;

    itor = root;
    while (itor) {
        push(s, itor);
        itor = itor->next;
    }

    itor = pop(s);
    result = NULL;;
    while (itor) {
        result = mergeTwoLists(itor, result);
        itor = pop(s);
    }

    return result;
}

int main ()
{
    ListNode *main = uint64num2LinkedList(50403010);
    ListNode *sub1 = uint64num2LinkedList(5958575655);
    ListNode *sub2 = uint64num2LinkedList(4948474645);
    ListNode *sub3 = uint64num2LinkedList(3938373635);

    linkBottomList(main, sub1, 50);
    linkBottomList(main, sub2, 40);
    linkBottomList(main, sub3, 30);

    printLinkedList(main);

    printf("\r\n\r\n The Final Result : \r\n");
    ListNode *newHead = flatten(main);
    printLinkedList(newHead);

    return;
}