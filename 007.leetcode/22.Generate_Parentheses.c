/*
22. Generate Parentheses

Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.



For example, given n = 3, a solution set is:

[
  "((()))",
  "(()())",
  "(())()",
  "()(())",
  "()()()"
]
*/

/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
char
pop_back (char *str)
{
    // usually NULL char is at Nth location in [0..n-1] array
    //e.g. "Google" Length = 6 i.e. 0..5
    //Extra char at 7th location means [N+1]
    // since we want to remove last char just subtract 1 and put '\0
    char ch = (str)[strlen (str) - 1];
    (str)[strlen (str) - 1] = '\0';
    return ch;
}

void
push_back (char *str, char *addChStr)
{
    strcat ((str), addChStr);
}

void
generateAll (int open, int close, int n, char ***list, int *listSize, char *str)
{
    if (open == n && close == n) {
        int len = strlen (str);
        int index = *listSize + 1;
        *list = (char **)realloc (*list, ((index) * sizeof (char *)));
        (*list)[*listSize] = (char *)malloc ((len + 1) * sizeof (char));
        strcpy ((*list)[*listSize], str);
        *listSize = *listSize + 1;
        return;
    }

    if (open < n) {
        open += 1;
        push_back (str, "(");
        generateAll (open, close, n, list, listSize, str);
        open -= 1;
        pop_back (str);
    }

    if (close < open) {
        close += 1;
        push_back (str, ")");
        generateAll (open, close, n, list, listSize, str);
        close -= 1;
        pop_back (str);
    }
}


char **
generateParenthesis (int n, int *returnSize)
{
    char **list = NULL;
    char *initStr = (char *)calloc (2 * n + 1, sizeof (char));
    *returnSize = 0;
    // open = 0, close = 0, N = n , list to retrun , return size and a temprary string for tracking
    //
    generateAll (0, 0, n, &list, returnSize, initStr);
    free (initStr);
    return list;
}

/*
Difficulty:Medium
Total Accepted:157.4K
Total Submissions:349.7K


Companies Google Uber Zenefits
Related Topics Backtracking String
Similar Questions


                    Letter Combinations of a Phone Number

                    Valid Parentheses
*/
