/*
30. Substring with Concatenation of All Words

You are given a string, s, and a list of words, words, that are all of the same length. Find all starting indices of substring(s) in s that is a concatenation of each word in words exactly once and without any intervening characters.



For example, given:
s: "barfoothefoobarman"
words: ["foo", "bar"]



You should return the indices: [0,9].
(order does not matter).
*/

/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
typedef struct item_s {
    char *word;
    int idx;
    struct item_s *next;
} item_t;

typedef struct {
    item_t *p;
    int n;
} buff_t;

#define HF 1021

unsigned int
hash_code (const char *s, int len)
{
    unsigned int hc = 5381;
    char c;

    while (len -- > 0) {
        hc = hc * 33 + s[len];
    }

    return hc % HF;
}

item_t *
lookup (item_t **ht, unsigned int hc, const char *w, int len)
{
    item_t *p = ht[hc];

    while (p) {
        if (!strncmp (p->word, w, len)) {
            return p;
        }

        p = p->next;
    }

    return NULL;
}

int *
findSubstring (char *s, char **words, int wordsSize, int *returnSize)
{
    // 1. sort all words
    // 2. caculate each uniq words appears how many times
    /* for (i = 0; i < length of string; i ++) {
        substr = s[i ... length of one word]
        if (substr is not one of the words or it appears more than what we have) continue;
        increase the count of this word
        if all words are found, add i into result
    } */
    item_t *ht[HF] = { 0 }, **sp, *p;
    int *counts, *counts2, total, i;
    char *w;
    unsigned int hc;
    int total_len, word_len;
    int left, mid, right;
    int *results;

    if (wordsSize == 0) {
        return NULL;
    }

    buff_t buff = { 0 };
    buff.p = malloc (wordsSize * sizeof (item_t));
    //assert(buff->p);
    counts = calloc (wordsSize * 2, sizeof (int));
    //assert(counts);
    counts2 = &counts[wordsSize];
    total = 0;
    word_len = strlen (words[0]);
    total_len = strlen (s);
    sp = malloc (total_len * sizeof (item_t *));
    //assert(sp);
    results = malloc (total_len * sizeof (int));
    //assert(results);
    *returnSize = 0;

    for (i = 0; i < wordsSize; i ++) {
        w = words[i];
        hc = hash_code (w, word_len);
        p = lookup (ht, hc, w, word_len);

        if (p) {
            counts[p->idx] ++;
        } else {
            p = &buff.p[buff.n];
            p->idx = buff.n ++;
            p->word = w;
            p->next = ht[hc];
            ht[hc] = p;
            counts[p->idx] = 1;
        }
    }

    left = mid = right = 0;

    while (right <= total_len - word_len) {
        w = &s[right];
        hc = hash_code (w, word_len);
        p = lookup (ht, hc, w, word_len);

        if (!p) {
            total = 0;
            memset (counts2, 0, buff.n * sizeof (int)); // reset all counts
            left ++;           // shift one character from left
            mid = left;
            right = left;      // reset right
        } else {
            sp[right] = p;
            i = p->idx;
            counts2[i] ++;
            total ++;

            while (counts2[i] > counts[i]) {
                p = sp[mid];
                mid += word_len;   // push out a word
                counts2[p->idx] --;
                total --;
            }

            if (total == wordsSize) {   // all are found
                results[*returnSize] = mid;
                (*returnSize) ++;
                total = 0;
                memset (counts2, 0, buff.n * sizeof (int)); // reset all counts
                left = mid + 1;
                mid = left;
                right = left;
            } else {
                right += word_len;
            }
        }
    }

    free (buff.p);
    free (counts);
    free (sp);
    return results;
}


typedef struct trie {
    uint16_t count;
    uint16_t used;
    struct trie *t[26];
} Trie;

void
FreeTrie (Trie *trie)
{
    for (int i = 0; i < 26; i++) {
        if (trie->t[i] != NULL) {
            FreeTrie (trie->t[i]);
            free (trie->t[i]);
        }
    }
}

void
ClearUsedTrie (Trie *trie)
{
    // trie->count implies it is a leaf node
    if (!trie->count) {
        for (int i = 0; i < 26; i++) {
            if (trie->t[i] != NULL) {
                ClearUsedTrie (trie->t[i]);
            }
        }
    }

    trie->used = 0;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *
findSubstring (char *s, char **words, int wordsSize, int *returnSize)
{
    int *ret = calloc (1, sizeof (int));
    int rCount = 0;
    Trie head = { 0, 0, { NULL } };
    const int wlen = strlen (words[0]);
    *returnSize = 1;

    for (int i = 0; i < wordsSize; i++) {
        Trie *cur = &head;

        for (int j = 0; j < wlen; j++) {
            const uint8_t idx = words[i][j] - 'a';

            if (cur->t[idx] == NULL) {
                cur->t[idx] = calloc (1, sizeof (Trie));
            }

            cur = cur->t[idx];
        }

        cur->count++;
    }

    {
        const int len = strlen (s);

        for (int i = 0; i <= (len - wlen * wordsSize); i++) {
            Trie *cur = &head;
            int count = 0;

            for (int j = i; j < len; j++) {
                const uint8_t idx = s[j] - 'a';

                if (cur->t[idx] == NULL) {
                    break;
                } else {
                    const bool endOfWord = ((j - i) % wlen) == (wlen - 1);
                    cur = cur->t[idx];

                    if (endOfWord) {
                        if (cur->used < cur->count) {
                            cur->used++;
                            count++;

                            if (count == wordsSize) {
                                break;
                            }
                        } else {
                            break;
                        }

                        cur = &head;
                    }
                }
            }

            if (count == wordsSize) {
                if (rCount == *returnSize) {
                    *returnSize *= 2;
                    ret = realloc (ret, *returnSize * sizeof (int));
                }

                ret[rCount++] = i;
            }

            if (count != 0) {
                ClearUsedTrie (&head);
            }
        }
    }

    FreeTrie (&head);

    if (rCount != *returnSize) {
        ret = realloc (ret, rCount * sizeof (int));
        *returnSize = rCount;
    }

    return ret;
}

/*
Difficulty:Hard
Total Accepted:82.6K
Total Submissions:376.3K


Related Topics Hash Table Two Pointers String
Similar Questions Minimum Window Substring

*/
