#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

// https://www.geeksforgeeks.org/flood-fill-algorithm/

#define MAX 100

typedef struct cell_ {
    int x;
    int y;
} CELL;

typedef struct Queue_ {
    CELL * queue[MAX];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

CELL * newCell(int x, int y)
{
    CELL *cell = malloc(sizeof(CELL));
    cell->x = x;
    cell->y = y;

    return cell;
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

void enQueue(QUEUE *q, CELL * val)
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
    //printf("Element %d,%d is enqued \r\n", val->x, val->y);
    q->currSize++;
}

CELL * deQueue(QUEUE *q)
{
    CELL * val;
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

// Function that returns true if the given pixel is valid
bool isValid(int screen[8][8], int m, int n, int x, int y, int prevC, int newC)
{
    if(x < 0 ||
       x >= m ||
       y < 0 ||
       y >= n ||
       screen[x][y] != prevC
       || screen[x][y]== newC) {
        return false;
    }

    return true;
}


// FloodFill function
void floodFill(int screen[][8], int m, int n, int x, int y, int prevC, int newC)
{
    QUEUE *q = initQueue();

    CELL *p = newCell(x, y);
    enQueue(q, p);
    screen[x][y] = newC;

    while(false == isQueueEmpty(q)) {

        // Dequeue the front node
        CELL *currPixel = deQueue(q);

        int posX = currPixel->x;
        int posY = currPixel->y;

        // Check if the adjacent pixels are valid
        if(isValid(screen, m, n, posX + 1, posY, prevC, newC))
        {
            // Color with newC
            screen[posX + 1][posY] = newC;
            p = newCell(posX + 1, posY);
            enQueue(q, p);
        }

        if(isValid(screen, m, n, posX-1, posY, prevC, newC))
        {
            screen[posX - 1][posY] = newC;
            p = newCell(posX - 1, posY);
            enQueue(q, p);
        }

        if(isValid(screen, m, n, posX, posY + 1, prevC, newC))
        {
            screen[posX][posY + 1] = newC;
            p = newCell(posX, posY + 1);
            enQueue(q, p);
        }

        if(isValid(screen, m, n, posX, posY-1, prevC, newC))
        {
            screen[posX][posY - 1] = newC;
            p = newCell(posX, posY - 1);
            enQueue(q, p);
        }
    }

    return;
}

int main()
{
    int screen[8][8] ={
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0},
    {1, 0, 0, 1, 1, 0, 1, 1},
    {1, 2, 2, 2, 2, 0, 1, 0},
    {1, 1, 1, 2, 2, 0, 1, 0},
    {1, 1, 1, 2, 2, 2, 2, 0},
    {1, 1, 1, 1, 1, 2, 1, 1},
    {1, 1, 1, 1, 1, 2, 2, 1}};

    int m = 8;
    int n = 8;

    // Co-ordinate provided by the user
    int x = 4;
    int y = 4;

    // Current color at that co-ordinate
    int prevC = screen[x][y];

    // New color that has to be filled
    int newC = 3;

    printf("Printing the current screen \r\n");
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%2d", screen[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    floodFill(screen, m, n, x, y, prevC, newC);

    printf("Printing the updated screen \r\n");
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%2d", screen[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    return 0;
}