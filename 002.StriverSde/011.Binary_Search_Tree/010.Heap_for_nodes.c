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
    The median is the middle value in an ordered integer list.
    If the size of the list is even, there is no middle value and the median is the mean
    of the two middle values.

    For example, for arr = [2,3,4], the median is 3.
    For example, for arr = [2,3], the median is (2 + 3) / 2 = 2.5.
    Implement the MedianFinder class:

    MedianFinder() initializes the MedianFinder object.
    void addNum(int num) adds the integer num from the data stream to the data structure.
    double findMedian() returns the median of all elements so far. Answers within 10-5 of
    the actual answer will be accepted.
*/

/*
 Array Implementation of MinHeap data Structure
*/

#define HEAP_SIZE 100
#define HEAP_TYPE_MIN 0
#define HEAP_TYPE_MAX 1

typedef struct node {
    int data;
    struct node * left;
    struct node * right;
}NODE;

typedef struct Heap {
    NODE * arr[HEAP_SIZE];
    int count;
    int capacity;
    int heap_type;
}HEAP;


NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

HEAP * initHeap(int heap_type)
{
    HEAP *h = (HEAP *) malloc(sizeof(HEAP));
    if(h == NULL){
        printf("Memory Error!");
        return NULL;
    }

    h->heap_type = heap_type;
    h->count = 0;
    h->capacity = HEAP_SIZE;

    return h;
}

void print(HEAP *h)
{
    printf("Print Heap : ");
    for(int i = 0; i < h->count; i++) {
        printf("-> %d ", (h->arr[i])->data);
    }
    printf("\r\n");
}

void heapify_bottom_top(HEAP *h, int index)
{
    NODE *temp = NULL;
    int parent_node = (index - 1) / 2;

    if (h->heap_type == HEAP_TYPE_MIN) {
        if((h->arr[parent_node])->data > (h->arr[index])->data) {

            //swap and recursive call
            temp = h->arr[parent_node];
            h->arr[parent_node] = h->arr[index];
            h->arr[index] = temp;

            heapify_bottom_top(h, parent_node);
        }
    } else {
        if((h->arr[parent_node])->data < (h->arr[index])->data) {

            //swap and recursive call
            temp = h->arr[parent_node];
            h->arr[parent_node] = h->arr[index];
            h->arr[index] = temp;

            heapify_bottom_top(h, parent_node);
        }
    }
}

void insert(HEAP *h, NODE *key)
{
    printf("Insert Heap : %d\n", key->data);

    if( h->count < h->capacity){

        h->arr[h->count] = key;
        heapify_bottom_top(h, h->count);
        h->count++;
    }

    print(h);
}

void heapify_top_bottom(HEAP *h, int parent_node)
{
    int swap = 0;
    NODE * temp = NULL;

    int left = parent_node * 2 + 1;
    int right = parent_node * 2 + 2;

    if ((left >= h->count) || (left < 0)) {
        left = -1;
    }

    if ((right >= h->count) || (right < 0)) {
        right = -1;
    }

    if (h->heap_type == HEAP_TYPE_MIN) {
        if ( (left != -1) && ((h->arr[left])->data < (h->arr[parent_node])->data) ) {
            swap = left;
        } else {
            swap = parent_node;
        }

        if ( (right != -1) && ((h->arr[right])->data < (h->arr[swap])->data) ) {
            swap = right;
        }
    } else {
        if ( (left != -1) && ((h->arr[left])->data > (h->arr[parent_node])->data) ) {
            swap = left;
        } else {
            swap = parent_node;
        }

        if ( (right != -1) && ((h->arr[right])->data > (h->arr[swap])->data) ) {
            swap = right;
        }
    }

    if (swap != parent_node) {
        temp = h->arr[swap];
        h->arr[swap] = h->arr[parent_node];
        h->arr[parent_node] = temp;

        // recursive  call
        heapify_top_bottom(h, swap);
    }

    return;
}

NODE * PopMin(HEAP *h)
{
    NODE * pop;
    if(h->count == 0){
        return NULL;
    }

    // replace first node by last and delete last
    pop = h->arr[0];
    h->arr[0] = h->arr[h->count - 1];
    h->count--;

    heapify_top_bottom(h, 0);

    return pop;
}

int main()
{
    HEAP *heap = initHeap(HEAP_TYPE_MIN);
    if( heap == NULL ){
        return -1;
    }

    NODE *tmp;

    tmp = newNode(20);
    insert(heap, tmp);
    NODE *root = tmp;

    tmp = newNode(15);
    insert(heap, tmp);
    root->left = tmp;

    tmp = newNode(14);
    insert(heap, tmp);
    root->left->left = tmp;

    tmp = newNode(17);
    insert(heap, tmp);
    root->left->left->right = tmp;

    tmp = newNode(18);
    insert(heap, tmp);
    root->left->right = tmp;

    tmp = newNode(16);
    insert(heap, tmp);
    root->left->right->left = tmp;

    tmp = newNode(19);
    insert(heap, tmp);
    root->left->right->right = tmp;

    tmp = newNode(40);
    insert(heap, tmp);
    root->right = tmp;

    tmp = newNode(30);
    insert(heap, tmp);
    root->right->left = tmp;

    tmp = newNode(60);
    insert(heap, tmp);
    root->right->right = tmp;

    tmp = newNode(50);
    insert(heap, tmp);
    root->right->right->left = tmp;

    printf("\r\n");
    print(heap);
    printf("\r\n");

    for(int i = 9; i >= 0; i--) {
        tmp = PopMin(heap);
        printf("Pop Minima : %d\n", tmp->data);
        print(heap);
    }

    return 0;
}