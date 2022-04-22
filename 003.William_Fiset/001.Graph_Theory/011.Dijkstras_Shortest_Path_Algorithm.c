#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAXIMUM 100
#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 0xFFFF
#define HEAP_TYPE_MIN 0
#define HEAP_TYPE_MAX 1

typedef struct node_ {
    int src;
    int dst;
    int cost;
} NODE;

typedef struct MinHeapNode_ {
    int dst;
    int cost;
} HEAPNODE;

typedef struct MinHeap_ {
    int size;
    int capacity;
    int pos[MAXIMUM];
    HEAPNODE *array[MAXIMUM];
} HEAP;

typedef struct Edge {
    int from;
    int to;
    int weight;
    struct Edge *prev;
    struct Edge *next;
} EDGE;

typedef struct graph{
    EDGE *nodes[MAXIMUM];
    bool visited[MAXIMUM];
    int  parent[MAXIMUM];
    int  distance[MAXIMUM];
    int  numOfNodes;
} GRAPH;

HEAPNODE* newHeapNode(int dst, int cost)
{
    HEAPNODE* node = (HEAPNODE*) malloc(sizeof(HEAPNODE));
    node->dst = dst;
    node->cost = cost;

    return node;
}

HEAP* newMinHeap(GRAPH *g)
{
    HEAP* minHeap = (HEAP*) calloc(1, sizeof(HEAP));

    minHeap->size = 0;
    minHeap->capacity = MAXIMUM;

    // Initialize min heap with all vertices. dist value of all vertices
    for (int v = 0; v < g->numOfNodes; ++v)
    {
        g->distance[v] = INT_MAX;
        minHeap->array[v] = newHeapNode(v, g->distance[v]);
        minHeap->pos[v] = v;
    }

    minHeap->size = g->numOfNodes;

    return minHeap;
}

void swapMinHeapNode(HEAPNODE** a, HEAPNODE** b)
{
    HEAPNODE* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(HEAP* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->cost < minHeap->array[smallest]->cost) {
        smallest = left;
    }

    if (right < minHeap->size && minHeap->array[right]->cost < minHeap->array[smallest]->cost) {
        smallest = right;
    }

    if (smallest != idx) {

        // The nodes to be swapped in min heap
        HEAPNODE *smallestNode = minHeap->array[smallest];
        HEAPNODE *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->dst] = idx;
        minHeap->pos[idxNode->dst] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(HEAP* minHeap)
{
    return minHeap->size == 0;
}

HEAPNODE* extractMin(HEAP* minHeap)
{
    if (isEmpty(minHeap)) {
        return NULL;
    }

    HEAPNODE* root = minHeap->array[0];

    // Replace root node with last node
    HEAPNODE* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->dst] = minHeap->size-1;
    minHeap->pos[lastNode->dst] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(HEAP* minHeap, int dst, int cost)
{
    // Get the index of dst in  heap array
    int i = minHeap->pos[dst];

    // Get the node and update its cost value
    minHeap->array[i]->cost = cost;

    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->cost < minHeap->array[(i - 1) / 2]->cost)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->dst] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->dst] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

void insertMinHeap(HEAP* minHeap, int src, int dist)
{
    minHeap->array[src] = newHeapNode(src, dist);
    minHeap->pos[src] = src;
    decreaseKey(minHeap, src, dist);

    return;
}

// A utility function to check if a given vertex 'dst' is in min heap or not
bool isInMinHeap(HEAP *minHeap, int dst)
{
    if (minHeap->pos[dst] < minHeap->size) {
        return true;
    }

    return false;
}

void refreshGraph(GRAPH * graph)
{
    memset(graph->visited, false, sizeof(graph->visited));
    memset(graph->parent, 0, sizeof(graph->parent));
    memset(graph->distance, INF, sizeof(graph->distance));

    return;
}

GRAPH * initGraph(int numOfNodes)
{
    GRAPH *g = calloc(1, sizeof(GRAPH));
    g->numOfNodes = numOfNodes;
    refreshGraph(g);
    return g;
}

void updEdge(EDGE *e, int u, int v, int w)
{
    e->from = u;
    e->to = v;
    e->weight = w;
}

EDGE* newEdge(int u, int v, int weight)
{
    EDGE *new = calloc(1, sizeof(EDGE));

    updEdge(new, u, v, weight);
    new->prev = NULL;
    new->next = NULL;

    return new;
}

