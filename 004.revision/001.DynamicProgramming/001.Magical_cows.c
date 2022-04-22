#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// https://open.kattis.com/problems/magicalcows
// https://www.youtube.com/watch?v=_tur2nPkIKo&list=PLDV1Zeh2NRsAsbafOroUBnNV8fhZa7P4u&ab_channel=WilliamFiset

// The maximum number of days.
#define MAX_DAYS  50

void solve(int N, int C, int M, int initial_cows[], int schedule[])
{
    int lastDay = schedule[M-1];

    int cowDB[lastDay+1][C+1];
    memset(cowDB, 0, sizeof(cowDB));

    // Count the initial frequency of farms of different sizes
    for (int i = 0; i < N; i++){
        cowDB[0][initial_cows[i]]++;
    }

    printf("Initial DP : \r\n");
    for(int i = 0; i <= lastDay; i++) {
        for(int j = 0; j <= C; j++) {
            printf("%3d", cowDB[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

     for (int day = 0; day < lastDay; day++) {

        // For all farm sizes between 1 and `C`, double the number of cows.
        for (int i = 1; i <= C; i++) {

            if (i <= C / 2) {

                // Cow count on farm with size `i` doubled, but the number of farms did not.
                cowDB[day + 1][i * 2] += cowDB[day][i];
            } else {

                // The number of cows per farm on the farm with size `i` exceeds the
                // permitted limit, so double the number of farms.
                cowDB[day + 1][i] += 2 * cowDB[day][i];
            }
        }
    }

    printf("Initial DP : \r\n");
    for(int i = 0; i <= lastDay; i++) {
        for(int j = 0; j <= C; j++) {
            printf("%10d", cowDB[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    // Answer each query
    for (int day = 0; day < M; day++) {

        int sum = 0;
        for (int i = 1; i <= C; i++){
            sum += cowDB[day][i];
        }

        printf("Day : %d, Number of Cows : %d \r\n", day, sum);
    }

    return;
}


int main ()
{
    // The maximum number of cows on a farm
    int C = 2;

    // The initial number of farms
    int N = 5;

    // The number of queries
    int M = 3;

    // Inital cows in the farm
    int initial_cows[] = {1, 2, 1, 2, 1};

    // regulator visit to the farm
    int schedule[] = {0, 1, 2};

    solve(N, C, M, initial_cows, schedule);

    return 0;
}