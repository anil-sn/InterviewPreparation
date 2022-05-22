/*
46. Permutations

Given a collection of distinct numbers, return all possible permutations.



For example,
[1,2,3] have the following permutations:
[
  [1,2,3],
  [1,3,2],
  [2,1,3],
  [2,3,1],
  [3,1,2],
  [3,2,1]
]
*/

/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
void
swap (int *nums, int x, int y)
{
    int temp = nums[x];
    nums[x] = nums[y];
    nums[y] = temp;
}
void
getResult (int **out, int *nums, int numsSize, int left, int right, int *size)
{
    if (left == right) {
        out[*size] = (int *)malloc (numsSize * sizeof (int));
        memcpy (out[ (*size)++], nums, numsSize * sizeof (int));
        return;
    }

    for (int i = left; i <= right; i++) {
        swap (nums, left, i);
        getResult (out, nums, numsSize, left + 1, right, size);
        swap (nums, left, i);
    }
}

int **
permute (int *nums, int numsSize, int *returnSize, int **returnColumnSizes)
{
    int **result = (int **)malloc (1000 * sizeof (int *));
    int *temp = (int *)malloc (numsSize * sizeof (int));
    int size = 0;
    getResult (result, nums, numsSize, 0, numsSize - 1, &size);
    *returnColumnSizes = (int *)malloc (size * sizeof (int));

    for (int i = 0; i < size; i++)
    { (*returnColumnSizes)[i] = numsSize; }

    *returnSize = size;
    return result;
}
/*
Difficulty:Medium
Total Accepted:176.8K
Total Submissions:404.2K


Companies LinkedIn Microsoft
Related Topics Backtracking
Similar Questions


                    Next Permutation

                    Permutations II

                    Permutation Sequence

                    Combinations
*/
