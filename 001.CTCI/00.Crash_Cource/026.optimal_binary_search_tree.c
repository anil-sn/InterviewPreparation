#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXSIZE 1000
int data[MAXSIZE];
int n, i;


void total_traversals(int n, int data[]);
int e[1000][1000];
int w[1000][1000];


void total_traversals(int n, int data[]) {
    int j, k, r, trav;

    // e[][] is for edges traversed and w[][] is weight of the edges
    for(i = 0; i < n; i++) {
        e[i][i] = 0;
        w[i][i] = data[i];
    }

    for(k = 1; k <= n; k++) {
        for(i = 0; i <= n - k; i++) {
            j = i + k;

            // initialize e[i][j] with maximum integer value
            e[i][j] = INT_MAX;

            // calculate w[i][j]
            w[i][j] = w[i][j - 1] + data[j];

            // for loop to calculate minimum trav value between i and j
            for(r = i; r < j; r++) {
                trav = e[i][r] + e[r + 1][j] + w[i][j];

                if(trav <= e[i][j]) {
                    e[i][j] = trav;

                }

            }

        }
    }

    // total number of traversals required is given by e[0][n-1]
    printf("%d\n", e[0][n - 1]);
}

int main(int argc, char *argv[]) {
    int i;
    // Total number of search terms
    scanf("%d", &n);

    //accept input
    for (i = 0; i < n; i++) {
        scanf("%d", &data[i]);
    }

    // Find OBST
    total_traversals(n, data);
}
