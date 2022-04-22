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

typedef struct pair_ {
    int parent;
    int node;
}PAIR;

typedef struct stack_ {
    int stack[MAX];
    int top;
    int capacity;
} STACK;

typedef struct Queue_ {
    PAIR* queue[MAX];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

PAIR * newPair(int node, int parent)
{
    PAIR *p = malloc(sizeof(PAIR));
    p->parent = parent;
    p->node = node;

    return p;
}

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

void enQueue(QUEUE *q, PAIR* val)
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

PAIR* deQueue(QUEUE *q)
{
    PAIR * val;
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

#if 0
    for(int i = 0; i < g->numOfNodes; i++) {
        g->graph[i][i] = 1;
    }
#endif

    return g;
}

void initVisited(GRAPH *g)
{
    memset(g->visited, false, sizeof(g->visited));
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

bool isCycleExistsDFSHelper(int node, int parent, GRAPH *g)
{
    bool result = false;

    if (node > g->numOfNodes) {
        return false;
    }

    printf("Visiting Node: [%d] from [%d]\r\n", node, parent);
    g->visited[node] = true;

    for (int i = 0; i < g->numOfNodes; i++) {
        if((g->graph[node][i]) && (i != node)) {
            if (false == g->visited[i]) {
                if(isCycleExistsDFSHelper(i, node, g)) {
                    return true;
                }
            } else if ((-1 != parent) && (i != parent)) {
                return true;
            }
        }
    }

    return false;
}

bool isCycleExistsDFS(GRAPH *g)
{
    bool result = false;

    initVisited(g);

    for (int i = 0; i < g->numOfNodes; i++) {
        if (false == g->visited[i]) {
            result = isCycleExistsDFSHelper(i, -1, g);
            if (true == result) {
                return result;
            }
        }
    }

    return result;
}

bool isCycleExistsBFSHelper(int node, GRAPH *g)
{
    PAIR *p = NULL;
    bool result = false;
    QUEUE *q = initQueue();

    g->visited[node] = true;
    enQueue(q, newPair(node, -1));
    printf("Visiting Node: [%d] from [%d]\r\n", node, -1);

    while (false == isQueueEmpty(q)){
        p = deQueue(q);
        for (int i = 0; i < g->numOfNodes; i++) {
            if((g->graph[p->node][i]) && (i != p->node)) {
                if (false == g->visited[i]) {
                    g->visited[i] = true;
                    enQueue(q, newPair(i, p->node));
                    printf("Visiting Node: [%d] from [%d]\r\n", i, p->node);
                } else if (p->parent != i) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool isCycleExistsBFS(GRAPH *g)
{
    bool result = false;

    initVisited(g);

    for (int i = 0; i < g->numOfNodes; i++) {

        if (false == g->visited[i]) {

            result = isCycleExistsBFSHelper(i, g);
            if (true == result) {
                return result;
            }
        }
    }

    return result;
}

int main()
{
    bool result = false;
    GRAPH *g = initGrapth(12);

    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 2, 4);
    addEdge(g, 3, 5);
    addEdge(g, 5, 6);
    addEdge(g, 5, 10);
    addEdge(g, 6, 7);
    addEdge(g, 7, 8);
    addEdge(g, 8, 11);
    addEdge(g, 9, 10);

    printf("\r\n-----------------------DFS------------------------\r\n");
    addEdge(g, 8, 9);
    result = isCycleExistsDFS(g);
    printf("In Graph Cycle : %s \r\n", result ? "Exists" : "Does not Exists");

    printf("\r\n---------------------------------------------------\r\n");
    removeEdge(g, 8, 9);
    result = isCycleExistsDFS(g);
    printf("In Graph Cycle : %s \r\n", result ? "Exists" : "Does not Exists");

    printf("\r\n-----------------------BFS------------------------\r\n");
    addEdge(g, 8, 9);
    result = isCycleExistsBFS(g);
    printf("In Graph Cycle : %s \r\n", result ? "Exists" : "Does not Exists");

    printf("\r\n---------------------------------------------------\r\n");
    removeEdge(g, 8, 9);
    result = isCycleExistsBFS(g);
    printf("In Graph Cycle : %s \r\n", result ? "Exists" : "Does not Exists");

    return 0;
}