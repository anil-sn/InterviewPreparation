#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void swap (char *a, char *b){
    char temp = *a;
    *a = *b;
    *b = temp;
}

void combiUtil(char name[], int l, int r)
{
    if (l >= r){
        printf("%s\r\n", name);
        return;
    }

    for (int i = l; i <= r; i++){
        swap(&(name[l]), &(name[i]));
        combiUtil(name, l+1, r);
        swap(&(name[l]), &(name[i]));
    }

    return;
}


void combi(char name[])
{
    int n = strlen(name);
    return (combiUtil(name, 0, n-1));
}

void permuteUtil(int perm[], int n, int k, int depth, int used[], int ans[], int *pos)
{
    if (depth == k){
        for (int j = 0; j < (*pos); j++){
            printf("%2d ", ans[j]);
        }
        printf("\r\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (0 == used[i]) {
            ans[(*pos)++] = perm[i];
            used[i] = 1;

            permuteUtil(perm, n, k, depth + 1, used, ans, pos);
            used[i] = 0;
            ans[(*pos)--] = 0;
        }
    }

    return;
}

void perm(int permute[], int n)
{
    int used[n];
    int ans[n];
    int pos;
    memset(used, 0, sizeof(used));
    memset(ans, 0, sizeof(ans));

    return (permuteUtil(permute, n, n, 0, used, ans, &pos));
}

int main() {
    char string[] = "ANIL";
    int permute[] = {1, 2, 3, 4};
    int n = sizeof(permute)/sizeof(permute[0]);

    (void)combi(string);
    (void)perm(permute, n);
    return 0;
}