/*
38. Count and Say

The count-and-say sequence is the sequence of integers with the first five terms as following:
1.     1
2.     11
3.     21
4.     1211
5.     111221



1 is read off as "one 1" or 11.
11 is read off as "two 1s" or 21.
21 is read off as "one 2, then one 1" or 1211.



Given an integer n, generate the nth term of the count-and-say sequence.



Note: Each term of the sequence of integers will be represented as a string.


Example 1:
Input: 1
Output: "1"



Example 2:
Input: 4
Output: "1211"

When I first read the question, I don't really understand what is this question meaning. Actually it is very simple.
Everthing start 1, 1 is 1, nothing else.
Then we go to the next one. If we wanna know this result, we need to count the previous result, which is "1". That's why the title of the question is "Count and Say". The last result is "1", just one "1", so this result is "11"
Now we keep going, the last result is "11", so there are 2 "1" in the result, so this time the result is "21".
For this time, we know last result is "21", which means there are one "2" and one "1", the result is "1211"
....
Now we do a test, if the (n-1)th result is "12212333312111238"(I typed it randomly), what is the n th result? Let's count it. one "1", two "2", one "1" , one "2" , four"3" , one "1" , one "2", three"1", one "2", one"3" and one "8".Therefore, the result is "1122111243111231121318"
Obviously, if we wanna know the n th result, we just need to count the (n - 1)th result, and the first result is "1". No explanation.

*/

#define BUF_SIZE 100 * 1024

char *
countAndSay (int n)
{
    char *s, *d, *x;
    int i, c;

    if (!n) {
        return NULL;
    }

    s = malloc (BUF_SIZE);
    x = d = malloc (BUF_SIZE);
    s[0] = '1';
    s[1] = 0;

    while (n > 1) {
        c = 1;

        for (i = 0; s[i]; i ++) {
            if (s[i] != s[i + 1]) {
                sprintf (x, "%d%c", c, s[i]);
                x += strlen (x);
                c = 1;
            } else {
                c ++;
            }
        }

        x = s;
        s = d;
        d = x;
        n --;
    }

    free (d);
    return s;
}


/*
Difficulty:Easy
Total Accepted:145.1K
Total Submissions:418.7K


Companies Facebook
Related Topics String
Similar Questions


                    Encode and Decode Strings
*/
