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
    Problem
    There are 'n' ants on a 'n+1' length rod. The ants are numbered from 1 to n and are initially placed at positions starting from position 1 till position n. They are moving either in left direction (denoted by '-1') or in the right direction (denoted by '1'). Whenever an ant crosses the boundary of the rod it falls off the rod. You are given the initial direction of the ants. Now, whenever two ants collide their direction switches, i.e. the ant going in left direction ('-1) changes it's direction towards right ('1') and the ant going in the right direction ('1') changes it's direction towards left ('-1'). Find last ant to fall off the rod.

    Note: In case two ants are falling simultaneously in the end print the index of the lower indexed ant.

    PARKING PROBLEM Problem Code: UEMCOSA
    Add problem to Todo list
    Submit (Practice)
    There are 'n' cars on a 'n+1' length parking track. The cars are numbered from '1' to 'n' and are initially placed at positions starting from position 1 till position n.

    They are moving either in the left direction (denoted by '-1') or in the right direction (denoted by '1'). Whenever an car crosses the boundary of the parking track it goes out of the parking track. You are given the initial direction of the cars. Now, whenever two cars collide their direction switches, i.e. the car going in the left direction ('-1) changes its direction towards the right ('1') and the car going in the right direction ('1') changes its direction towards left ('-1'). Find the last car to fall off or go out of the parking track.

    Note: In case two cars are going out simultaneously in the end print the index of the lower indexed car.
*/

#include <stdio.h>
int
main()
{
    int a[] = {1, -1, 1, -1, 1};
    int n = sizeof (a) / sizeof (a[0]);
    int i;

    int left = 0, right = 0;

    // Number of contiguous ants at left edge and moving towards left
    for (i = 0; i < n; i++) {
        if (a[i] == -1) {
            left ++;
        } else {
            break;
        }
    }

    for (i = n - 1; i >= 0; i--) {
        if (a[i] == 1) {
            right++;
        } else {
            break;
        }
    }

    // Number of ants between 'left' and 'right' position and moving towards left
    int leftgoingants = 0;

    for (i = left; i <= n - right - 1; i++) {
        if (a[i] == -1) {
            leftgoingants ++;
        }
    }

    if (left >= right) {
        printf ("%d", left + leftgoingants);
    } else {
        printf ("%d", left + leftgoingants + 1);
    }

    return 0;
}