/*
31. Next Permutation

Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.


If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).


The replacement must be in-place, do not allocate extra memory.


Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.
1,2,3 → 1,3,2
3,2,1 → 1,2,3
1,1,5 → 1,5,1
*/

/*
    Find next permutation:
        Algorithm:
        step 1: Linearly traverse given array from the end and find a number that is greater than its adjacent nums[i] > nums[i-1]. Store the index of smaller number in breakPoint variable. If there is no such element, reverse entire array and return.

        step 2: Linearly traverse from the end and this time find a number larger than nums[breakPoint]. Let's call it nums[i].

        step 3: Swap nums[i] and nums[breakPoint].

        step 4: Reverse the array from index breakPoint + 1 to numsSize.

        Example:
        Consider nums[] = [1, 3, 5, 4, 2].
        Traverse from back and find a breakpoint. Here, index breakPoint = 1 and nums[breakPoint] = 3
        Traverse from back and find a number larger than this. Here this number is: nums[i] = 4
        Swap nums[breakPoint] and nums[i]. Value after swapping: nums[] = [1, 4, 5, 3, 2].
        Reverse array from breakPoint + 1 to numsSize i.e. these elements: [1, 4, 5, 3, 2]
        Final answer = [1, 4, 2, 3, 5].
*/
int
cmp (const void *a, const void *b)
{
    return * (int *)a - * (int *)b;
}
void
nextPermutation (int *nums, int numsSize)
{
    int i, j, k;

    for (i = numsSize - 1; i > 0; i --) {
        if (nums[i - 1] < nums[i]) {
            j = i - 1;
            // this is the first small number from tail

            while (i < numsSize - 1 &&
                    nums[i + 1] > nums[j]) {
                // find the second small number in the tail
                i ++;
            }

            k = nums[i];
            // swap it
            nums[i] = nums[j];
            nums[j] = k;
            qsort (&nums[j + 1], numsSize - j - 1, sizeof (int), cmp);
            return;
        }
    }

    qsort (nums, numsSize, sizeof (int), cmp);
}

void
nextPermutation (int *nums, int numsSize)
{
    // initialize variable:
    int breakPoint = -1;

    // find a breakpoint:
    for (int i = numsSize - 1; i > 0; i--) {
        if (nums[i] > nums[i - 1]) {
            breakPoint = i - 1;
            break;
        }
    }

    // if no breakpoint
    if (breakPoint < 0) {
        reverse (nums.begin(), nums.end());
        return;
    }

    // if found a breakpoint
    for (int i = numsSize - 1; i >= 0; i--) {
        if (nums[i] > nums[breakPoint]) {
            swap (nums[breakPoint], nums[i]);
            reverse (nums.begin() + breakPoint + 1, nums.end());
            break;
        }
    }
}


/*
Difficulty:Medium
Total Accepted:116.6K
Total Submissions:405.2K


Companies Google
Related Topics Array
Similar Questions


                    Permutations

                    Permutations II

                    Permutation Sequence

                    Palindrome Permutation II
*/
