/*
42. Trapping Rain Water

Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it is able to trap after raining.



For example,
Given [0,1,0,2,1,0,1,3,2,1,2,1], return 6.




The above elevation map is represented by array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of rain water (blue section) are being trapped. Thanks Marcos for contributing this image!
*/



int
trap (int *height, int heightSize)
{
    int w = 0;
    int left = 0;
    int right = heightSize - 1;
    int res = 0;
    int maxleft = 0;
    int maxright = 0;

    while (left <= right) {
        if (height[left] <= height[right]) {
            if (height[left] >= maxleft) {
                maxleft = height[left];
            } else {
                res += maxleft - height[left];
            }

            left++;
        } else {
            if (height[right] >= maxright) {
                maxright = height[right];
            } else {
                res += maxright - height[right];
            }

            right --;
        }
    }

    return res;
}

/*
Difficulty:Hard
Total Accepted:121.2K
Total Submissions:330.6K


Companies Google Twitter Zenefits Amazon Apple Bloomberg
Related Topics Array Stack Two Pointers
Similar Questions


                    Container With Most Water

                    Product of Array Except Self

                    Trapping Rain Water II
*/
