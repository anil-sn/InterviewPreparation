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

typedef struct graph_{
    int graph[MAX][MAX];
    bool visited[MAX];
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

    for(int i = 0; i < g->numOfNodes; i++) {
        g->graph[i][i] = 1;
    }

    return g;
}

// Function to add edge between nodes
void addEdge(GRAPH *g, int a, int b)
{
    g->graph[a][b]=1;
    g->graph[b][a]=1;
}

void dfs(int node, GRAPH *g)
{
    printf(" -> %d ", node);
    g->visited[node] = true;

    for(int i = 0; i < g->numOfNodes; i++) {
        if((false == g->visited[i]) && (g->graph[node][i])) {
            dfs(i, g);
        }
    }

    return;
}

void DFS(GRAPH *g)
{
    for(int i = 0; i < g->numOfNodes; i++) {
        if(false == g->visited[i]) {
            dfs(i, g);
        }
    }

    return;
}

void DFS_Stack(GRAPH *g, int source)
{
    // Create a stack for DFS
    STACK *s = initStack();

    // Push the current source node.
    stackPush(s, source);

    while (false == isStackEmpty(s)) {

        int data = stackPop(s);
        if (false == g->visited[data]) {

            printf("-> %d ", data);
            g->visited[data] = true;
        }

        for (int i = 0; i < g->numOfNodes; i++) {
            if((false == g->visited[i]) && (g->graph[data][i])) {
                stackPush(s, i);
            }
        }
    }

    return;
}

void BFS_Queue(GRAPH *g, int source)
{
    QUEUE *q = initQueue();

    enQueue(q, source);
    g->visited[source] = true;

    while (false == isQueueEmpty(q)) {

        int node = deQueue(q);
        printf("-> %d ", node);

        for (int i = 0; i < g->numOfNodes; i++) {
            if((false == g->visited[i]) && (g->graph[node][i])) {
                enQueue(q, i);
                g->visited[i] = true;
            }
        }
    }

    return;
}

int main()
{
    GRAPH *g = initGrapth(7);

    addEdge(g, 0, 1);
    addEdge(g, 0, 5);
    addEdge(g, 0, 2);
    addEdge(g, 1, 3);
    addEdge(g, 2, 3);
    addEdge(g, 2, 4);
    addEdge(g, 3, 6);
    addEdge(g, 4, 6);
    addEdge(g, 4, 5);

    //DFS(g);
    //DFS_Stack(g, 0);
    BFS_Queue(g, 0);
    printf("\r\n");

    return 0;
}