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

typedef struct graph_{
    int graph[MAX][MAX];
    int numOfNodes;

    bool visited[MAX];
    bool dfsVisitPath[MAX];

    int inDegree[MAX];

    STACK *s;
    QUEUE *q;
} GRAPH;

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
    memset(g->inDegree, 0, sizeof(g->inDegree));

    return g;
}

void initVisited(GRAPH *g)
{
    memset(g->visited, false, sizeof(g->visited));
    memset(g->dfsVisitPath, false, sizeof(g->dfsVisitPath));
    memset(g->inDegree, 0, sizeof(g->inDegree));

    return;
}

// Function to add edge between nodes
void addEdge(GRAPH *g, int a, int b)
{
    g->graph[a][b]=1;
}

// Function to add edge between nodes
void removeEdge(GRAPH *g, int a, int b)
{
    g->graph[a][b]=0;
}

void findTopoSortDFS(int node, GRAPH *g)
{
    g->visited[node] = true;

    for(int i = 0; i < g->numOfNodes; i++) {
        if ((i != node) && (false == g->visited[i]) && (true == g->graph[node][i])) {
            findTopoSortDFS(i, g);
        }
    }

    stackPush(g->s, node);
}

void topoSortDFS(GRAPH *g)
{
    g->s = initStack();
    initVisited(g);

    for(int i = 0; i < g->numOfNodes; i++) {
        if (false == g->visited[i]) {
            findTopoSortDFS(i, g);
        }
    }

    while(false == isStackEmpty(g->s)) {

        printf(" ->%2d", stackPop(g->s));
    }

    return;
}

void topoSortBFS(GRAPH *g)
{
    g->q = initQueue();
    initVisited(g);

    for(int i = 0; i < g->numOfNodes; i++) {
        for(int j = 0; j < g->numOfNodes; j++) {
            if ((i != j) && (true == g->graph[i][j])) {
                g->inDegree[j] += 1;
            }
        }
    }

    //printf("\r\n");
    for(int i = 0; i < g->numOfNodes; i++) {
        //printf("%d->[%d],", i, g->inDegree[i]);
        if(0 == g->inDegree[i]) {
            enQueue(g->q, i);
        }
    }
    //printf("\r\n Topological Sort : ");


    while(false == isQueueEmpty(g->q)) {

        int node = deQueue(g->q);
        printf(" ->%2d", node);

        for(int i = 0; i < g->numOfNodes; i++) {
            if ((i != node) && (true == g->graph[node][i])) {
                g->inDegree[i] -= 1;
                if (0 == g->inDegree[i]) {
                    enQueue(g->q, i);
                }
            }
        }

    }

    return;
}

int main ()
{
    GRAPH *g = initGrapth(10);
    addEdge(g, 0,1);
    addEdge(g, 0,3);
    addEdge(g, 1,2);
    addEdge(g, 2,3);
    addEdge(g, 3,4);
    addEdge(g, 3,6);
    addEdge(g, 4,5);
    addEdge(g, 6,5);
    addEdge(g, 7,2);
    addEdge(g, 7,8);
    addEdge(g, 8,9);
    //addEdge(g, 9,7);

    printf("[DFS] The Topological sort : ");
    topoSortDFS(g);
    printf("\r\n");

    printf("[BFS] The Topological sort : ");
    topoSortBFS(g);
    printf("\r\n");
}