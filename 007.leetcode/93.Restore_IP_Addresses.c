#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"
#define RESULT_SIZE 10000

bool
is_valid (char *s, int start, int end)
{
    int value = 0;

    if (s[start] == '0' && end - start > 1) {
        return false;
    }

    if (end - start > 3) {
        return false;
    }

    for (int i = start; i < end; i++) {
        value = value * 10 + (s[i] - '0');
    }

    if (value > 255) {
        return false;
    }

    return true;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char **
restoreIpAddresses (char *s, int *returnSize)
{
    int length[4] = {1, 1, 1, 1};
    int start = 0;
    int end = 0;
    int string_len = 0;
    char **result;
    int result_ind = 0;
    int increase = 0;
    result = malloc (RESULT_SIZE * sizeof (char *));

    for (char *c = s; *c != 0; c++) {
        string_len++;
    }

    //string_len++;
    for (int i = 0; i < 16 * 16; i++) {
        //printf("%d %d %d %d\n", length[0], length[1], length[2], length[3]);
        start = 0;

        for (int j = 0; j < 4; j++) {
            end = start + length[j];

            if (end > string_len) {
                break;
            }

            if (!is_valid (s, start, end)) {
                break;
            } else {
                if (j == 3) {
                    if (end != string_len) {
                        break;
                    }

                    //result[result_ind] = malloc((string_len + 4) * sizeof(char));
                    result[result_ind] = calloc (string_len + 4, sizeof (char));
                    start = 0;

                    for (int k = 0; k < 4; k++) {
                        /* MISTAKE*/
                        //end = start + length[k];
                        end = start + length[k];
                        memcpy (result[result_ind] + start + k, s + start, sizeof (char) * length[k]);
                        result[result_ind][end + k] = '.';
                        start = end;
                    }

                    result[result_ind][end + 3] = '\0';
                    result_ind++;
                }
            }

            start = end;
        }

        for (int k = 3; k >= 0; k--) {
            length[k] = length[k] + 1;

            if (length[k] > 3 && k != 0) {
                length[k] = 1;
            } else {
                break;
            }
        }

        if (length[0] >= 5) {
            break;
        }
    }

    *returnSize = result_ind;
    return result;
}
