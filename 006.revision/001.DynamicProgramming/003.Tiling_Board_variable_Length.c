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
// https://youtu.be/L1x3an2pl3U


int solve (int length,  int noOfTiles, int tiles[], int maxTileSize)
{
    int frequency[maxTileSize];
    memset(frequency, 0, sizeof(frequency));

    int dp[length+1];
    memset(dp, 0, sizeof(dp));

    dp[0] = 1;

    for (int i = 0; i < noOfTiles; i++){
        frequency[tiles[i]]++;
    }

    for (int i = 1; i <= length; i++) {
        for (int tile = 0; tile < maxTileSize; tile++) {
            if ((0 == frequency[tile]) || (frequency[tile] > i)) {
                continue;
            }
            if ((i - tile) >= 0) {
                dp[i] += dp[i - tile] * frequency[tile];
            }
        }
    }

    return dp[length];
}

int main ()
{
    int length = 6;
    int tiles[] = {1, 1, 2, 4};
    int n = sizeof(tiles)/sizeof(tiles[0]);
    int maxTileSize = 4;

    int ways = solve(length, n, tiles, maxTileSize);
    printf("Number of ways board of size %d can be places with variable size tiles : %d \r\n", length, ways);

    return 0;
}