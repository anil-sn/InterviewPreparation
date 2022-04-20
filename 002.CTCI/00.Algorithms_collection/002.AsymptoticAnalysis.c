/*
 ============================================================================
 Name        : AsymptoticAnalysis.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Towers of Hanoi
// C recursive function to solve tower of hanoi puzzle
static int moves = 0;

void towerOfHanoi(int n, char from_rod, char to_rod, char aux_rod) {
    moves++;

    if (n == 1) {
        printf("\n Move disk 1 from rod %c to rod %c", from_rod, to_rod);
        return;
    }

    towerOfHanoi(n - 1, from_rod, aux_rod, to_rod);
    printf("\n Move disk %d from rod %c to rod %c", n, from_rod, to_rod);
    towerOfHanoi(n - 1, aux_rod, to_rod, from_rod);

}

int main(void) {

    const int n = 15;
    int arr[] = {3, 5, 6, 7, 8, 12, 13, 14, 16, 17, 22, 23, 26, 29, 31};

    /**
     * O(n)
     */
    for (int i = 0; i < n; i++) {
        printf("a[%d] = %d\n", i, arr[i]);
    }

    /**
     * O( n log n )
     */
    int count = 0;

    for (int i = 1; i <= n; i *= 2) {
        printf("i: %d\n", i);

        for (int j = 0; j < n; j++) {
            printf("count: %d\n", count++);
        }
    }

    /**
     * O( 1 )
     */
    if (n > 0) {
        printf("array last n-1 item is %d", arr[n - 1]);
    }

    /**
     * O( 2^n )
     */
    //towerOfHanoi()
    //we use n number of disks
    towerOfHanoi(n - 10, 'A', 'C', 'B'); // A, B and C are names of rods
    printf("\nTotal moves %d", moves);
    return 0;


    return EXIT_SUCCESS;
}
