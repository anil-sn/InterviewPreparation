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
    4.1 Route Between Nodes: Given a directed graph, design an algorithm to find out whether there is a
        route between two nodes.
    SOLUTION
        This problem can be solved by just simple graph traversal, such as depth-first search or breadth-first search.
        We start with one of the two nodes and, during traversal, check if the other node is found. We should mark
        any node found in the course of the algorithm as "already visited" to avoid cycles and repetition of the
        nodes.

        It may be worth discussing with your interviewer the tradeoffs between breadth-first search and depth-first
        search for this and other problems. For example, depth-first search is a bit simpler to implement since it can
        be done with simple recursion. Breadth-first search can also be useful to find the shortest path, whereas
        depth-first search may traverse one adjacent node very deeply before ever going onto the immediate
        neighbors.

    Step 1: One of the standard rule before starting with DFS algorithm is that
            DFS puts each vertex of graph into two categories i.e. Visited & Non Visited.
    Step 2: Keeping in mind that each vertex is visited at least once without making cycles.
    Step 3: First, need to put any one graph vertices on top of the stack
    Step 4: Need to take a top item of the stack and get it added to the visited list
    Step 5: Then, create a list of vertex adjacent nodes and
            add the ones which are not in the visited list to the top of the stack
    Step 6: Just keep repeating the above step 4 & step 5 until the stack gets emptied.
            DFS employs some major rules for traversal,
    Rule 1: Visit adjacent unvisited vertex and mark it as Visited.
            Display and push it to a stack
    Rule 2: If there is no adjacent vertex found, remove vertex from the top of the stack.
            It will remove all the vertices from the stack which has no adjacent vertices.
    Rule 3: Keep on repeating Rule 1 and Rule 2 until the stack is emptied.
*/


int32_t g_max_nodes = 0;

typedef struct Vertex {
   char label;
   bool visited;
} VERTEX;

//candiate variables
int16_t *candiate;
int16_t top;

//graph variables

//array of vertices
VERTEX* lstVertices;

//adjacency matrix
int32_t *adjMatrix;

//vertex count
int16_t vertexCount = 0;


//display the vertex
void
displayCandisateAddDel(int16_t vertexIndex, bool isAdd) {
    if (isAdd) {
       printf("Vertex [%c] is pushed to candidate stack \r\n", (lstVertices+vertexIndex)->label);
   } else {
       printf("Vertex [%c] is popped from candidate stack \r\n", (lstVertices+vertexIndex)->label);
   }
}

//candiate functions
bool
isStackEmpty() {
   return top == -1;
}

void
push(int16_t data) {
   top++;
   candiate[top] = data;
   displayCandisateAddDel(candiate[top], true);
}

int16_t
pop() {
   displayCandisateAddDel(candiate[top], false);
   return candiate[top--];
}

int16_t peek() {
   return candiate[top];
}

//graph functions
//add vertex to the vertex list
void
addVertex(char label) {
   VERTEX* cur = (lstVertices + vertexCount);
   cur->label = label;
   cur->visited = (char)false;
   vertexCount++;
}

//add edge to edge array
void
addEdge(int16_t start,int16_t end) {
    adjMatrix[start*g_max_nodes + end] = true;
    adjMatrix[start + g_max_nodes * end] = true;
}

//display the vertex
void
displayVertex(int16_t vertexIndex) {
   printf("\r\n *** Declare Vertex [%c] is reachable *** \r\n\r\n", (lstVertices+vertexIndex)->label);
}

//display the vertex
void
processingVertex(int16_t vertexIndex) {
   printf("Processing Vertex [%c] \r\n", (lstVertices+vertexIndex)->label);
}

//display the vertex
void
pickUpVertex(int16_t vertexIndex) {
   printf("Found Adjecent Vertex [%c] \r\n", (lstVertices+vertexIndex)->label);
}

//get the adjacent unvisited vertex
int16_t
getAdjUnvisitedVertex(int16_t vertexIndex) {
   int16_t i;

   for(i = 0; i < vertexCount; i++) {
      if(adjMatrix[vertexIndex * g_max_nodes + i] == true && (lstVertices+i)->visited == false) {
        return i;
     }
   }

   return -1;
}

void
displayAdjMatrix() {
    int16_t i,j;

    for (i = 0; i < g_max_nodes; i++) {
       for (j = 0; j < g_max_nodes; j++) {
          printf("%d \t", adjMatrix[i*g_max_nodes + j]);
       }
       printf("\r\n");
    }
    printf("\r\n");
}

void
displayVertices() {
    int16_t i;

    for(i = 0; i< vertexCount; i++) {
        printf("Vertes %c visited : [%s] \r\n", (lstVertices +i)->label, (lstVertices +i)->visited ? "True" : "False");
    }
    printf("\r\n");
   return;
}

void
initAdjMatrix() {
    int16_t i,j;

    top = -1;

     adjMatrix = calloc((g_max_nodes * g_max_nodes), sizeof(int32_t));
     lstVertices = calloc(g_max_nodes, sizeof(VERTEX));
     candiate = calloc(g_max_nodes * 10, sizeof(int16_t));

     for(i = 0; i< g_max_nodes; i++) { // set adjacency
         for(j = 0; j< g_max_nodes; j++) { // matrix to 0
             adjMatrix[i*g_max_nodes + j] = 0;
         }
     }
     return;
}

void
depthFirstSearch() {
   int i;

   //mark first node as visited
   (lstVertices+0)->visited = true;

   //display the vertex
   displayVertex(0);

   //push vertex index in stack
   push(0);

   while(!isStackEmpty()) {
      //get the unvisited vertex of vertex which is at top of the stack
      int unvisitedVertex = getAdjUnvisitedVertex(peek());

      processingVertex(unvisitedVertex);

      //no adjacent vertex found
      if(unvisitedVertex == -1) {
         pop();
      } else {
         (lstVertices + unvisitedVertex)->visited = true;
         displayVertex(unvisitedVertex);
         push(unvisitedVertex);
      }
   }

   //stack is empty, search is complete, reset the visited flag
   for(i = 0;i < vertexCount;i++) {
      (lstVertices +i)->visited = false;
   }
}

int32_t
main() {
    int32_t i, j, len;

    g_max_nodes = 5;

    printf("\r\n");

    initAdjMatrix();

    addVertex('S');   // 0
    addVertex('A');   // 1
    addVertex('B');   // 2
    addVertex('C');   // 3
    addVertex('D');   // 4

    printf ("Adding Edge 0->1 [S-A]\n");
    addEdge(0, 1);    // S - A

    printf ("Adding Edge 0->2 [S-B]\n");
    addEdge(0, 2);    // S - B

    printf ("Adding Edge 0->3 [S-C]\n");
    addEdge(0, 3);    // S - C

    printf ("Adding Edge 1->4 [A-D]\n");
    addEdge(1, 4);    // A - D

    printf ("Adding Edge 2->4 [B-D]\n");
    addEdge(2, 4);    // B - D

    printf ("Adding Edge 3->4 [C-D]\n");
    addEdge(3, 4);    // C - D

    printf("\r\n");

    displayAdjMatrix();
    displayVertices();

    printf("Depth First Search: \r\n");
    depthFirstSearch();

   return 0;
}
