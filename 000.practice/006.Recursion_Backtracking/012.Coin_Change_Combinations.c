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
    1. You are given a number n, representing the count of coins.
    2. You are given n numbers, representing the denominations of n coins.
    3. You are given a number "amt".
    4. You are required to calculate and print the combinations of the n coins (non-duplicate) using
        which the amount "amt" can be paid.

    Note -> Use the code snippet and follow the algorithm discussed in question video. The judge can't
                force you but the intention is to teach a concept. Play in spirit of the question.
*/


void coinsChange(int coins[], int count, int amount, int combination[100]) {
    if (cs > ts) {
        printf ("\r\n");
        return;
    }

    for (int i = lc + 1; i < strlen(ustr); i++) {
        printf ("%c", ustr[i]);
        generateSelection(ustr, cs+1, ts, i);
    }
}


int main()
{
    int coins[] = [2, 3, 5, 6, 7];
    int amount = 12;
    int combination[100] = {0};

    coinsChange(coins, count, amount, combination);
}


public static void coinChange(int i, int[] coins, int amtsf, int tamt, String asf){
    if(i == coins.length){
        if(amtsf == tamt){
            System.out.println(asf + ".");
        }
        return;
    }
    coinChange(i + 1, coins, amtsf + coins[i], tamt, asf + coins[i] + "-");
    coinChange(i + 1, coins, amtsf + 0, tamt, asf + "");
}
public static int main(String[] args) throws Exception {
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    int n = Integer.parseInt(br.readLine());
    int[] coins = new int[n];
    for (int i = 0; i < n; i++) {
        coins[i] = Integer.parseInt(br.readLine());
    }
    int amt = Integer.parseInt(br.readLine());

    coinChange(0, coins, 0, amt, "");
}



