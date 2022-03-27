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

/*
    Huffman coding is a lossless data compression algorithm.
    The idea is to assign variable-length codes to input characters,
    lengths of the assigned codes are based on the frequencies of corresponding characters.
    The most frequent character gets the smallest code and the least frequent character
    gets the largest code.

    The variable-length codes assigned to input characters are Prefix Codes, means
    the codes (bit sequences) are assigned in such a way that the code assigned to one
    character is not the prefix of code assigned to any other character.
    This is how Huffman Coding makes sure that there is no ambiguity when decoding the
    generated bitstream.

    Let us understand prefix codes with a counter example. Let there be four characters
    a, b, c and d, and their corresponding variable length codes be 00, 01, 0 and 1.
    This coding leads to ambiguity because code assigned to c is the prefix of codes
    assigned to a and b. If the compressed bit stream is 0001, the de-compressed output
    may be “cccd” or “ccb” or “acd” or “ab”.
    See this for applications of Huffman Coding.

    There are mainly two major parts in Huffman Coding
        Build a Huffman Tree from input characters.
        Traverse the Huffman Tree and assign codes to characters.

    Steps to build Huffman Tree
    Input is an array of unique characters along with their frequency of occurrences and output is Huffman Tree.

        1. Create a leaf node for each unique character and build a min heap of all leaf nodes
           (Min Heap is used as a priority queue. The value of frequency field is used to compare two nodes in min heap.
            Initially, the least frequent character is at root)
        2. Extract two nodes with the minimum frequency from the min heap.
        3. Create a new internal node with a frequency equal to the sum of the two nodes frequencies.
           Make the first extracted node as its left child and the other extracted node as its right child.
           Add this node to the min heap.
        4. Repeat steps#2 and #3 until the heap contains only one node.
           The remaining node is the root node and the tree is complete.
*/

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 100

// A Huffman tree node
typedef struct MinHeapNode {

    // One of the input characters
    char data;

    // Frequency of the character
    unsigned freq;

    // Left and right child of this node
    struct MinHeapNode *left, *right;
}HEAPNODE;

// A Min Heap:  Collection of
// min-heap (or Huffman tree) nodes
typedef struct MinHeap {

    // Current size of min heap
    unsigned size;

    // capacity of min heap
    unsigned capacity;

    // Array of minheap node pointers
    HEAPNODE **array;
} HEAP;

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
HEAPNODE *newNode(char data, unsigned freq)
{
    HEAPNODE *temp = (HEAPNODE *)malloc( sizeof(HEAPNODE));

    temp->left = NULL;
    temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

// A utility function to create
// a min heap of given capacity
HEAP *createMinHeap(unsigned capacity)
{
    HEAP *minHeap = (HEAP *)malloc(sizeof(HEAP));

    // current size is 0
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HEAPNODE **)malloc(minHeap->capacity * sizeof(HEAPNODE *));
    return minHeap;
}

// A utility function to
// swap two min heap nodes
void swapMinHeapNode(HEAPNODE **a, HEAPNODE **b)
{
    HEAPNODE *t = NULL;

    t = *a;
    *a = *b;
    *b = t;
}

// The standard minHeapify function.
void minHeapify(HEAP *minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size
            && minHeap->array[left]->freq < minHeap->array[smallest]->freq) {
        smallest = left;
    }

    if (right < minHeap->size
            && minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
        smallest = right;
    }

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check
// if size of heap is 1 or not
int isSizeOne(HEAP *minHeap) {

    return (minHeap->size == 1);
}

// A standard function to extract
// minimum value node from heap
HEAPNODE *extractMin(HEAP *minHeap)
{
    HEAPNODE *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    minHeap->size -= 1;

    minHeapify(minHeap, 0);
    return temp;
}

// A utility function to insert
// a new node to Min Heap
void insertMinHeap(HEAP *minHeap, HEAPNODE *minHeapNode)
{
    minHeap->size += 1;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(HEAP *minHeap)
{
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i) {
        minHeapify(minHeap, i);
    }
}

// Utility function to check if this node is leaf
int isLeaf(HEAPNODE *root)
{
    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
HEAP *createAndBuildMinHeap(char data[], int freq[], int size)
{
    HEAP *minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = newNode(data[i], freq[i]);
    }

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// The main function that builds Huffman tree
HEAPNODE *buildHuffmanTree(char data[], int freq[], int size)
{
    HEAPNODE *left, *right, *top;

    // Step 1: Create a min heap of capacity equal to size.
    // Initially, there are modes equal to size.
    HEAP *minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {

        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3:  Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not
        // used
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}

// A utility function to print an array of size n
void printArr(int arr[], int n) {
    int i;

    for (i = 0; i < n; ++i) {
        printf("%d", arr[i]);
    }

    printf("\n");
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(HEAPNODE *root, int arr[], int top)
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

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root)) {
        printf("%c: ", root->data);
        printArr(arr, top);
    }
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size){

    // Construct Huffman Tree
    HEAPNODE *root = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;

    printCodes(root, arr, top);
}

// Driver code
int main() {

    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = {  5,   9,  12,  13,  16,  45  };

    int size = sizeof(arr) / sizeof(arr[0]);

    HuffmanCodes(arr, freq, size);

    return 0;
}