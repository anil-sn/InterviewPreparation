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
    Leetcode 332: Reconstruct Itinerary

    Given a list of airline tickets represented by pairs of departure and arrival airports
    [from, to], reconstruct the itinerary in order.
    All of the tickets belong to a man who departs from JFK.
    Thus, the itinerary must begin with JFK.

    Note:
    If there are multiple valid itineraries, you should return the itinerary that has
    the smallest lexical order when read as a single string.
    For example, the itinerary ["JFK", "LGA"] has a smaller lexical order than ["JFK", "LGB"].

    All airports are represented by three capital letters (IATA code).
    You may assume all tickets form at least one valid itinerary.
    One must use all the tickets once and only once.

    Example 1:
    Input: [["MUC", "LHR"], ["JFK", "MUC"], ["SFO", "SJC"], ["LHR", "SFO"]]
    Output: ["JFK", "MUC", "LHR", "SFO", "SJC"]

    Example 2:
    Input: [["JFK","SFO"],["JFK","ATL"],["SFO","ATL"],["ATL","JFK"],["ATL","SFO"]]
    Output: ["JFK","ATL","JFK","SFO","ATL","SFO"]
    Explanation: Another possible reconstruction is ["JFK","SFO","ATL","JFK","ATL","SFO"].
                But it is larger in lexical order.

    Problem Analysis:
    For the description mentioned it has at least one valid solution,
    so we can construct the itineraries using dfs (depth first search) algorithm.
    Because there may be multiple valid itineraries, we should construct with smallest
    lexical order.

    Note that don’t worry about loops or repeated airports, as problem description
    tips that it has valid solution, so dfs algorithm can succeed to construct it no
    matter whether it has loops or not.

    Explanation

        First keep going forward until you get stuck. That's a good main path already.
        Remaining tickets form cycles which are found on the way back and get merged into
        that main path. By writing down the path backwards when retreating from recursion,
        merging the cycles into the main path is easy - the end part of the path has already
        been written, the start part of the path hasn't been written yet,
        so just write down the cycle now and then keep backwards-writing the path.

        Example:

                          ┌─────┐
                  ┌───────►  B  ├─────┐
                  │       └─────┘     │
                ┌─┴───┐            ┌──▼──┐
                │  D  ◄────────────┤  C  │
                └──▲─┬┘            └┬──▲─┘
                   │ └──┐           │  │
                   │ ┌──┼───────────┘  │
                   │ │  └────────────┐ │
                ┌──┴─▼┐             ┌▼─┴──┐
                │ JFK ├─────────────►  A  │
                └─────┘             └─────┘

    From JFK we first visit JFK -> A -> C -> D -> A.
    There we're stuck, so we write down A as the end of the route and retreat back to D.
    There we see the unused ticket to B and follow it: D -> B -> C -> JFK -> D.
    Then we're stuck again, retreat and write down the airports while doing so:
    Write down D before the already written A, then JFK before the D, etc.
    When we're back from our cycle at D, the written route is D -> B -> C -> JFK -> D -> A.
    Then we retreat further along the original path, prepending C, A and
    finally JFK to the route, ending up with the route
    JFK -> A -> C -> D -> B -> C -> JFK -> D -> A.

    I think it helps to think about how nature of the problem means that any path will
    deadend at the final destination, and building on that insight leads to better
    understanding this solution.

    Also it should help to see how targets is updating at each step, e.g.,

    targets = { 'JFK'   : ['D', 'A'],
                'A'     : ['C'],
                'B'     : ['C'],
                'C'     : ['JFK', 'D'],
                'D'     : ['B', 'A']}
    route = []
    stack = ['JFK']
    First point at which we get stuck:

    targets = { 'JFK'   : ['D'],
                'A'     : [],
                'B'     : ['C'],
                'C'     : ['JFK', 'D'],
                'D'     : ['B']}
    route = []
    stack = ['JFK', 'A', 'C', 'D', 'A']
    Update route:

    targets = { 'JFK'   : ['D'],
                'A'     : [],
                'B'     : ['C'],
                'C'     : ['JFK'],
                'D'     : ['B']}
    route = ['A']
    stack = ['JFK', 'A', 'C', 'D']
    Search forward again until stuck:

    targets = { 'JFK'   : [],
                'A'     : [],
                'B'     : [],
                'C'     : [],
                'D'     : []}
    route = ['A']
    stack = ['JFK', 'A', 'C', 'D', 'B', 'C', 'JFK', 'D']
    Update route:

    targets = { 'JFK'   : ['D'],
                'A'     : [],
                'B'     : [],
                'C'     : ['JFK'],
                'D'     : []}
    route = ['A', 'D', 'JFK', 'C', 'B', 'D', 'C', 'A', 'JFK']
    stack = []
    Return route in reverse:

    route = ['JFK', 'A', 'C', 'D', 'B', 'C', 'JFK', 'D', 'A']
*/

#define MAXLEN 64

typedef struct ticket {
    char from[MAXLEN];
    char to[MAXLEN];
} TICKET;

typedef struct route {
    char AIRPORT[MAXLEN];
} ROUTE;

void addTicket(char *from, char *to, TICKET tickets[], int pos) {
    sprintf(tickets[pos].from, "%s", from);
    sprintf(tickets[pos].to, "%s", to);
}

int cmp (const void *a, const void *b){
    TICKET *x = (TICKET *)a;
    TICKET *y = (TICKET *)b;
    return (strcmp(x->from, y->from));
}

void visit(char airport[], TICKET tickets[], int n, ROUTE route[], int *pos)
{
    char next[MAXLEN];

    for (int i = 0; i < n; i++) {
        if (0 == strcmp(tickets[i].from, airport)) {
            sprintf(next, "%s", tickets[i].to);
            tickets[i].from[0] = '\0';
            tickets[i].to[0] = '\0';
            visit(next, tickets, n, route, pos);
        }
    }

    sprintf(route[(*pos)++].AIRPORT, "%s", airport);
    return;
}

void
printItinerary(TICKET tickets[], int n) {

    ROUTE route[MAXLEN];
    memset(route, '\0', sizeof(route));

    qsort(tickets, n, sizeof(tickets[0]), cmp);

    for (int i = 0; i < n; i++) {
        printf ("%s -> %s \r\n", tickets[i].from, tickets[i].to);
    }

    int pos = 0;
    char start[MAXLEN];
    sprintf(start, "%s", "JFK");
    visit(start, tickets, n, route, &pos);

    printf ("\r\n The route : ");
    for (int i = pos; i >= 0; i--) {
        printf ("%s ", route[i].AIRPORT);
    }
    printf ("\r\n");

    return;
}

int main () {
    TICKET tickets[8];
    int pos;

    pos = 0;
    addTicket("JFK", "A", tickets, pos++);
    addTicket("JFK", "D", tickets, pos++);
    addTicket("A", "C", tickets, pos++);
    addTicket("B", "C", tickets, pos++);
    addTicket("C", "D", tickets, pos++);
    addTicket("C", "JFK", tickets, pos++);
    addTicket("D", "B", tickets, pos++);
    addTicket("D", "A", tickets, pos++);

    printItinerary(tickets, pos);
}