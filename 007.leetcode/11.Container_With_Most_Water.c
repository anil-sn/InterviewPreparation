/*
11. Container With Most Water

Given n non-negative integers a1, a2, ..., an, where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water.

Note: You may not slant the container and n is at least 2.
*/

#define MIN(_A,_B) (_A < _B ? _A : _B)
#define MAX(_A,_B) (_A > _B ? _A : _B)

int
maxArea (int *height, int heightSize)
{
    int l, r, min, water, max = 0;
    int i = 0;
    int j = heightSize - 1;

    while (i < j) {
        l = height[i];
        r = height[j];
        min = MIN (l, r);
        max = MAX (max, min * (j - i))

        if (l < r) {
            i ++;
        } else {
            j --;
        }
    }

    return max;
}


/*
Difficulty:Medium
Total Accepted:144.5K
Total Submissions:395.2K


Companies Bloomberg
Related Topics Array Two Pointers
Similar Questions


                    Trapping Rain Water
*/
