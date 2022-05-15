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

    unsigned int distance[MAX];

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

    printf("Element %d is pushed to stack \r\n", value);
    s->stack[++s->top] = value;
    return;
}

int stackPop(STACK *s)
{
    if (isStackEmpty(s)) {
        return -1;
    }

    printf("Element %d is poped out of stack \r\n", s->stack[s->top]);
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

void findTopoSort(int node, STACK *st, GRAPH *g)
{
    g->visited[node] = true;
    for (int i = 0; i < g->numOfNodes; i++) {
        if (-1 == g->graph[node][i]) {
            continue;
        }

        if(false == g->visited[i]) {
            findTopoSort(i, st, g);
        }
    }

    stackPush(st, node);
    return;
}

void shortestPath(int src, GRAPH *g)
{
    STACK *st = initStack();
    initVisited(g);

    for (int i = 0; i < g->numOfNodes; i++) {
        if (false == g->visited[i]) {
            findTopoSort(i, st, g);
        }
    }

    g->distance[src] = 0;

    while(false == isStackEmpty(st))
    {
        int node = stackPop(st);

        if (g->distance[node] == INT_MAX) {
            continue;
        }

        for (int i = 0; i < g->numOfNodes; i++) {

            if (-1 == g->graph[node][i]) {
                continue;
            }

            if (i == node) {
                continue;
            }

            if(g->distance[node] + g->graph[node][i] < g->distance[i]) {
                g->distance[i] = g->distance[node] + g->graph[node][i];
            }
        }
    }
}

int main ()
{
    GRAPH *g = initGrapth(9);
    addEdge(g, 0, 1, 1);
    addEdge(g, 0, 3, 1);
    addEdge(g, 1, 2, 1);
    addEdge(g, 1, 3, 1);
    addEdge(g, 2, 6, 1);
    addEdge(g, 3, 4, 1);
    addEdge(g, 4, 5, 1);
    addEdge(g, 5, 6, 1);
    addEdge(g, 6, 7, 1);
    addEdge(g, 6, 8, 1);
    addEdge(g, 7, 8, 1);


    int src = 0;
    shortestPath(src, g);

    printf("[DFS] The shortest path from %d to nodes: \r\n", src);
    for(int i = 0; i < g->numOfNodes; i++) {
        printf(" %2d = %3d \r\n", i, g->distance[i]);
    }
    printf("\r\n");

    return;
}