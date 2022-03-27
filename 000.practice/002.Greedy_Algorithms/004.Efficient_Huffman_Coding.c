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

// This constant can be avoided by explicitly calculating
// height of Huffman Tree
#define MAX_TREE_HT 100

// A node of huffman tree
typedef struct QueueNode {
    char data;
    unsigned freq;
    struct QueueNode *left;
    struct QueueNode *right;
}QUEUENODE;

// Structure for Queue: collection of Huffman Tree nodes (or
// QueueNodes)
typedef struct Queue {
    int front, rear;
    int capacity;
    QUEUENODE** array;
}QUEUE;

// A utility function to create a new Queuenode
QUEUENODE* newNode(char data, unsigned freq)
{
    QUEUENODE* temp = (QUEUENODE*)malloc( sizeof(QUEUENODE));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// A utility function to create a Queue of given capacity
QUEUE* createQueue(int capacity)
{
    QUEUE* queue = (QUEUE*)malloc(sizeof(QUEUE));
    queue->front = queue->rear = -1;
    queue->capacity = capacity;
    queue->array = (QUEUENODE**)malloc(queue->capacity * sizeof(QUEUENODE*));
    return queue;
}

// A utility function to check if size of given queue is 1
int isSizeOne(QUEUE* queue)
{
    return queue->front == queue->rear && queue->front != -1;
}

// A utility function to check if given queue is empty
int isEmpty(QUEUE* queue)
{
    return queue->front == -1;
}

// A utility function to check if given queue is full
int isFull(QUEUE* queue)
{
    return queue->rear == queue->capacity - 1;
}

// A utility function to add an item to queue
void enQueue(QUEUE* queue, QUEUENODE* item)
{
    if (isFull(queue))
        return;
    queue->array[++queue->rear] = item;
    if (queue->front == -1){
        ++queue->front;
    }
}

// A utility function to remove an item from queue
QUEUENODE* deQueue(QUEUE* queue)
{
    if (isEmpty(queue))
        return NULL;
    QUEUENODE* temp = queue->array[queue->front];
    if (queue->front== queue->rear) // If there is only one item in queue
        queue->front = queue->rear = -1;
    else
        ++queue->front;
    return temp;
}

// A utility function to get from of queue
QUEUENODE* getFront(QUEUE* queue)
{
    if (isEmpty(queue))
        return NULL;
    return queue->array[queue->front];
}

/* A function to get minimum item from two queues */
QUEUENODE* findMin(QUEUE* firstQueue, QUEUE* secondQueue)
{
    // Step 3.a: If first queue is empty, dequeue from
    // second queue
    if (isEmpty(firstQueue))
        return deQueue(secondQueue);

    // Step 3.b: If second queue is empty, dequeue from
    // first queue
    if (isEmpty(secondQueue))
        return deQueue(firstQueue);

    // Step 3.c:  Else, compare the front of two queues and
    // dequeue minimum
    if (getFront(firstQueue)->freq < getFront(secondQueue)->freq)
        return deQueue(firstQueue);

    return deQueue(secondQueue);
}

// Utility function to check if this node is leaf
int isLeaf(QUEUENODE* root)
{
    return !(root->left) && !(root->right);
}

// A utility function to print an array of size n
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

// The main function that builds Huffman tree
QUEUENODE* buildHuffmanTree(char data[], int freq[], int size)
{
    QUEUENODE *left, *right, *top;

    // Step 1: Create two empty queues
    QUEUE* firstQueue = createQueue(size);
    QUEUE* secondQueue = createQueue(size);

    // Step 2:Create a leaf node for each unique character
    // and Enqueue it to the first queue in non-decreasing
    // order of frequency. Initially second queue is empty
    for (int i = 0; i < size; ++i)
        enQueue(firstQueue, newNode(data[i], freq[i]));

    // Run while Queues contain more than one node. Finally,
    // first queue will be empty and second queue will
    // contain only one node
    while (!(isEmpty(firstQueue) && isSizeOne(secondQueue))) {

        // Step 3: Dequeue two nodes with the minimum
        // frequency by examining the front of both queues
        left = findMin(firstQueue, secondQueue);
        right = findMin(firstQueue, secondQueue);

        // Step 4: Create a new internal node with frequency
        // equal to the sum of the two nodes frequencies.
        // Enqueue this node to second queue.
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        enQueue(secondQueue, top);
    }

    return deQueue(secondQueue);
}

// Prints huffman codes from the root of Huffman Tree.  It
// uses arr[] to store codes
void printCodes(QUEUENODE* root, int arr[], int top)
{
    // Assign 0 to left edge and recur
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then it contains one of the
    // input characters, print the character and its code
    // from arr[]
    if (isLeaf(root)) {
        printf("%c: ", root->data);
        printArr(arr, top);
    }
}

// The main function that builds a Huffman Tree and print
// codes by traversing the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size)
{
    //  Construct Huffman Tree
    QUEUENODE* root = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using the Huffman tree built
    // above
    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
}

// Driver program to test above functions
int main()
{
    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = { 5, 9, 12, 13, 16, 45 };
    int size = sizeof(arr) / sizeof(arr[0]);
    HuffmanCodes(arr, freq, size);
    return 0;
}