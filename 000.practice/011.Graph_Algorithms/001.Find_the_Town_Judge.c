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
    In a town, there are n people labeled from 1 to n.
    There is a rumor that one of these people is secretly
    the town judge.

    If the town judge exists, then:

    The town judge trusts nobody.
    Everybody (except for the town judge) trusts the town judge.
    There is exactly one person that satisfies properties 1 and 2.

    You are given an array trust where trust[i] = [ai, bi] representing
    that the person labeled ai trusts the person labeled bi.

    Return the label of the town judge if the town judge exists and can be identified, or return -1 otherwise.

    Example 1:

    Input: n = 2, trust = [[1,2]]
    Output: 2
    Example 2:

    Input: n = 3, trust = [[1,3],[2,3]]
    Output: 3
    Example 3:

    Input: n = 3, trust = [[1,3],[2,3],[3,1]]
    Output: -1


    Constraints:

    1 <= n <= 1000
    0 <= trust.length <= 104
    trust[i].length == 2
    All the pairs of trust are unique.
    ai != bi
    1 <= ai, bi <= n
*/

int findJudge(int totalPeople, int trust[][2], int trustSize) {

    int degreeTrust[totalPeople + 1];

    for (int i = 0; i < trustSize; i++) {
        degreeTrust[trust[i][0]]--;
        degreeTrust[trust[i][1]]++;
    }

    for (int person = 1; person <= totalPeople; person++) {
        if (degreeTrust[person] == totalPeople - 1) {
            return person;
        }
    }

    return -1;
}

int main ()
{
    int trust[][2] = {{1,3}, {1,4}, {2,3}, {2,4}, {4,3}};
    int trustSize = sizeof(trust)/sizeof(trust[0]);
    int totalPeople = 4;
    int judge = findJudge (totalPeople, trust, trustSize);
    printf ("The Judge is %d \r\n", judge);
}

