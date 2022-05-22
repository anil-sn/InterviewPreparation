/*
4. Median of Two Sorted Arrays

There are two sorted arrays nums1 and nums2 of size m and n respectively.

Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).

Example 1:
nums1 = [1, 3]
nums2 = [2]

The median is 2.0



Example 2:
nums1 = [1, 2]
nums2 = [3, 4]

The median is (2 + 3)/2 = 2.5
*/

int
bs (int *n, int sz, int k)
{
    int i, j, m;
    i = 0;
    j = sz - 1;

    while (i <= j) {
        m = i + (j - i) / 2;

        if (n[m] > k) {
            j = m - 1;
        } else {
            i = m + 1;
        }
    }

    return j;   // this is the lastest one which is not greater than k
}

double
bs2 (int *n1, int sz1, int *n2, int sz2, int m, int m1)
{
    double d;
    int i;
    i = bs (n1, sz1, n2[0]); // search in first array

    if (i >= m) {           // median is in the first array
        d = n1[m];

        if (m1) {
            if (i > m) {
                d += n1[m + 1];
            } else {
                d += n2[0];
            }

            d /= 2;
        }
    } else if (i == sz1 - 1) {  // median is in the second array
        d = n2[m - i - 1];

        if (m1) {
            d += n2[m - i];
            d /= 2;
        }
    } else {                    // reverse arrays and search again
        d = bs2 (n2, sz2, &n1[i + 1], sz1 - i - 1, m - i - 1, m1);
    }

    return d;
}

// binary search 40+ms
double
findMedianSortedArrays (int *nums1, int nums1Size, int *nums2, int nums2Size)
{
    double d;
    int p1, p2;
    p1 =  (nums1Size + nums2Size - 1) / 2;
    p2 = ((nums1Size + nums2Size) % 2) ? 0 : 1;

    if (nums2Size == 0) {
        d = nums1[p1];

        if (p2) {
            d += nums1[p1 + 1];
            d /= 2;
        }

        return d;
    }

    if (nums1Size == 0) {
        d = nums2[p1];

        if (p2) {
            d += nums2[p1 + 1];
            d /= 2;
        }

        return d;
    }

    if (nums2[0] < nums1[0]) {
        return bs2 (nums2, nums2Size, nums1, nums1Size, p1, p2);
    }

    return bs2 (nums1, nums1Size, nums2, nums2Size, p1, p2);
}

/****************************************************************************/

double
findMedianSortedArrays (int *nums1, int nums1Size, int *nums2, int nums2Size)
{
    int index1 = 0;
    int index2 = 0;
    int med1 = 0;
    int med2 = 0;

    for (int i = 0; i <= (nums1Size + nums2Size) / 2; i++) {
        med1 = med2;

        if (index1 == nums1Size) {
            med2 = nums2[index2];
            index2++;
        } else if (index2 == nums2Size) {
            med2 = nums1[index1];
            index1++;
        } else if (nums1[index1] < nums2[index2] ) {
            med2 = nums1[index1];
            index1++;
        }  else {
            med2 = nums2[index2];
            index2++;
        }
    }

    // the median is the average of two numbers
    if ((nums1Size + nums2Size) % 2 == 0) {
        return (float) (med1 + med2) / 2;
    }

    return med2;
}

/*
Difficulty:Hard
Total Accepted:182.7K
Total Submissions:839.6K


Companies Google Zenefits Microsoft Apple Yahoo Dropbox Adobe
Related Topics Binary Search Array Divide and Conquer

*/
