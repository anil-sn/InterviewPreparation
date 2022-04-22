#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX 100
#define HEAP_SIZE 100
#define HEAP_TYPE_MIN 0
#define HEAP_TYPE_MAX 1

typedef struct graph_{
    int graph[MAX][MAX];

    bool visited[MAX];
    bool dfsVisitPath[MAX];

    unsigned int distance[MAX];

    int numOfNodes;
} GRAPH;

typedef struct node_ {
    int src;
    int dst;
    int cost;
} NODE;

NODE * newNode(int src, int dst, int cost)
{
    NODE *n = malloc(sizeof(NODE));
    n->src = src;
    n->dst = dst;
    n->cost = cost;
}

typedef struct Heap {
    NODE * arr[HEAP_SIZE];
    int count;
    int capacity;
    int heap_type;
}HEAP;

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
    printf("Print Heap  : ");
    for(int i = 0; i < h->count; i++) {
        printf("-> %d ", (h->arr[i])->cost);
    }
    printf("\r\n");
}

void heapify_bottom_top(HEAP *h, int index)
{
    NODE *temp = NULL;
    int parent_node = (index - 1) / 2;

    if (h->heap_type == HEAP_TYPE_MIN) {
        if((h->arr[parent_node])->cost > (h->arr[index])->cost) {

            //swap and recursive call
            temp = h->arr[parent_node];
            h->arr[parent_node] = h->arr[index];
            h->arr[index] = temp;

            heapify_bottom_top(h, parent_node);
        }
    } else {
        if((h->arr[parent_node])->cost < (h->arr[index])->cost) {

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
    printf("Insert Heap : Node:%d, Cost:%d\n", key->dst, key->cost);

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
        if ( (left != -1) && ((h->arr[left])->cost < (h->arr[parent_node])->cost) ) {
            swap = left;
        } else {
            swap = parent_node;
        }

        if ( (right != -1) && ((h->arr[right])->cost < (h->arr[swap])->cost) ) {
            swap = right;
        }
    } else {
        if ( (left != -1) && ((h->arr[left])->cost > (h->arr[parent_node])->cost) ) {
            swap = left;
        } else {
            swap = parent_node;
        }

        if ( (right != -1) && ((h->arr[right])->cost > (h->arr[swap])->cost) ) {
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

bool isHeapEmpty(HEAP *h)
{
    return (h->count == 0);
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

    printf("Pop Heap    : Node:%d, Cost:%d\n", pop->dst, pop->cost);
    return pop;
}

void initVisited(GRAPH *g)
{
    memset(g->visited, false, sizeof(g->visited));
    memset(g->dfsVisitPath, false, sizeof(g->dfsVisitPath));
    memset(g->distance, INT_MAX, sizeof(g->distance));

    return;
}

// function to initialise the adjacency matrix
GRAPH * initGrapth(int num)
{
    GRAPH *g = malloc(sizeof(GRAPH));
    memset(g, 0, sizeof(GRAPH));
    memset(g->graph, -1, sizeof(g->graph));

    g->numOfNodes = num;
    initVisited(g);

    return g;
}


// Function to add edge between nodes
void addEdge(GRAPH *g, int a, int b, int weight)
{
    g->graph[a][b] = weight;
}

// Function to add edge between nodes
void removeEdge(GRAPH *g, int a, int b)
{
    g->graph[a][b] = -1;
}

int main()
{
    GRAPH *g = initGrapth(11);
    addEdge(g, 0, 1,  1);
    addEdge(g, 0, 2,  1);
    addEdge(g, 0, 10, 1);
    addEdge(g, 1, 9,  1);
    addEdge(g, 1, 2,  1);
    addEdge(g, 2, 3,  1);
    addEdge(g, 3, 0,  1);
    addEdge(g, 4, 0,  1);
    addEdge(g, 4, 3,  1);
    addEdge(g, 4, 5,  1);
    addEdge(g, 5, 6,  1);
    addEdge(g, 6, 4,  1);
    addEdge(g, 6, 7,  1);
    addEdge(g, 7, 10, 1);
    addEdge(g, 7, 8,  1);
    addEdge(g, 8, 9,  1);
    addEdge(g, 9, 10, 1);
    addEdge(g, 10, 8, 1);

    HEAP *heap = initHeap(HEAP_TYPE_MIN);
    if( heap == NULL ){
        return -1;
    }

    int src = 6;
    g->distance[src] = 0;
    insert(heap, newNode(src, src, 0));

    while (false == isHeapEmpty(heap)) {

        NODE *min =  PopMin(heap);
        for (int i = 0; i < g->numOfNodes; i++) {

            if (-1 == g->graph[min->dst][i]) {
                continue;
            }

            if (min->dst == i){
                continue;
            }

            if( g->distance[i] > g->distance[min->dst] + g->graph[min->dst][i]) {

                g->distance[i] = g->distance[min->dst] + g->graph[min->dst][i];
                insert(heap, newNode(min->dst, i, g->distance[i]));
            }
        }
    }

    printf("Shortest Path : \r\n");
    for (int i = 0; i < g->numOfNodes; i++) {
        printf("[%d]%2d, ", i, g->distance[i]);
    }
    printf("\r\n");

    return 0;
}