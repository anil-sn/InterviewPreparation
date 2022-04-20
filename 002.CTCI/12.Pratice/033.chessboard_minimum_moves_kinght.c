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
    Given a square chessboard of N x N size, the position of Knight
    and position of a target is given, the task is to find out the minimum
    steps a Knight will take to reach the target position.

    ┌───┬───┬───┬───┬───┬───┬───┬───┐
    │ ♜ │ ♞ │ ♝ │ ♛ │ ♚ │ ♝ │ ♞ │ ♜ │
    ├───┼───┼───┼───┼───┼───┼───┼───┤
    │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │
    ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │
    ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │
    ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │
    ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │
    ├───┼───┼───┼───┼───┼───┼───┼───┤
    │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │
    ├───┼───┼───┼───┼───┼───┼───┼───┤
    │ ♖ │ ♘ │ ♗ │ ♕ │ ♔ │ ♗ │ ♘ │ ♖ │
    └───┴───┴───┴───┴───┴───┴───┴───┘
*/
typedef struct Node {
    int x, y;
    int dis;
	struct Node *next;
}CELL;

typedef struct Queue {
	int size;
	int max_size;
	struct Node *head;
	struct Node *tail;
}QUEUE;

struct Queue* newQueue(int capacity)
{
	struct Queue *q;
	q = malloc(sizeof(struct Queue));

	if (q == NULL) {
		return q;
	}
	q->size = 0;
	q->max_size = capacity;
	q->head = NULL;
	q->tail = NULL;

	return q;
}

int enqueue(struct Queue *q, int x, int y, int dist)
{
	if ((q->size + 1) > q->max_size) {
		return q->size;
	}

	struct Node *node = malloc(sizeof(struct Node));
	if (node == NULL) {
		return q->size;
	}

    node->x = x;
    node->y = y;
    node->dis = dist;
	node->next = NULL;

	if (q->head == NULL) {
		q->head = node;
		q->tail = node;
		q->size = 1;

		return q->size;
	}


	q->tail->next = node;
	q->tail = node;
	q->size += 1;

	return q->size;
}

void dequeue(struct Queue *q, int *x, int *y, int *dist)
{
	if (q->size == 0) {
		return;
	}

	void *value = NULL;
	struct Node *tmp = NULL;

	*x = q->head->x;
    *y = q->head->y;
    *dist = q->head->dis;

	tmp = q->head;
	q->head = q->head->next;
	q->size -= 1;

	free(tmp);
	return;
}

bool isQueueEmpty(struct Queue *q)
{
	if (q->size == 0) {
		return true;
	}
    return false;
}

void freeQueue(struct Queue *q)
{
	if (q == NULL) {
		return;
	}

	while (q->head != NULL) {
		struct Node *tmp = q->head;
		q->head = q->head->next;
		free(tmp);
	}

	if (q->tail != NULL) {
		free(q->tail);
	}

	free (q);
}

// Utility method returns true if (x, y) lies
// inside Board
bool isInside(int x, int y, int N)
{
    if (x >= 0 && x <= N && y >= 0 && y <= N)
        return true;
    return false;
}

// Method returns minimum step
// to reach target position
int minStepToReachTarget( int knightPos[], int targetPos[], int N)
{
    int A, B;
    int dist;

    // x and y direction, where a knight can move
    int dx[] = { -2, -1, 1, 2, -2, -1, 1, 2 };
    int dy[] = { -1, -2, -2, -1, 1, 2, 2, 1 };

    // queue for storing states of knight in board
    QUEUE *q = newQueue(100);

    // push starting position of knight with 0 distance
    enqueue(q, knightPos[0], knightPos[1], 0);

    CELL t;
    int x, y;
    bool visit[N + 1][N + 1];
    int debugDist[N + 1][N + 1];

    // make all cell unvisited
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            visit[i][j] = false;
            debugDist[i][j] = 0xFF;
        }
    }

    // visit starting state
    visit[knightPos[0]][knightPos[1]] = true;
    debugDist[knightPos[0]][knightPos[1]] = 0;

    // loop until we have one element in queue
    while (false == isQueueEmpty(q)) {
        dequeue(q, &A, &B, &dist);

        // Debug visited
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                printf("%3d:%3d\t", visit[i][j], debugDist[i][j]);
            }
            printf("\r\n");
        }
        printf("\r\n\r\n");

        // if current cell is equal to target cell,
        // return its distance
        if (A == targetPos[0] && B == targetPos[1]) {
            return dist;
        }

        // loop for all reachable states
        for (int i = 0; i < 8; i++) {
            x = A + dx[i];
            y = B + dy[i];

            // If reachable state is not yet visited and
            // inside board, push that state into queue
            if (isInside(x, y, N) && !visit[x][y]) {
                visit[x][y] = true;
                debugDist[x][y] = dist + 1;
                enqueue(q, x, y, dist + 1);
            }
        }
    }
}

// Driver code to test above methods
int main()
{
    int N = 8;
    int knightPos[] = { 4, 5 };
    int targetPos[] = { 1, 1 };
    int ans = minStepToReachTarget(knightPos, targetPos, N);
    printf("\r\n Minimum Number of Moves for Night (%d,%d) to Dest (%d,%d) is %d\r\n",
            knightPos[0],
            knightPos[1],
            targetPos[0],
            targetPos[1],
            ans);

    printf ("\r\n"
    "    0   1   2   3   4   5   6   7  \r\n"
    "  ┌───┬───┬───┬───┬───┬───┬───┬───┐\r\n"
    "0 │   │   │   │ ○ │   │   │   │   │\r\n"
    "  ├───┼───┼───┼───┼───┼───┼───┼───┤\r\n"
    "1 │   │ ● │   │   │   │   │   │   │\r\n"
    "  ├───┼───┼───┼───┼───┼───┼───┼───┤\r\n"
    "2 │   │   │   │   │ ○ │   │   │   │\r\n"
    "  ├───┼───┼───┼───┼───┼───┼───┼───┤\r\n"
    "3 │   │   │ ○ │   │   │   │   │   │\r\n"
    "  ├───┼───┼───┼───┼───┼───┼───┼───┤\r\n"
    "4 │   │   │   │   │   │ ♞ │   │   │\r\n"
    "  ├───┼───┼───┼───┼───┼───┼───┼───┤\r\n"
    "5 │   │   │   │   │   │   │   │   │\r\n"
    "  ├───┼───┼───┼───┼───┼───┼───┼───┤\r\n"
    "6 │   │   │   │   │   │   │   │   │\r\n"
    "  ├───┼───┼───┼───┼───┼───┼───┼───┤\r\n"
    "7 │   │   │   │   │   │   │   │   │\r\n"
    "  └───┴───┴───┴───┴───┴───┴───┴───┘\r\n"
    "\r\n");

    return 0;
}