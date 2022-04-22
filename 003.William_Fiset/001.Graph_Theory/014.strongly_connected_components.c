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

typedef struct stack_ {
    int stack[MAXIMUM];
    int top;
    int capacity;
} STACK;

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAXIMUM;
    return stack;
}

bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

void push(STACK *s, int value) {
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    printf("\t [STACK] Push : %d \r\n", value);
    s->stack[++s->top] = value;
    return;
}

int peek(STACK *s) {
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return -1;
    }

    return (s->stack[s->top]);
}

int pop(STACK *s) {
    if (isStackEmpty(s)) {
        return -1;
    }

    printf("\t [STACK] Pop : %d \r\n", s->stack[s->top]);
    return (s->stack[s->top--]);
}

typedef struct Edge {
    int from;
    int to;
    int weight;
    struct Edge *prev;
    struct Edge *next;
} EDGE;

typedef struct graph{
    int id;
    int sccCount;
    EDGE *nodes[MAXIMUM];
    bool visited[MAXIMUM];
    bool onStack[MAXIMUM];
    int  lowLink[MAXIMUM];
    int  sccs[MAXIMUM];
    int  ids[MAXIMUM];
    int  numOfNodes;
} GRAPH;

void refreshGraph(GRAPH * graph)
{
    memset(graph->visited, false, sizeof(graph->visited));
    memset(graph->lowLink, 0, sizeof(graph->lowLink));
    memset(graph->sccs, 0, sizeof(graph->sccs));
    memset(graph->ids, 0, sizeof(graph->ids));
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
    printf("[%2d]:->\t", node);

    if (NULL == head){
        printf("  NIL");
    }

    while (NULL != head){
        printf("%2d->%2d [%2d]\t", head->from, head->to, head->weight);
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

void dfs(int at, GRAPH * g, STACK *s)
{
    g->id++;
    g->ids[at] = g->id;
    g->lowLink[at] = g->id;

    g->onStack[at] = true;
    push(s, at);
    g->visited[at] = true;

    printf("[DFS] Node %d's ID: %d Low Link Value : %d\r\n", at, g->ids[at], g->lowLink[at]);
    EDGE *edge = g->nodes[at];
    while (NULL != edge){
        if(false == g->visited[edge->to]) {
            dfs(edge->to, g, s);
        }

        if (true == g->onStack[edge->to]) {

            g->lowLink[at] = MIN(g->lowLink[at], g->lowLink[edge->to]);
            printf("[DFS Finished] Node %d's Low Link Value : %d\r\n", at, g->lowLink[at]);
        }

        edge = edge->next;
    }

    // On recursive callback,
    // if we're at the root node (start of SCC)
    // empty the seen stack until back to root.
    int node = 0;
    if (g->ids[at] == g->lowLink[at]) {
        while (false == isStackEmpty(s)){
            node = pop(s);
            g->onStack[node] = false;
            g->lowLink[node] = g->ids[at];
            g->sccs[node] = g->sccCount;
            if (node == at) {
                break;
            }
        }

        g->sccCount++;
    }
}

void StronglyConnectedComponents(GRAPH * g)
{
    STACK *s = initStack();
    for (int i = 0; i < g->numOfNodes; i++) {
        if (false == g->visited[i]) {
            dfs(i, g, s);
        }
    }
}

int main(void) {

    // Graph setup
    int N = 8;
    GRAPH * g = initGraph(N);

    addEdgeToGraph(g, 0, 1,  0,  0, false);
    addEdgeToGraph(g, 1, 2,  0,  0, false);
    addEdgeToGraph(g, 2, 0,  0,  0, false);
    addEdgeToGraph(g, 3, 7,  0,  0, false);
    addEdgeToGraph(g, 3, 4,  0,  0, false);
    addEdgeToGraph(g, 4, 5,  0,  0, false);
    addEdgeToGraph(g, 5, 6,  0,  0, false);
    addEdgeToGraph(g, 6, 4,  0,  0, false);
    addEdgeToGraph(g, 7, 3,  0,  0, false);
    addEdgeToGraph(g, 7, 5,  0,  0, false);

    printGraph(g);
    StronglyConnectedComponents(g);

    printf("The number of Strongly connected components: %d \r\n", g->sccCount);

    printf("The Nodes                         : ");
    for (int i = 0; i < g->numOfNodes; i++){
        printf("%2d", i);
    }
    printf("\r\n");

    printf("The Node Traversal IDs            : ");
    for (int i = 0; i < g->numOfNodes; i++){
        printf("%2d", g->ids[i]);
    }
    printf("\r\n");

    printf("The Low link values               : ");
    for (int i = 0; i < g->numOfNodes; i++){
        printf("%2d", g->lowLink[i]);
    }
    printf("\r\n");


    printf("The Strongly connected Components : ");
    for (int i = 0; i < g->numOfNodes; i++){
        printf("%2d", g->sccs[i]);
    }
    printf("\r\n");

    return 0;
}