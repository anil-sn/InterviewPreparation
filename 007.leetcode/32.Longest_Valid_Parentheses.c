/*
32. Longest Valid Parentheses

Given a string containing just the characters '(' and ')', find the length of the longest valid (well-formed) parentheses substring.


For "(()", the longest valid parentheses substring is "()", which has length = 2.


Another example is ")()())", where the longest valid parentheses substring is "()()", which has length = 4.
*/

int
longestValidParentheses (char *s)
{
    int len = strlen (s);
    int *stack = (int *)malloc (len * sizeof (int));
    int top = -1;
    int longest = 0;
    int last = -1;

    for (int i = 0; i < len; i++) {
        if (s[i] == ')') {
            if (top < 0) {
                last = i;
            } else {
                top--;

                if (top < 0) {
                    if (i - last > longest) {
                        longest = i - last;
                    }
                } else if (i - stack[top] > longest) {
                    longest = i - stack[top];
                }
            }
        } else if (s[i] == '(') {
            top++;
            stack[top] = i;
        }
    }

    return longest;
}

int
longestValidParentheses (char *s)
{
    int i, l, n, *v;
    int max;

    if (!s) {
        return 0;
    }

    l = strlen (s);
    v = malloc (l * sizeof (int));
    //assert(v);
    n = 0;

    for (i = 0; i < l; i ++) {
        if (s[i] == '(') {
            v[i] = 1;
            n ++;
        } else { /*if (s[i] == ')')*/
            if (n > 0) {
                n --;
                v[i] = 1;
            } else {
                v[i] = 0;
            }
        }
    }

    n = 0;

    for (i = l - 1; i >= 0; i --) {
        if (s[i] == ')') {
            if (v[i]) {
                n ++;
            }
        } else { /*if (s[i] == '(')*/
            if (n > 0) {
                n --;
            } else {
                v[i] = 0;
            }
        }
    }

    n = 0;
    max = 0;

    for (i = 1; i < l; i ++) {
        if (v[i]) {
            v[i] = v[i - 1] + 1;

            if (max < v[i]) {
                max = v[i];
            }
        }
    }

    free (v);
    return max;
}

int
longestValidParentheses (char *s)
{
    if (!*s) {
        return 0;
    }

    for (int i = 0, j = 1; s[j]; j++) { //Step 1: replace valid parentheses with 1. Like "((()()(()" -> "((1111(11"
        while (s[i] == 1 && i > 0) {
            i--;
        }

        while (s[j] == 1) {
            j++;
        }

        if (s[i] == '(' && s[j] == ')') {
            s[i] = 1;
            s[j] = 1;

            if (i != 0) {
                i--;
            }
        } else {
            i = j;
        }
    }

    int max = 0;

    for (int i = 0, local = 0; s[i]; i++, local = 0) { //Step 2: count it
        while (s[i] == 1) {
            i++;
            local++;
        }

        if (local > max) {
            max = local;
        }

        if (!s[i]) {
            break;
        }
    }

    return max;
}

/*
Difficulty:Hard
Total Accepted:100.6K
Total Submissions:436.1K


Related Topics Dynamic Programming String
Similar Questions Valid Parentheses

*/
