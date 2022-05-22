/*
3. Longest Substring Without Repeating Characters

Given a string, find the length of the longest substring without repeating characters.

Examples:

Given "abcabcbb", the answer is "abc", which the length is 3.

Given "bbbbb", the answer is "b", with the length of 1.

Given "pwwkew", the answer is "wke", with the length of 3. Note that the answer must be a substring, "pwke" is a subsequence and not a substring.
*/

int
lengthOfLongestSubstring (char *s)
{
    char c;
    int seen[128] = { 0 };
    int right = 0;
    int left = 0;
    int longest = 0;

    for (int i = 0; s[i]; i++) {
        c = s[i];
        left = i - seen[c] + 1;
        seen[c] = i + 1;
        right ++;
        right = right < left ? right : left;
        longest = longest > right ? longest : right;
    }

    return longest;
}


/*
Difficulty:Medium
Total Accepted:330.3K
Total Submissions:1.4M


Companies Amazon Adobe Bloomberg Yelp
Related Topics Hash Table Two Pointers String
Similar Questions


                    Longest Substring with At Most Two Distinct Characters
*/
