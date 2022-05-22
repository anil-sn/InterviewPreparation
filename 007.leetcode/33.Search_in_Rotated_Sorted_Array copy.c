/*
33. Search in Rotated Sorted Array

Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.

(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).

You are given a target value to search. If found in the array return its index, otherwise return -1.

You may assume no duplicate exists in the array.
*/

int
search (int *nums, int numsSize, int target)
{
    if (numsSize == 0) {
        return -1;
    }

    int low = 0, high = numsSize - 1;

    while (low < high) {
        int mid = low + (high - low) / 2;

        //found the target, return
        if (nums[mid] == target) {
            return mid;
        }

        //check if nums[mid] and the target are split into 2 different sides
        bool split = (nums[mid] >= nums[0]) != (target >= nums[0]);

        if (split) {
            // if split, need to check the value nums[0] to decide next step
            if (target >= nums[0]) {
                high = mid;
            } else {
                low = mid + 1;
            }
        } else {
            // if not split, just a normal binary search algorithm
            if (target > nums[mid]) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
    }

    return nums[low] == target ? low : -1;
}



int
search (int *nums, int numsSize, int target)
{
    int start, end, mid;
    start = 0;
    end = numsSize - 1;

    while (start <= end) {
        mid = start + (end - start) / 2;

        if (nums[mid] == target) {
            return mid;
        }

        if (nums[start] <= nums[mid]) { // first half are sorted
            if (target   >  nums[mid] || target < nums[start]) {
                start = mid + 1;
            } else {
                end = mid - 1;
            }
        } else { // second half are sorted
            if (target < nums[mid] || target > nums[end]) {
                end = mid - 1;
            } else {
                start = mid + 1;
            }
        }
    }

    return -1;
}

/*
Difficulty:Medium
Total Accepted:184.4K
Total Submissions:574K


Companies LinkedIn Bloomberg Uber Facebook Microsoft
Related Topics Binary Search Array
Similar Questions


                    Search in Rotated Sorted Array II

                    Find Minimum in Rotated Sorted Array
*/
