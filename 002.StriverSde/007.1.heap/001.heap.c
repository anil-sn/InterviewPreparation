// Max-Heap data structure in C
#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX 100000

int capacity;
int *mH;
int currentSize;

bool isEmpty() {
    return currentSize == 0;
}

int heapSize(){
    return currentSize;
}

void minHeapInit(int cap)
{
    capacity = cap;
    mH = calloc(capacity, sizeof(int));
    currentSize =0;
}

void swap(int a, int b) {
    int temp = mH[a];
    mH[a] = mH[b];
    mH[b] = temp;
}

void bubbleUp(int pos) {
    int parentIdx = pos/2;
    int currentIdx = pos;
    while (currentIdx > 0 && mH[parentIdx] > mH[currentIdx]) 
    {

        swap(currentIdx, parentIdx);
        currentIdx = parentIdx;
        parentIdx = parentIdx/2;
    }
}

void insert(int x) {
    if(currentSize == capacity) {
        printf("heap is full \r\n");
        return;
    }

    currentSize++;
    int idx = currentSize;
    mH[idx] = x;
    bubbleUp(idx);
}

void createHeap(int n, int arr[n])
{
    if(n > 0){
        for(int i=0; i<n; i++)
        {
            insert(arr[i]);
        }
    }
}

void display() 
{
    for(int i=1; i<currentSize; i++) {
        printf(" %d",mH[i]);
    }

    printf("\r\n");
}

void sinkDown(int k) {
    int smallest = k;
    int leftChildIdx = 2 * k;
    int rightChildIdx = 2 * k+1;
    if (leftChildIdx < heapSize() && mH[smallest] > mH[leftChildIdx]) {
        smallest = leftChildIdx;
    }
    if (rightChildIdx < heapSize() && mH[smallest] > mH[rightChildIdx]) {
        smallest = rightChildIdx;
    }
    if (smallest != k) {

        swap(k, smallest);
        sinkDown(smallest);
    }
}

int extractMin() {
    int min = mH[1];
    mH[1] = mH[currentSize];
    mH[currentSize] = 0;
    sinkDown(1);
    currentSize--;
    return min;
}




int main(void)
{
    int arrA[] = {3,2,1,7,8,4,10,16,12};
    int len = sizeof(arrA)/sizeof(arrA[0]);

    printf("\r\nOriginal Array : ");
    for(int i=0; i<len; i++){
        printf("  %d", arrA[i]);
    }

    minHeapInit(MAX);

    printf("\r\nMin-Heap : ");
    createHeap(len, arrA);
    display();

    printf("\r\nExtract Min :");
    for(int i=0;i<len;i++){
        printf("  %d",extractMin());
    }
    printf("\r\n");
}