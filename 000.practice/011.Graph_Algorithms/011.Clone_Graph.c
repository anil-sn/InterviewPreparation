#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Given a reference of a node in a connected undirected graph.

    Return a deep copy (clone) of the graph.

    Each node in the graph contains a value (int) and a list (List[Node]) of its neighbors.

    class Node {
        public int val;
        public List<Node> neighbors;
    }

    Definition for a Node.
        struct Node {
        int val;
        int numNeighbors;
        struct Node** neighbors;
    };

    struct Node *cloneGraph(struct Node *s) {
    }

    Test case format:
    For simplicity, each node's value is the same as the node's index (1-indexed).
    For example, the first node with val == 1, the second node with val == 2, and so on.
    The graph is represented in the test case using an adjacency list.
    An adjacency list is a collection of unordered lists used to represent a finite graph.
    Each list describes the set of neighbors of a node in the graph.
    The given node will always be the first node with val = 1.
    You must return the copy of the given node as a reference to the cloned graph.

    Input: adjList = [[2,4],[1,3],[2,4],[1,3]]
    Output: [[2,4],[1,3],[2,4],[1,3]]

    Explanation: There are 4 nodes in the graph.

    1st node (val = 1)'s neighbors are 2nd node (val = 2) and 4th node (val = 4).
    2nd node (val = 2)'s neighbors are 1st node (val = 1) and 3rd node (val = 3).
    3rd node (val = 3)'s neighbors are 2nd node (val = 2) and 4th node (val = 4).
    4th node (val = 4)'s neighbors are 1st node (val = 1) and 3rd node (val = 3).

    Example 2:

    Input: adjList = [[]]
    Output: [[]]
    Explanation: Note that the input contains one empty list.
    The graph consists of only one node with val = 1 and it does not have any neighbors.

    Example 3:
    Input: adjList = []
    Output: []
    Explanation: This an empty graph, it does not have any nodes.

    Solution:
        The idea is to do a BFS traversal of the graph and while visiting
        a node make a clone node of it (a copy of original node).
        If a node is encountered which is already visited then it already has a clone node.
    https://programs.programmingoneonone.com/2021/08/leetcode-clone-graph-problem-solution.html
*/
struct Node {
    int val;
    int numNeighbors;
    struct Node** neighbors;
};

static struct Node* newNode(struct Node* original) {
    struct Node* new = malloc(sizeof(*new));
    new->val = original->val;
    new->numNeighbors = original->numNeighbors;
    new->neighbors = calloc(new->numNeighbors, sizeof(struct Node*));
    return new;
}

static struct Node* dfsCopy(struct Node* s, struct Node** newNodes) {
    int curr = s->val;
    newNodes[curr] = newNode(s);
    for (int i = 0; i < s->numNeighbors; i++) {
        if (newNodes[s->neighbors[i]->val] == NULL) {
            newNodes[curr]->neighbors[i] = dfsCopy(s->neighbors[i], newNodes);
        } else {
            newNodes[curr]->neighbors[i] = newNodes[s->neighbors[i]->val];
        }
    }
    return newNodes[curr];
}

struct Node *cloneGraph(struct Node *s) {
    struct Node* newGraph = NULL;
    if (s != NULL) {
        struct Node** newNodes = calloc(101, sizeof(struct Node*));
        newGraph = dfsCopy(s, newNodes);
        free(newNodes);
    }
    return newGraph;
}

int adj[100][100];

void
addUnDirectedEdge(int v,int w)
{
    adj[w][v] = 1;
    adj[v][w] = 1;
}

void
addDirectedEdge(int v,int w)
{
    adj[v][w] = 1;
}

void
printAdjMatrix (int n)
{
    printf("Adjecency Matrix : \r\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", adj[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");
    return;
}

static struct Node* newNode(struct Node* original) {
    struct Node* new = malloc(sizeof(*new));
    new->val = original->val;
    new->numNeighbors = original->numNeighbors;
    new->neighbors = calloc(new->numNeighbors, sizeof(struct Node*));
    return new;
}

struct Node* createAdjList(int numberOfNodes) {

    struct Node** newNodes = calloc(numberOfNodes, sizeof(struct Node*));

    for (int i = 0; i < numberOfNodes; i++) {
        int count = 0;
        for (int j = 0; j < numberOfNodes; j++) {
            if (0 == adj[i][j]) {
                continue;
            }
            count++;
        }
        newNodes[i]->val = i;
        newNodes[i]->numNeighbors = count;
    }
}

int main () {
    int numberOfNodes = 5;
    int input[][2] = {{ 2, 4 },
                      { 1, 3 },
                      { 2, 4 },
                      { 1, 3 }};

    int numRows = 4;

    for (int i = 0; i < numRows; i++) {
        addUnDirectedEdge(input[i][0], input[i][1]);
    }
    printAdjMatrix(numberOfNodes);

    //struct Node *newGraph = cloneGraph();

    return;
}