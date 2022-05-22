/*
43. Multiply Strings

Given two non-negative integers num1 and num2 represented as strings, return the product of num1 and num2.

Note:

The length of both num1 and num2 is < 110.
Both num1 and num2 contains only digits 0-9.
Both num1 and num2 does not contain any leading zero.
You must not use any built-in BigInteger library or convert the inputs to integer directly.
*/
char *
multiply (char *num1, char *num2)
{
    if (num1[0] == '0' || num2[0] == '0') {
        return "0";
    }

    int n, m;
    n = strlen (num1);
    m = strlen (num2);
    char *ans = (char *)calloc (n + m + 1, sizeof (char));

    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            int k = i + j + 1;
            int mul = (num1[i] - '0') * (num2[j] - '0');

            while (mul) {
                mul += ans[k];
                ans[k] = (mul) % 10;
                mul = mul / 10;
                k--;
            }
        }
    }

    for (int i = 0 ; i < m + n ; i++) {
        ans[i] += '0';
    }

    if (ans[0] == '0') {
        return ans + 1;
    }

    return ans;
}

/*
Difficulty:Medium
Total Accepted:107.8K
Total Submissions:398.6K


Companies Facebook Twitter
Related Topics Math String
Similar Questions


                    Add Two Numbers

                    Plus One

                    Add Binary

                    Add Strings
*/
