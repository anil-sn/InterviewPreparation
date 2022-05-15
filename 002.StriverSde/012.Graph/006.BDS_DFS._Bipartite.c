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
#define WHITE   0
#define RED     1
#define BLUE    2

typedef struct graph_{
    int graph[MAX][MAX];
    bool visited[MAX];
    bool dfsVisitPath[MAX];
    int color[MAX];
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
    memset(g->color, WHITE, sizeof(g->color));

    return g;
}

void initVisited(GRAPH *g)
{
    memset(g->visited, false, sizeof(g->visited));
    memset(g->dfsVisitPath, false, sizeof(g->dfsVisitPath));
    memset(g->color, WHITE, sizeof(g->color));

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

int getNextColor(int myColor)
{
    if (myColor == RED) {
        return BLUE;
    }

    return RED;
}

bool isAdjColorOk(int myColor, int AdjColor) {
    return (myColor == AdjColor ? false : true);
}

bool isGraphBipartiteBFSHelper(int node, GRAPH *g)
{
    QUEUE *q = initQueue();
    g->color[node] = BLUE;
    g->visited[node] = true;
    enQueue(q, node);

    while (false == isQueueEmpty(q)) {
        node = deQueue(q);

        for (int i = 0; i < g->numOfNodes; i++){
            if(true != g->graph[node][i]) {
                continue;
            }

            if(true == g->visited[i]) {
                if (false == isAdjColorOk(g->color[node], g->color[i])) {
                    return false;
                }
                continue;
            }

            g->visited[i] = true;
            g->color[i] = getNextColor(g->color[node]);
            enQueue(q, i);
        }
    }

    return true;
}

bool isGraphBipartiteBFS(GRAPH *g)
{
    initVisited(g);
    for (int i = 0; i < g->numOfNodes; i++){
        if(WHITE == g->color[i]) {
            if(true != isGraphBipartiteBFSHelper(i, g)) {
                return false;
            }
        }
    }

    return true;
}

bool isGraphBipartiteDFSHelper(int node, GRAPH *g) {

    for (int i = 0; i < g->numOfNodes; i++) {

        if(false == g->graph[node][i]) {
            continue;
        }

        if(WHITE == g->color[i]) {

            g->color[i] = getNextColor(g->color[node]);
            if(false == isGraphBipartiteDFSHelper(i, g)) {
                return false;
            }
        } else if(g->color[i] == g->color[node]) {
            return false;
        }
    }

    return true;
}

bool isGraphBipartiteDFS(GRAPH *g)
{
    initVisited(g);
    for (int i = 0; i < g->numOfNodes; i++){
        if(WHITE == g->color[i]) {
            g->color[i] = BLUE;
            if(!isGraphBipartiteDFSHelper(i, g)) {
                return false;
            }
        }
    }
    return true;
}

int main ()
{
    GRAPH *g = initGrapth(8);
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 2, 3);
    addEdge(g, 3, 4);
    addEdge(g, 4, 5);
    addEdge(g, 4, 6);
    addEdge(g, 6, 7);
    addEdge(g, 7, 1);

    bool result = isGraphBipartiteBFS(g);
    printf("[BFS] The graph is : %s \r\n", result ? "Bipartite" : "Not Bipartite");

    GRAPH *g2 = initGrapth(8);
    addEdge(g2, 0, 1);
    addEdge(g2, 1, 2);
    addEdge(g2, 2, 3);
    addEdge(g2, 3, 4);
    addEdge(g2, 4, 5);
    addEdge(g2, 4, 6);
    addEdge(g2, 6, 1);
    addEdge(g2, 5, 7);

    result = isGraphBipartiteBFS(g2);
    printf("[BFS] The graph is : %s \r\n", result ? "Bipartite" : "Not Bipartite");

    result = isGraphBipartiteDFS(g);
    printf("[DFS] The graph is : %s \r\n", result ? "Bipartite" : "Not Bipartite");

    result = isGraphBipartiteDFS(g2);
    printf("[DFS] The graph is : %s \r\n", result ? "Bipartite" : "Not Bipartite");

    return;
}