void addEdgeToGraph(GRAPH *g, int u, int v, int uWeight, int vWeight, bool isUnDirected)
{
    EDGE* uhead = g->nodes[u];
    EDGE* vhead = g->nodes[v];
    EDGE* uAdj = NULL;
    EDGE* vAdj = NULL;

    if (false == isUnDirected) {
        if (NULL == uhead) {
            g->nodes[u] = newEdge(u, v, uWeight);
        } else {
            uAdj = newEdge(u, v, uWeight);
            uAdj->prev = uhead;
            uAdj->next = uhead->next;
            uhead->next = uAdj;
        }
        return;
    } else {

        if (NULL == uhead) {
            g->nodes[u] = newEdge(u, v, uWeight);
        } else {
            uAdj = newEdge(u, v, uWeight);
            uAdj->prev = uhead;
            uAdj->next = uhead->next;
            uhead->next = uAdj;
        }

        if (NULL == vhead) {
            g->nodes[v] = newEdge(v, u, vWeight);
        } else {
            vAdj = newEdge(v, u, vWeight);
            vAdj->prev = vhead;
            vAdj->next = vhead->next;
            vhead->next = vAdj;
        }
        return;
    }

    uAdj = newEdge(u, v, uWeight);
    uAdj->prev = uhead;
    uAdj->next = uhead->next;
    uhead->next = uAdj;

    if (true == isUnDirected) {
        vAdj = newEdge(v, u, vWeight);
        vAdj->prev = vhead;
        vAdj->next = vhead->next;
        vhead->next = vAdj;
    }

    return;
}

void printEdgeList(int node, EDGE *head)
{
    printf("[%3d]:->\t", node);

    if (NULL == head){
        printf("  NIL");
    }

    while (NULL != head){
        printf("%3d->%3d [%3d]\t", head->from, head->to, head->weight);
        head = head->next;
    }

    printf("\r\n");
}

void printGraph(GRAPH *g)
{
    printf("Graph : \r\n");
    for (int i = 0; i < g->numOfNodes; i++){
        printEdgeList(i, g->nodes[i]);
    }
    printf("\r\n");
}

void dijkstra(GRAPH * graph, int start)
{

    HEAP *pq = newMinHeap(graph);

    graph->distance[start] = 0;
    insertMinHeap(pq, start, 0);

    while (false == isEmpty(pq)) {

        HEAPNODE *node = extractMin(pq);
        graph->visited[node->dst] = true;

        // We already found a better path before we got to processing this node so we can ignore it.
        if (graph->distance[node->dst] < node->cost){
            continue;
        }

        EDGE *edge = graph->nodes[node->dst];
        while (NULL != edge) {

            if (false == graph->visited[edge->to]) {

                int newDist = graph->distance[edge->from] + edge->weight;
                if (newDist < graph->distance[edge->to]) {

                    graph->parent[edge->to] = edge->from;
                    graph->distance[edge->to] = newDist;

                    insertMinHeap(pq, edge->to, newDist);
                }
            }

            edge = edge->next;
        }
    }

    return;
}

// Example usage of topological sort
int main(void) {

    // Graph setup
    int N = 9;
    GRAPH * graph = initGraph(N);

    addEdgeToGraph(graph, 0, 1,  4 , 4, true);
    addEdgeToGraph(graph, 0, 7,  8 , 8, true);
    addEdgeToGraph(graph, 1, 7, 11, 11, true);
    addEdgeToGraph(graph, 1, 2,  8 , 8, true);
    addEdgeToGraph(graph, 2, 8,  2 , 2, true);
    addEdgeToGraph(graph, 2, 5,  4 , 4, true);
    addEdgeToGraph(graph, 2, 3,  7,  7, true);
    addEdgeToGraph(graph, 3, 4,  9,  9, true);
    addEdgeToGraph(graph, 3, 5, 14, 14, true);
    addEdgeToGraph(graph, 4, 5, 10, 10, true);
    addEdgeToGraph(graph, 5, 6,  2,  2, true);
    addEdgeToGraph(graph, 6, 7,  1,  1, true);
    addEdgeToGraph(graph, 6, 8,  6,  6, true);
    addEdgeToGraph(graph, 7, 8,  7,  7, true);

    printGraph(graph);
    dijkstra(graph, 0);

    printf("The Shortest path from 0 to all nodes : \r\n");
    for (int i = 0; i < graph->numOfNodes; i++){
        printf("%2d ", i);
    }
    printf("\r\n");
    for (int i = 0; i < graph->numOfNodes; i++){
        printf("%2d ", graph->distance[i]);
    }
    printf("\r\n");

    return 0;
}