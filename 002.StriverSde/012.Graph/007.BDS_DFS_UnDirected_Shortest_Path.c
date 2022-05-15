#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX     100


typedef struct graph_{
    int graph[MAX][MAX];
    bool visited[MAX];
    bool dfsVisitPath[MAX];

    u_int32_t distance[MAX];

    int numOfNodes;
} GRAPH;

typedef struct stack_ {
    int stack[MAX];
    int top;
    int capacity;
} STACK;

typedef struct Queue_ {
    int queue[MAX];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAX;
    return stack;
}

QUEUE* initQueue()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->start = -1;
    queue->end = -1;
    queue->currSize = 0;
    queue->maxSize = MAX;

    return queue;
}

bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

int stackPeek(STACK *s)
{
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return -1;
    }

    return (s->stack[s->top]);
}

void stackPush(STACK *s, int value)
{
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    //printf("Element %d is pushed to stack \r\n", value->data);
    s->stack[++s->top] = value;
    return;
}

int stackPop(STACK *s)
{
    if (isStackEmpty(s)) {
        return -1;
    }

    return (s->stack[s->top--]);
}

int queueSize(QUEUE *q)
{
    return (q->currSize);
}

bool isQueueFull(QUEUE *q)
{
    return (q->currSize >= (q->maxSize - 1)) ? true : false;
}

bool isQueueEmpty(QUEUE *q)
{
    return (q->start < 0) ? true : false;
}

void enQueue(QUEUE *q, int val)
{
    if (isQueueFull(q)) {
        printf("Queue is full\r\nExiting...\r\n");
    }

    if (q->end == -1) {
        q->end = 0;
        q->start = 0;
    } else {
        q->end = (q->end + 1) % q->maxSize;
    }

    q->queue[q->end] = val;
    //printf("Element %d is enqued \r\n", val->data);
    q->currSize++;
}

int deQueue(QUEUE *q)
{
    int val;
    if (isQueueEmpty(q)) {
        printf("Queue is Empty \r\nExiting...\r\n");
    }

    val = q->queue[q->start];
    if (q->currSize == 1) {
        q->end = -1;
        q->start = -1;
    } else {
        q->start = (q->start + 1) % q->maxSize;
    }

    q->currSize--;
    return val;
}

// function to initialise the adjacency matrix
GRAPH * initGrapth(int num)
{
    GRAPH *g = malloc(sizeof(GRAPH));
    memset(g, 0, sizeof(GRAPH));

    g->numOfNodes = num;
    memset(g->visited, false, sizeof(g->visited));
    memset(g->dfsVisitPath, false, sizeof(g->dfsVisitPath));
    memset(g->distance, INT_MAX, sizeof(g->distance));

    return g;
}

void initVisited(GRAPH *g)
{
    memset(g->visited, false, sizeof(g->visited));
    memset(g->dfsVisitPath, false, sizeof(g->dfsVisitPath));
    memset(g->distance, INT_MAX, sizeof(g->distance));

    return;
}

// Function to add edge between nodes
void addEdge(GRAPH *g, int a, int b)
{
    g->graph[a][b]=1;
    g->graph[b][a]=1;
}

// Function to add edge between nodes
void removeEdge(GRAPH *g, int a, int b)
{
    g->graph[a][b]=0;
    g->graph[b][a]=0;
}

void shortestPath_BSF(int src, GRAPH *g)
{
    QUEUE *q = initQueue();
    initVisited(g);
    g->distance[src] = 0;
    enQueue(q, src);

    while(false == isQueueEmpty(q)) {

        int node = deQueue(q);
        for(int i = 0; i < g->numOfNodes; i++) {

            if(false == g->graph[node][i]) {
                continue;
            }

            if(g->distance[node] + 1 < g->distance[i]) {

                g->distance[i] = g->distance[node] + 1;
                enQueue(q, i);
            }
        }
    }
    return;
}

// Depth-limited search
int dls(GRAPH *g, int currentNode, int goal, int maxDepth, int currentDepth)
{
    int found = -1;

    if (true == g->visited[currentNode]) {
        return -1;
    }

    g->visited[currentNode] = true;

    if (goal == currentNode) {
        return currentNode;
    }

    // Stop searching when exceed max depth this is the only difference from DFS
    if (currentDepth >= maxDepth) {
        return -1;
    }

    for(int i = 0; i < g->numOfNodes; i++) {

        if(false == g->graph[currentNode][i]) {
            continue;
        }

        found = dls(g, i, goal, maxDepth, currentDepth + 1);
        if (found != -1) {
            return found;
        }
    }

    return -1;
}

// iterative deepening depth first search
int shortestPath_IDS(GRAPH *g, int start, int goal)
{
    int depth = 1;
    int found = -1;

    while ((found != goal) && (depth <= g->numOfNodes)) {

        found = dls(g, start, goal, depth, 0);
        if (found == goal) {
            return depth;
        }

        initVisited(g);
        depth = depth + 1;
    }

    return depth;
}

int main ()
{
    GRAPH *g = initGrapth(9);
    addEdge(g, 0, 1);
    addEdge(g, 0, 3);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);
    addEdge(g, 2, 6);
    addEdge(g, 3, 4);
    addEdge(g, 4, 5);
    addEdge(g, 5, 6);
    addEdge(g, 6, 7);
    addEdge(g, 6, 8);
    addEdge(g, 7, 8);

    int src = 0;
    int dest = 8;
    shortestPath_BSF(src, g);

    printf("[BFS] The shortest path from %d to nodes: \r\n", src);
    for(int i = 0; i < g->numOfNodes; i++) {
        printf(" %2d = %3d \r\n", i, g->distance[i]);
    }
    printf("\r\n");

    int distance = shortestPath_IDS(g, src, dest);

    printf("[DFS] The shortest path distance from %d to %d is : %d\r\n", src, dest, distance);
    return;
}