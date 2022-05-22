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
    Brief note about Question-

    We have to return the length of the shortest path that visits every node.
    Let's take an example not given in question -
    Suppose graph given to us is like : [[1,4],[0,2],[1,3],[2,4],[3,0]]
    It should look like -
                              → → → →
                            0--------1
                        ↗  /         |
                       ↗  /          |
                         /           |  ↓
                        4            |  ↓
                         \           |  ↓
                       ↖  \          |  ↓
                        ↖  \         |
                            3--------2
                                ← ← ← ←

    The answer is 4 and one possible path is [0,1,2,3,4]
    I have mark that possible path with arrows, see above.
    Solution - I (Using BFS, Accepted)-

    When we see our constraint i.e 1 <= n <= 12, then it gives us an idea that to find the shortest path we may are able to explore all possibilites and all of those possibilities we find the shortest path.
    Suppose if there aren nodes, then how many total possiilities we have, we have 2 ^ n possible paths for n nodes.
    Whenever we encounter a problem to find the shortest path in graph, we will apply bfs to solve it because bfs will gurantee us to be the shortest path.
    Now, when we know that we have to explore all possibility of a graph, then we need some variables and data structure to avoid repetative paths and to know whether we explore all other nodes from this current node.
    Variables to be needed-

    mask - This will tell whether we explore this path or not. (1 for visited & 0 for not visited).
    node - This will tell the current node we are exploring
    cost - This will tell the toatl cost used in the path.
    Set data structure - It tells us which path we have already visited. It warn us to not to travel the repeative paths.
    How mask will help us-

    Let's take an example to understand it better.
    Suppose we visited a node, say 2, then we represent it using our bitmask. How?

    We make a binary value like this  1 0 0
    here, we make 1 to our position 2, it says that we have visited our node present on position 2.
    Since binary numbers read from right to left, so position is like this.

                                ←
            position :       2 1 0
    we have binary value     1 0 0
                             ↑
                            Now, how we will reach to 100
                            if we change 100 to decimal then it comes out to be '4'
                            And 4 can be represented as 2 ^ 2 (read it as, power of 2 of node 2)
                            we have node 2, and when we make power of node then it changes to our mask
                _________________

    Similiarly, if we say we have visited node 3,
    then we represent our binary value like this 1 0 0 0
    here, we make 1 to our position 3, it says that we have visited the node.

                                ←
            position :       3 2 1 0
    we have binary value     1 0 0 0
                             ↑
                            Now, how we will reach to 1000
                            if we change 1000 to decimal then it comes out to be '8'
                            And 8 can be represented as 2 ^ 3 (read it as, power of 2 of node 3)
                            we have node 3, and when we make power of node then it changes to our mask
    Some Important points to understand from code-

    Now, we know how we know how we represent our nodes to mask but since we are exploring paths,
    then we should also know when we travelling from a node to anthor node, then how our mask will change.

    Let's take an example for better understanding.
    Suppose we are standing on node 2 then in bit it is represented as (100) see above
    And also let say we travel from our node 2 to node 3.
    We also know, if we visit our node 3 then in bit it should be represented as (1000) see above

    So, How we will make changes in our bit, so that we can say that okay if we travel from node 2 to node 3
    then according to this our bit changes.

    So see, by using bit what we are trying to say, we are trying to say that we make 1 to the position of node.
    means if visited node 2 then we make 1 to our position 2 i.e 100
    and also if we visited our node 3 then we make 1 to our position 3 i.e 1000

    but when we travel from node 2 to node 3, what we will say?
    We will say since we are standing on our node 2,then definatiely our position 2 is to be 1
    But also since we now visit to our node 3 then definately we have to set our position 3 i.e to make it 1.

    So, if we represent this, then how it should looks like.

                                ←
            position :        3 2 1 0
    we have binary value      1 1 0 0

    See, on our position 2, we have 1
    and also on position 3, we have 1.
    which says that we visited our node 2 as well as our node 3 also

    Now, we have mask of node 2 i.e 0100
    Ans, We have mask of node 3 i.e 1000

    And we want our final answer as 1100

    So, Can we relate them using some mathematical type thing?
    Answer is YES.
    If we take  BITWISE OR of 0100 and 1000 then we get our result as
                        0100
                        1000
                        ____
                        1100
                        ____
                        since property of OR is
                        1 | 0 --> 1
                        0 | 1 --> 1

    So, In this way we change our mask.
    Anthor Some Importants to be noted-

    Since, We don't not know on starting which node we get our minimum path so intially for nodes we store mask and values in our queue.
    We make a set of pair type that will tell us which path we have been already visited.
    Since we have to take care three values at a time, so we will make an class for that.
    We can also use pair of pair to do the same thing, Here I use class.
    If we visited all nodes then what should our mask value look like.
    Suppose we have three nodes, then our mask should looks like

                        ←
            position :       2 1 0
    we have binary value      1 1 1

    111 represents we have visited our each node,
    now if we convert 111 to decimal then it comes out to be 7 i.e (2 ^ 3 - 1) (read it as power of 2 of three nodes minus 1)
                                                                        ↑
                                                                        It represents number of nodes we have

    So, If we visit all the nodes then at point our mask should be equal to
    all = ({power of 2 [total number of nodes]} - 1)
    suppose if there are n nodes, then mask for visiting all nodes looks like
    all = (2 ^ n ) - 1

