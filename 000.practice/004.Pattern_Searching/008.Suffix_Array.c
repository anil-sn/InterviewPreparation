#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// Structure to store information of a suffix
typedef struct suffix
{
    int index;
    char *suff;
}suffix;

// A comparison function used by sort() to compare two suffixes
int cmp(const void *x, const void *y)
{
    struct suffix *a = (struct suffix *)x;
    struct suffix *b = (struct suffix *)y;
    return strcmp(a->suff, b->suff) >= 0 ? 1 : 0;
}

// A utility function to print an array of given size
void printArr(struct suffix arr[], int n)
{
    for(int i = 0; i < n; i++)
        printf("%d ", arr[i].index);
    printf("\r\n");
}

// This is the main function that takes a string 'txt' of size n as an
// argument, builds and return the suffix array for the given string
void buildSuffixArray(char *txt, int n)
{
    // A structure to store suffixes and their indexes
    struct suffix suffixes[n];

    // Store suffixes and their indexes in an array of structures.
    // The structure is needed to sort the suffixes alphabetically
    // and maintain their old indexes while sorting
    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        suffixes[i].suff = (txt+i);
    }

    // Sort the suffixes using the comparison function defined above.
    qsort(suffixes, n, sizeof(suffixes[0]), cmp);

    printArr(suffixes, n);

    return;
}



// Driver program to test above functions
int main()
{
    char txt[] = "banana";
    int n = strlen(txt);

    printf("Following is suffix array for %s\r\n", txt);
    buildSuffixArray(txt,  n);
    return 0;
}