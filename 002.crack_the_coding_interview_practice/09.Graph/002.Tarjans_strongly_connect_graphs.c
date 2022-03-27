#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    TARJAN’S ALGORITHM FOR STRONGLY CONNECTED COMPONENTS

    Strongly connected components (SCCs) can be thought of as self-contained cycles
    within a directed graph where every vertex in a given cycle can reach every other
    vertex in the same cycle.

    LOW-LINK VALUES
    Before we jump to Tarjan’s algorithm, we must have an understanding of low-link values.
    The low-link value of a node is the smallest node ID reachable from that node when doing a depth-first search (DFS), including itself.
    There is a catch with doing a DFS on the graph, as it is highly dependent on the traversal order of the DFS, which is effectively random.
    Note: Depending on where the DFS starts, and the order in which nodes/edges are visited, the low-link values for identifying SCCs could be wrong.
    So, to make it work, we have to use an invariant that prevents SCCs from interfering with the low-link value of other SCCs.

    STACK INVARIANT
    ALGORITHM:
    To cope with the random traversal order of the DFS, Tarjan’s algorithm maintains
    a stack of valid nodes from which to update low-link values.
    Nodes are added to the stack of valid nodes as they are explored for the first time.
    Nodes are removed from the stack each time a complete SCC is found.

    UPDATE CONDITION FOR LOW-LINK
    If u and v are nodes in a graph and we were currently exploring u,
    then our new low-link update condition is:
    To update node u to node v low-link there has to be a path of edges from u to v
    and node v must be on the stack.

    TIME COMPLEXITY
    We are going to update low-link values on the fly during the DFS so we can get a
    linear O(V+E) time complexity.

*/
#define v 5

int graph[NODE][NODE] = {
   {0, 0, 1, 1, 0},
   {1, 0, 0, 0, 0},
   {0, 1, 0, 0, 0},
   {0, 0, 0, 0, 1},
   {0, 0, 0, 0, 0}
};

void findComponent(int u, int disc[], int low[], stack<int>&stk, bool stkItem[]) {
   static int time = 0;
   disc[u] = low[u] = ++time;    //inilially discovery time and low value is 1
   stk.push(u);
   stkItem[u] = true;    //flag as u in the stack

   for(int v = 0; v<NODE; v++) {
      if(graph[u][v]) {
         if(disc[v] == -1) {   //when v is not visited
            findComponent(v, disc, low, stk, stkItem);
            low[u] = min(low[u], low[v]);
         } else if(stkItem[v])    //when v is in the stack, update low for u
            low[u] = min(low[u], disc[v]);
      }
   }

   int poppedItem = 0;
   if(low[u] == disc[u]) {
      while(stk.top() != u) {
         poppedItem = stk.top();
         cout << poppedItem << " ";
         stkItem[poppedItem] = false;    //mark as item is popped
         stk.pop();
      }
      poppedItem = stk.top();
      cout << poppedItem <<endl;
      stkItem[poppedItem] = false;
      stk.pop();
   }
}

void strongConComponent() {
   int disc[NODE], low[NODE];
   bool stkItem[NODE];
   stack<int> stk;

   for(int i = 0; i<NODE; i++) {    //initialize all elements
      disc[i] = low[i] = -1;
      stkItem[i] = false;
   }

   for(int i = 0; i<NODE; i++)    //initialize all elements
      if(disc[i] == -1)
         findComponent(i, disc, low, stk, stkItem);
}

int main() {
   strongConComponent();
}