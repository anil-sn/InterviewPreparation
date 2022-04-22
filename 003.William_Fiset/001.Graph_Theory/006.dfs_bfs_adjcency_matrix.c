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

STACK * newStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAX;
    return stack;
}

QUEUE* newQ()
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

int qSize(QUEUE *q)
{
    return (q->currSize);
}

bool isQFull(QUEUE *q)
{
    return (q->currSize >= (q->maxSize - 1)) ? true : false;
}

bool isQEmpty(QUEUE *q)
{
    return (q->start < 0) ? true : false;
}

void addQ(QUEUE *q, int val)
{
    if (isQFull(q)) {
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

int delQ(QUEUE *q)
{
    int val;
    if (isQEmpty(q)) {
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

void refreshGraph(GRAPH *g)
{
    memset(g->visited, false, sizeof(g->visited));
}

// function to initialise the adjacency matrix
GRAPH * newGraph(int num)
{
    GRAPH *g = malloc(sizeof(GRAPH));
    memset(g, 0, sizeof(GRAPH));

    g->numOfNodes = num;
    memset(g->graph, 0, sizeof(g->graph));
    for(int i = 0; i < g->numOfNodes; i++) {
        g->graph[i][i] = 1;
    }

    refreshGraph(g);
    return g;
}

// Function to add edge between nodes
void addEdge(GRAPH *g, int a, int b, bool isDirected)
{
    g->graph[a][b]=1;
    if (false == isDirected) {
        g->graph[b][a]=1;
    }
}

void dfs(int node, GRAPH *g)
{
    printf("%3d ->", node);
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
    refreshGraph(g);
    for(int i = 0; i < g->numOfNodes; i++) {
        if(false == g->visited[i]) {
            dfs(i, g);
        }
    }

    return;
}

void DFS_Stack(GRAPH *g, int source)
{
    refreshGraph(g);

    // Create a stack for DFS
    STACK *s = newStack();

    // Push the current source node.
    stackPush(s, source);

    while (false == isStackEmpty(s)) {

        int data = stackPop(s);
        if (false == g->visited[data]) {

            printf("%3d ->", data);
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
    QUEUE *q = newQ();

    refreshGraph(g);

    addQ(q, source);
    g->visited[source] = true;

    while (false == isQEmpty(q)) {

        int node = delQ(q);
        printf("%3d ->", node);

        for (int i = 0; i < g->numOfNodes; i++) {
            if((false == g->visited[i]) && (g->graph[node][i])) {
                addQ(q, i);
                g->visited[i] = true;
            }
        }
    }

    return;
}

int main()
{
    GRAPH *g = newGraph(7);

    addEdge(g, 0, 1, false);
    addEdge(g, 0, 5, false);
    addEdge(g, 0, 2, false);
    addEdge(g, 1, 3, false);
    addEdge(g, 2, 3, false);
    addEdge(g, 2, 4, false);
    addEdge(g, 3, 6, false);
    addEdge(g, 4, 6, false);
    addEdge(g, 4, 5, false);

    printf("\r\n DFS with recursion: ");
    DFS(g);

    printf("\r\n DFS with stack    : ");
    DFS_Stack(g, 0);

    printf("\r\n BFS with queue    : ");
    BFS_Queue(g, 0);
    printf("\r\n");

    return 0;
}