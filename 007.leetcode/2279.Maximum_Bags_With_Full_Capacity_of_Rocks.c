#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int
cmp (void *a, void *b)
{
    int x = * (int *)a;
    int y = * (int *)b ;
    return (x - y);
}

int
maximumBags (int *capacity, int capacitySize, int *rocks, int rocksSize, int additionalRocks)
{
    int bags = 0;
    int space[capacitySize];

    for (int i = 0; i < capacitySize; i++) {
        space[i] = capacity[i] - rocks[i];
    }

    qsort (space, capacitySize, sizeof (int), cmp);

    for (int i = 0; i < capacitySize ; i++) {
        if (space[i] == 0) {
            bags++;
            continue;
        } else {
            if (additionalRocks >= space[i]) {
                additionalRocks -= space[i];
                bags++;
            } else {
                break;
            }
        }
    }

    return bags;
}