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
    bool dfsVisitPath[MAX];
    int inDegree[MAX];
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

bool isCycleExistsDFSHelper(int node, int parent, GRAPH *g)
{
    bool result = false;

    if (node > g->numOfNodes) {
        return false;
    }

    printf("Visiting Node: [%d] from [%d]\r\n", node, parent);
    g->visited[node] = true;
    g->dfsVisitPath[node] = true;

    for (int i = 0; i < g->numOfNodes; i++) {
        if((g->graph[node][i]) && (i != node)) {
            if (false == g->visited[i]) {
                if(isCycleExistsDFSHelper(i, node, g)) {
                    return true;
                }
            } else if (true == g->dfsVisitPath[i]) {
                return true;
            }
        }
    }

    g->dfsVisitPath[node] = false;
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

bool isCycleExistsBFS(GRAPH *g)
{
    QUEUE *q = initQueue();
    initVisited(g);

    for(int i = 0; i < g->numOfNodes; i++) {
        for(int j = 0; j < g->numOfNodes; j++) {
            if ((i != j) && (true == g->graph[i][j])) {
                g->inDegree[j] += 1;
            }
        }
    }

    for(int i = 0; i < g->numOfNodes; i++) {
        if(0 == g->inDegree[i]) {
            enQueue(q, i);
            printf("QUEUE Node: [%d]\r\n", i);
        }
    }

    int cnt = 0;
    while(false == isQueueEmpty(q)) {
        int node = deQueue(q);
        printf("Visiting Node: [%d]\r\n", node);
        cnt++;
        for(int i = 0; i < g->numOfNodes; i++) {
            if ((i != node) && (true == g->graph[node][i])) {
                g->inDegree[i] -= 1;
                if (0 == g->inDegree[i]) {
                    enQueue(q, i);
                    printf("QUEUE Node: [%d]\r\n", i);
                }
            }
        }
    }

    if(cnt == g->numOfNodes) {
        return false;
    }

    return true;
}

int main()
{
    bool result = false;
    GRAPH *g = initGrapth(10);

    addEdge(g, 0, 1);
    addEdge(g, 0, 3);
    addEdge(g, 1, 2);
    addEdge(g, 2, 3);
    addEdge(g, 3, 4);
    addEdge(g, 3, 6);
    addEdge(g, 4, 5);
    addEdge(g, 6, 5);
    addEdge(g, 7, 2);
    addEdge(g, 7, 8);
    //addEdge(g, 8, 9);
    addEdge(g, 9, 7);

#if 0
    printf("\r\n-----------------------DFS------------------------\r\n");
    addEdge(g, 8, 9);
    result = isCycleExistsDFS(g);
    printf("In Graph Cycle : %s \r\n", result ? "Exists" : "Does not Exists");

    printf("\r\n---------------------------------------------------\r\n");
    removeEdge(g, 8, 9);
    result = isCycleExistsDFS(g);
    printf("In Graph Cycle : %s \r\n", result ? "Exists" : "Does not Exists");
#endif

    printf("\r\n-----------------------BFS------------------------\r\n");
    addEdge(g, 8, 9);
    result = isCycleExistsBFS(g);
    printf("In Graph Cycle : %s \r\n", result ? "Exists" : "Does not Exists");


#if 0
    printf("\r\n---------------------------------------------------\r\n");
    removeEdge(g, 8, 9);
    result = isCycleExistsBFS(g);
    printf("In Graph Cycle : %s \r\n", result ? "Exists" : "Does not Exists");
#endif

    return 0;
}