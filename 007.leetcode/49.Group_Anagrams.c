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
--- Strategy ---
Make an array of structures called Units from strs.
A Unit holds a string and its sorted version. Sort this array
using the sorted string of each Unit, which groups all anagrams next to each other.
Copy the unsorted strings of each Unit back into a new array of strings called
grouped, making it a contiguous block of strings with all anagrams grouped together.
Then make an array called partitionIndices, an array that assigns the same
integer index to strings that are anagrams. From this partitionIndices array,
it is easy to calculate each entry in *returnColumnSizes. Then assign each entry
in anagramGroup (the answer) to pointers pointing to different entries in grouped
array, now that anagrams in strs are next to each other. Return anagramGroup.
Assume the user will free anagramGroup[0], anagramGroup,
*returnColumnSizes, and every string pointed by elements in grouped.

--Example --
Suppose that strs = ["red", "ten", "net", "bat", "tab"]
After the algorithm involving Unit: grouped = ["tab", "bat", "red", "ten", "net"]
Notice that angrams are next to each other in "grouped".
Then partitionIndices = [0, 0, 1, 2, 2]
Note that indices corresponding to anagrams have the same values.
Next, *returnSize = 3 (because there are 3 different groupings, ie 3 different values in
the partitionIndices array)
Then *returnColumnSizes = [2, 1, 2] (b/c there are two 0's, one 1, and two 2's in partitionIndices)
Lastly, anagramGroups = [grouped, grouped + 2, grouped + 3] where each entry in
anagramGroups points to the first element beginning a new anagramGroup in the grouped array.
*/

typedef struct {
    char *string;
    char *sorted;
} Unit;

int
compareChar (const void *a, const void *b)
{
    return (* (char *)a - * (char *)b);
}

int
compareUnitPtr (const void *a, const void *b)
{
    Unit **u1Ptr = (Unit **) a;
    char *u1Sorted = (*u1Ptr)->sorted;
    Unit **u2Ptr = (Unit **) b;
    char *u2Sorted = (*u2Ptr)->sorted;
    int i;

    for (i = 0; i < strlen (u1Sorted); i++) {
        if (u1Sorted[i] != u2Sorted[i]) {
            return u1Sorted[i] - u2Sorted[i];
        }
    }

    return (int) u1Sorted[i] - (int) u2Sorted[i];
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
char  ***
groupAnagrams (char **strs, int strsSize, int *returnSize, int **returnColumnSizes)
{
    if (strsSize == 0) {
        return NULL;
    }

    // fill up an array with Unit pointers
    Unit *units[strsSize];

    for (int i = 0; i < strsSize; i++) {
        units[i] = (Unit *) malloc (sizeof (Unit));
        units[i]->string = (char *) malloc (sizeof (char) * (strlen (strs[i]) + 1));
        strcpy (units[i]->string, strs[i]);
        units[i]->sorted = (char *) malloc (sizeof (char) * (strlen (strs[i]) + 1));
        strcpy (units[i]->sorted, strs[i]);
        qsort (units[i]->sorted, strlen (strs[i]), sizeof (char), compareChar);
    }

    // sort Unit pointer array and copy each one's unsorted string into grouped
    qsort (units, strsSize, sizeof (Unit *), compareUnitPtr);
    char **grouped = (char **) malloc (sizeof (char *) * strsSize);

    for (int i = 0; i < strsSize; i++) {
        grouped[i] = (char *) malloc (sizeof (char) * (strlen (units[i]->string) + 1));
        strcpy (grouped[i], units[i]->string);
    }

    // create and fill partitionIndices
    int partitionIndices[strsSize];
    int index = 0;
    partitionIndices[0] = index;

    for (int i = 1; i < strsSize; i++) {
        if (strcmp (units[i]->sorted, units[i - 1]->sorted)) {
            index++;
        }

        partitionIndices[i] = index;
    }

    // assign *returnSize and allocate memory for answers
    *returnSize = index + 1;
    char ***anagramGroup = (char ***) malloc (sizeof (char **) * (*returnSize));
    *returnColumnSizes = (int *) malloc (sizeof (int) * (*returnSize));
    // calculate *returnColumnSizes and anagramGroup
    int lengthHelper = 1;
    int colSizesIndex = 0;
    int partitionIndex = 1;

    while (partitionIndex < strsSize) {
        if (partitionIndices[partitionIndex] == partitionIndices[partitionIndex - 1]) {
            lengthHelper++;
        } else {
            (*returnColumnSizes)[colSizesIndex] = lengthHelper;
            lengthHelper = 1;
            colSizesIndex++;
        }

        partitionIndex++;
    }

    (*returnColumnSizes)[colSizesIndex] = lengthHelper;
    anagramGroup[0] = grouped;

    for (int i = 1; i < *returnSize; i++) {
        anagramGroup[i] = anagramGroup[i - 1] + (*returnColumnSizes)[i - 1];
    }

    // free memory allocated for Units array
    for (int i = 0; i < strsSize; i++) {
        free (units[i]->string);
        free (units[i]->sorted);
        free (units[i]);
    }

    return anagramGroup;
}