*/

#define MAX 10000

typedef struct tuple_ {
    int node; // on which current node we are standing
    int mask; // mask of that node
    int cost; // cost of path explore by this node
} tuple;

tuple *
getTuple (int node, int mask, int cost)
{
    tuple *t = calloc (1, sizeof (tuple));
    t->node = node;
    t->mask = mask;
    t->cost = cost;
    return t;
}

typedef struct queue_ {
    tuple *queue[MAX];
    int front;
    int rear;
} QUEUE;


void
initializeQueue (QUEUE *q)
{
    q->rear = -1;
    q->front = -1;
}

bool
isQueueEmpty (QUEUE *q)
{
    if (q->front == -1 || q->front == q->rear + 1) {
        return true;
    } else {
        return false;
    }
}

bool
isQueueFull (QUEUE *q)
{
    if (q->rear == MAX - 1) {
        return true;
    } else {
        return false;
    }
}

void
enqueue (QUEUE *q, tuple *x)
{
    if (isQueueFull (q)) {
        printf ("Queue Overflow\n");
        return;
    }

    if (q->front == -1) {
        q->front = 0;
    }

    q->rear = q->rear + 1;
    q->queue[q->rear] = x;
}

tuple *
dequeue (QUEUE *q)
{
    int x;

    if (isQueueEmpty (q)) {
        printf ("Queue Underflow\n");
        return -1;
    }

    x = q->queue[q->front];
    q->front = q->front + 1;
    return x;
}

typedef struct pair_ {
    int src;
    int mask;
} pair;


int
shortestPathLength (int **graph, int graphSize, int *graphColSize)
{
    // total number of nodes present
    int n = graphSize; // extracting size of graph
    QUEUE q;
    initializeQueue (&q);
    // set to take care which path we have already visited
    int pos = 0;
    pair vis[1000];
    memset (vis, 0, sizeof (vis));
    int all = (1 << n) - 1; // if all nodes visited then

    // we don't know which node will give us shortest path so intially for all nodes we will store in our queue
    for (int i = 0; i < n; i++) {
        int maskValue = (1 << i); // 2 ^ i
        // make a tuple for every nod & push tuple into our queue
        enqueue (&q, getTuple (i, maskValue, 1));
        vis[pos].src = i;
        vis[pos].mask = maskValue;
        pos++;
    }

    // Implementing BFS
    while (isQueueEmpty (&q) == false) { // until queue is not empty
        tuple *curr = dequeue (&q); // extracting front tuple

        // if mask value becomes all, that means we have visited all of our nodes, so from here return cost - 1
        if (curr->mask == all) {
            return curr->cost - 1;
        }

        // if not, start exploring in its adjcant
        for (int i = 0; i < graphSize; i++) {
            for (int j = 0; j < (* graphColSize); j++) {
                if (0 == graph[i][j]) {
                    continue;
                }

                int bothVisitedMask = curr->mask; // current mask
                // we are moving from one node o anthor node
                bothVisitedMask = bothVisitedMask | (1 << j);
                // if this path is not explored i.e
                // if it is not present in our set then,
                bool found = false;

                for (int k = 0; k < pos; k++) {
                    if ((vis[k].src == j) && (vis[k].mask == bothVisitedMask)) {
                        found = true;
                    }
                }

                if (false == found) {
                    vis[pos].src = j;
                    vis[pos].mask = bothVisitedMask;
                    pos++;
                    enqueue (&q, getTuple (j, bothVisitedMask, curr->cost + 1)); // also insert into queue
                }
            }
        }
    }

    // return -1, but this condition will never come
    return -1;
}