# A Framework for Deconstructing Programming Problems

Before a single line of code is written, a problem must be broken down. Rushing to a solution without a deep understanding of the problem is the most common path to failure. This document outlines a brutal, systematic framework for analysis. Its purpose is to force clarity and expose the problem's true nature, which in turn will point directly to the correct algorithmic pattern.

### Phase 1: Interrogate the Problem Statement

Your first goal is to extract every piece of explicit and implicit information. Do not assume anything. Clarify everything.

#### 1. The Core Task: What is the Actual Goal?

Strip away the story and context. Summarize the objective in a single, precise sentence. Are you being asked to find a specific value? Are you asked to generate all possible solutions? Are you being asked to restructure the given data? Or are you being asked to simply verify if a condition is met? Answering this defines the category of the problem.

#### 2. The Inputs: What Are You Given?

This is the most fertile ground for clues.
*   **Data Structure:** Is the input an array? A linked list? A tree? A graph? The structure of the data dictates the operations you can perform efficiently.
*   **Data Properties:** Is the data sorted? Does it contain duplicates? Are the numbers within a specific, continuous range (like 1 to N)? Are they positive or negative? These properties are not flavor text; they are massive signals. A sorted array screams "Binary Search" or "Two Pointers." A list of numbers from 1 to N hints at "Cyclic Sort."

#### 3. The Outputs: What Must You Produce?

Define the expected output with precision.
*   **Format:** Is it a single number, a boolean value, a new list, or the modified input array itself (an in-place modification)?
*   **Relationship to Input:** Does the output need to preserve the original order? Is it a subset or a permutation of the input?

#### 4. The Constraints: What Are Your Operational Limits?

Constraints are not suggestions; they are hard rules that dictate the viability of an algorithm.
*   **`N` is small (e.g., N <= 20):** This is a green light for solutions with exponential complexity, such as O(2^N) or O(N!). This immediately suggests a brute-force recursive approach like **Backtracking**.
*   **`N` is medium (e.g., N <= 5,000):** A quadratic solution, O(N²), is likely acceptable. This allows for simpler solutions involving nested loops or basic dynamic programming.
*   **`N` is large (e.g., N >= 10^5):** A quadratic solution will time out. You are being forced to find a more efficient algorithm, typically O(N log N) or O(N). This constraint immediately invalidates brute-force and pushes you toward patterns like **Sliding Window**, **Two Pointers**, **Heaps**, or algorithms involving sorting.

### Phase 2: The Brute-Force Baseline

Before optimizing, you must first have a solution that works, no matter how inefficient.

Formulate the most straightforward, naive solution. This is often a nested loop or a simple recursive function that checks every possibility. This step is critical for two reasons:
1.  It proves you have a complete understanding of the problem's requirements.
2.  It establishes a baseline. The inefficiencies and repetitive calculations within your brute-force solution are the very things you need to optimize. The bottleneck you identify here is the problem you are actually trying to solve with a clever pattern.

### Phase 3: Identify the Bottleneck and Select the Pattern

Analyze your brute-force solution. Where is the wasted work?
*   **Repetitive Calculations:** Are you repeatedly calculating the sum of overlapping subarrays? This points directly to the **Sliding Window** or **Prefix Sum** pattern.
*   **Redundant Comparisons:** In a sorted array, are you re-scanning the entire array for each element to find its partner? This points to the **Two Pointers** pattern.
*   **Exploring an Entire Search Space:** Are you generating all possibilities when you could be pruning invalid paths? This is a signal for **Backtracking**.

Only after this rigorous deconstruction can you confidently select a pattern. The pattern is not a magic bullet; it is a targeted tool chosen to solve a specific, identified inefficiency.

---

# The Two Pointers Pattern

The Two Pointers pattern is a technique primarily used to transform problems on sorted arrays that would otherwise require quadratic time complexity, O(N²), into a solution with linear time complexity, O(N). It is not a random trick; it is a logical consequence of leveraging the sorted property of the data to make intelligent decisions.

### The Problem: Pair with Target Sum

Let's analyze a canonical problem where this pattern is the optimal solution: "Given a **sorted** array of unique integers and a target sum, find a pair in the array whose sum is equal to the target. The function should return the indices of the two numbers."

### Step 1: Deconstruction and The Brute-Force Baseline

We will begin by applying the analytical framework.

First, we deconstruct the problem statement.
*   **Core Task:** Find a pair of numbers that add up to a specific value.
*   **Input:** An array of integers. The critical, non-negotiable property is that the array is **sorted**. The problem also specifies the numbers are unique.
*   **Output:** The indices of the two numbers forming the pair.
*   **Constraints:** We will assume the array can be large, making an O(N²) solution too slow and thus unacceptable.

Next, we establish a baseline by formulating the most direct, brute-force solution. This involves checking every possible pair of numbers. We can achieve this with two nested loops. The outer loop picks the first number, and the inner loop iterates through the remaining numbers to see if any of them form a valid pair with the first number.

Here is the brute-force implementation in C.

```c
// C - Brute-Force Implementation (O(N^2))
#include <stdio.h>

void find_pair_brute_force(int arr[], int n, int target) {
    // Outer loop selects the first element of the pair.
    for (int i = 0; i < n; i++) {
        // Inner loop selects the second element.
        // It starts from 'i + 1' to avoid duplicate pairs and self-comparison.
        for (int j = i + 1; j < n; j++) {
            if (arr[i] + arr[j] == target) {
                printf("Pair found at indices: %d, %d (Values: %d, %d)\n", i, j, arr[i], arr[j]);
                return; // Exit after finding the first pair.
            }
        }
    }
    printf("No pair found for target %d.\n", target);
}
```

This code works correctly. However, its time complexity is O(N²), which violates our constraint for large inputs. The clear bottleneck is the nested loop structure. For every single element, we perform a linear scan over the rest of the array. This repeated scanning is the specific inefficiency that we must eliminate.

### Step 2: The Insight: Leveraging the "Sorted" Property

The brute-force approach completely ignores the most important piece of information we were given: the array is **sorted**. A sorted array allows us to make powerful inferences. If we pick a number, we know that everything to its left is smaller and everything to its right is larger. How can we use this to avoid a linear scan?

Let's set up an experiment. Instead of scanning from the beginning each time, let's place one pointer at the absolute beginning of the array (`left`, index 0) and a second pointer at the absolute end (`right`, index `n-1`). These two pointers represent a candidate pair: the smallest available element and the largest available element.

Now, we calculate their sum, `current_sum = arr[left] + arr[right]`. This single sum gives us a wealth of information.
1.  If `current_sum` equals the `target`, we have found our solution.
2.  If `current_sum` is **less than** the `target`, we know our sum is too small. To make the sum larger, we must increase the value of one of the numbers. Since `arr[right]` is already the largest element in the current search space, we cannot increase it. Our only logical move is to discard the smallest element, `arr[left]`, and try the next smallest one by moving the `left` pointer one step to the right (`left++`).
3.  If `current_sum` is **greater than** the `target`, we know our sum is too large. To make the sum smaller, we must decrease the value of one of the numbers. Since `arr[left]` is already the smallest element, our only choice is to discard the largest element, `arr[right]`, and try the next largest one by moving the `right` pointer one step to the left (`right--`).

This process is guaranteed to find the pair if one exists. With every single comparison, we intelligently eliminate one element from our search space. The pointers move towards each other, and the search ends when they cross.

### Step 3: The Optimized C Implementation

This logic translates directly into a clean and efficient algorithm.

```c
#include <stdio.h>

// Assumes arr is sorted
void find_pair_two_pointers(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;

    // Continue the search as long as the pointers have not crossed.
    while (left < right) {
        int current_sum = arr[left] + arr[right];

        if (current_sum == target) {
            printf("Pair found at indices: %d, %d (Values: %d, %d)\n", left, right, arr[left], arr[right]);
            return;
        } 
        
        if (current_sum < target) {
            // The sum is too small. The only way to increase it is
            // to move the left pointer to a larger value.
            left++;
        } else { // current_sum > target
            // The sum is too large. The only way to decrease it is
            // to move the right pointer to a smaller value.
            right--;
        }
    }

    printf("No pair found for target %d.\n", target);
}

int main() {
    int arr[] = {1, 2, 4, 6, 8, 9};
    int n = sizeof(arr) / sizeof(arr);
    int target = 10;
    
    find_pair_two_pointers(arr, n, target); // Expected: 2, 4 (for values 4 and 6)
    
    int target2 = 18;
    find_pair_two_pointers(arr, n, target2); // Expected: No pair found.

    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N). The `left` pointer only moves forward, and the `right` pointer only moves backward. In the worst-case scenario, they each traverse the array once, meaning the `while` loop runs at most N-1 times. This is a definitive improvement over O(N²).
*   **Space Complexity:** O(1). We only use a fixed number of variables (`left`, `right`, `current_sum`) to store state. The memory usage does not scale with the size of the input array.

The Two Pointers pattern, in this case, is not a clever "trick." It is the direct result of analyzing the problem's inputs, identifying the bottleneck in the naive solution, and using the properties of the data to eliminate that bottleneck.

Of course. Here is the next document, focusing on the **Sliding Window Pattern**.

---

# The Sliding Window Pattern

The Sliding Window pattern is an algorithmic technique used to efficiently handle problems involving a contiguous block of elements, such as a subarray or substring. It avoids redundant work by maintaining a "window" of elements and sliding it through the data structure, rather than re-computing values for each possible block. This approach can dramatically reduce the time complexity from quadratic, O(N\*K), to linear, O(N).

### The Problem: Maximum Sum Subarray of Size K

Let's ground this pattern in a concrete problem: "Given an array of integers and a positive integer `k`, find the maximum sum of any contiguous subarray of size `k`."

### Step 1: Deconstruction and The Brute-Force Baseline

We begin with our standard analytical framework.

First, the deconstruction.
*   **Core Task:** Find a maximum sum over a fixed-size block of elements.
*   **Input:** An array of integers and a size `k`. The key terms are **"contiguous subarray"** and **"size k"**.
*   **Output:** A single integer representing the calculated maximum sum.
*   **Constraints:** Assume `N` (the array size) can be large, making any solution worse than linear time unacceptable.

Next, we establish the brute-force baseline. The most straightforward method is to generate every single contiguous subarray of size `k`, calculate the sum of each one, and keep track of the largest sum we've seen. This naturally leads to a nested loop structure. The outer loop will define the starting point of the subarray, and the inner loop will sum up the `k` elements starting from that point.

Here is the brute-force implementation in C.

```c
#include <stdio.h>
#include <limits.h> // Required for INT_MIN

// C - Brute-Force Implementation (O(N*K))
int find_max_sum_brute_force(int arr[], int n, int k) {
    if (n < k) {
        return -1; // Sanity check: not possible to form a subarray of size k.
    }
    
    int max_sum = INT_MIN; // Initialize with the smallest possible integer.

    // The outer loop iterates through all possible starting points of a subarray.
    // It stops at n-k, as any start beyond that won't form a full subarray of size k.
    for (int i = 0; i <= n - k; i++) {
        int current_sum = 0;
        // The inner loop calculates the sum of the subarray starting at i.
        for (int j = i; j < i + k; j++) {
            current_sum += arr[j];
        }
        
        // Update the maximum sum if the current subarray's sum is greater.
        if (current_sum > max_sum) {
            max_sum = current_sum;
        }
    }
    return max_sum;
}
```

This code is correct, but its time complexity is O(N\*K). For each of the approximately `N` starting positions, we perform `K` additions. The **bottleneck is the inner loop**. Consider the subarray starting at index 0: `[arr[0], arr[1], ..., arr[k-1]]`. Now consider the next subarray at index 1: `[arr[1], arr[2], ..., arr[k]]`. We are re-summing `k-1` elements that we just processed. This massive amount of redundant calculation is precisely what the Sliding Window pattern is designed to eliminate.

### Step 2: The Insight: Sliding Instead of Recalculating

The inefficiency of the brute-force method comes from treating each subarray as an independent entity. The Sliding Window pattern recognizes that adjacent subarrays are highly related. When we move from one subarray to the next, we are simply dropping one element from the beginning and adding one element to the end.

The insight is this: instead of rebuilding the sum from scratch, we can maintain a running sum for our "window." To "slide" the window one position to the right:
1.  **Subtract** the element that is now outside the window (the one on the far left).
2.  **Add** the element that has just entered the window (the one on the far right).

This is a constant-time O(1) operation. By applying this simple update `N-K` times, we can find the sum of every possible subarray in a single pass through the data.

### Step 3: The Optimized C Implementation

This logic translates into a much more efficient algorithm. The process involves two main stages: first, priming the pump by calculating the sum of the initial window, and second, sliding the window across the rest of the array.

```c
#include <stdio.h>

int find_max_sum_sliding_window(int arr[], int n, int k) {
    if (n < k) {
        return -1; // Not possible to have a subarray of size k.
    }

    int max_sum = 0;
    int window_sum = 0;

    // Phase 1: Calculate the sum of the very first window.
    // This is the only time we need a loop to compute a full sum.
    for (int i = 0; i < k; i++) {
        window_sum += arr[i];
    }
    
    max_sum = window_sum;

    // Phase 2: Slide the window from the start to the end of the array.
    // The loop starts from the k-th element, which is the first new element to enter a window.
    for (int i = k; i < n; i++) {
        // The core logic: update the sum in O(1) time.
        // arr[i] is the new element entering the window.
        // arr[i - k] is the old element leaving the window.
        window_sum = window_sum + arr[i] - arr[i - k];

        // Update the overall maximum sum if the new window's sum is greater.
        if (window_sum > max_sum) {
            max_sum = window_sum;
        }
    }

    return max_sum;
}

int main() {
    int arr[] = {2, 1, 5, 1, 3, 2};
    int k = 3;
    int n = sizeof(arr) / sizeof(arr);
    
    int result = find_max_sum_sliding_window(arr, n, k);
    
    // The subarrays are:
    // -> sum = 8
    // -> sum = 7
    // -> sum = 9
    // -> sum = 6
    // The max sum is 9.
    printf("The maximum sum of a subarray of size %d is: %d\n", k, result);

    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N). We perform a single pass through the array. The initial sum takes K steps, and the subsequent sliding takes N-K steps. The total number of operations is proportional to N, not N\*K.
*   **Space Complexity:** O(1). We use only a few variables to store the sums and loop counters. The memory usage is constant and does not depend on the input size.

By identifying the "contiguous subarray" signal and recognizing the re-computation bottleneck in the naive solution, we were naturally led to the Sliding Window pattern as the optimal tool for the job.

Of course. Here is the next document in the series, focusing on the **Fast & Slow Pointers Pattern**.

---

# The Fast & Slow Pointers Pattern

The Fast & Slow Pointers pattern, often called the "tortoise and the hare" algorithm, is a technique that employs two pointers moving through a sequence, typically a linked list, at different speeds. The relative difference in their speed is a powerful mechanism for detecting cycles, finding middle elements, or identifying other structural properties within the data.

### The Problem: Linked List Cycle

Let's explore this pattern through its most classic application: "Given the head of a singly linked list, determine if it has a cycle. A cycle exists if some node in the list can be reached again by continuously following the `next` pointer."

### Step 1: Deconstruction and The Naive Approach

First, we apply our analytical framework.
*   **Core Task:** Detect if a loop exists in a sequence of nodes.
*   **Input:** The `head` node of a singly linked list.
*   **Output:** A boolean value: `true` if a cycle is present, `false` otherwise.
*   **Constraints:** The ideal solution should not use extra memory proportional to the size of the list. We are aiming for a constant, O(1), space complexity.

The most intuitive, brute-force-like solution is to keep a record of every node we have already visited. We can traverse the list one node at a time. For each node we encounter, we check if we have seen it before. If we have, we have discovered a cycle. If we haven't, we add it to our record of visited nodes and move to the next. If we successfully traverse the entire list and reach `NULL`, then no cycle exists.

A hash set is the ideal data structure for this, providing O(1) average time complexity for insertions and lookups. While C does not have a built-in hash set, the logic is what matters. The fundamental flaw of this approach is not its time complexity, but its space complexity. To store a record of every visited node, we would need O(N) extra space for a list of N nodes. This violates our constraint.

### Step 2: The Insight: A Race on a Circular Track

The constraint of O(1) space is a powerful hint. It forces us to solve the problem without using any auxiliary data structures. The solution must be found by only manipulating pointers within the list itself. This is the primary signal for the Fast & Slow Pointers pattern.

Let's re-frame the problem with an analogy. Imagine two runners on a track. One runner, the "hare," is twice as fast as the other, the "tortoise."
*   If the track is a straight line, the fast runner will simply reach the finish line and the race ends. They will never meet again.
*   If the track is a circular loop, the fast runner will eventually lap the slow runner and meet them from behind. Their meeting is an inevitable consequence of their different speeds within a closed loop.

We can apply this exact logic to our linked list.
1.  We initialize two pointers, `slow` and `fast`, both starting at the `head` of the list.
2.  In each step of the traversal, we move `slow` by one node (`slow = slow->next`).
3.  Simultaneously, we move `fast` by two nodes (`fast = fast->next->next`).

If the linked list is linear (a straight line), the `fast` pointer will reach the end (`NULL`) first. This is our condition for a list with no cycle. However, if a cycle exists, both pointers will eventually enter the loop. Once inside, the `fast` pointer gains one position on the `slow` pointer with every iteration. This guarantees that they will eventually point to the same node. This collision is our proof that a cycle exists.

### Step 3: The Optimized C Implementation

This two-pointer race translates directly into a very clean and memory-efficient algorithm.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // For using bool, true, false

// Define the structure for a node in the linked list.
typedef struct ListNode {
    int value;
    struct ListNode *next;
} ListNode;

bool has_cycle(ListNode *head) {
    // If the list is empty or has only one node, no cycle can exist.
    if (head == NULL || head->next == NULL) {
        return false;
    }

    ListNode *slow = head;
    ListNode *fast = head;

    // The loop must terminate if the list is linear.
    // The condition 'fast != NULL && fast->next != NULL' is critical.
    // It ensures we can safely access 'fast->next->next'.
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;          // Moves one step.
        fast = fast->next->next;    // Moves two steps.

        // If the pointers meet, a cycle has been detected.
        if (slow == fast) {
            return true;
        }
    }

    // If the loop finishes, it means 'fast' reached the end of the list.
    return false;
}

int main() {
    // Create a sample linked list with a cycle: 1 -> 2 -> 3 -> 4 -> 2
    ListNode *head = (ListNode*)malloc(sizeof(ListNode));
    head->value = 1;
    head->next = (ListNode*)malloc(sizeof(ListNode));
    head->next->value = 2;
    ListNode *cycle_node = head->next; // Node 2 is where the cycle will start.
    head->next->next = (ListNode*)malloc(sizeof(ListNode));
    head->next->next->value = 3;
    head->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    head->next->next->next->value = 4;
    head->next->next->next->next = cycle_node; // Point back to node 2.

    if (has_cycle(head)) {
        printf("A cycle was detected in the linked list.\n");
    } else {
        printf("No cycle was detected.\n");
    }

    // Remember to free allocated memory in a real application.
    // For this example, we skip freeing to avoid complexity, as the structure is cyclical.

    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N). While the `fast` pointer moves twice as fast, the total number of operations is proportional to the number of nodes, N. The `slow` pointer will have traversed at most N nodes before a collision or the end of the list is found.
*   **Space Complexity:** O(1). This is the pattern's key advantage. We only use two `ListNode` pointers, `slow` and `fast`. The memory footprint is constant and does not scale with the list's size, perfectly satisfying our constraint.

The Fast & Slow Pointers pattern provides an elegant solution by leveraging the relative motion of pointers to uncover the underlying structure of the data, a feat impossible to achieve efficiently with a single pointer and no extra memory.

Of course. Here is the next document in the series, focusing on the **Merge Intervals Pattern**.

---

# The Merge Intervals Pattern

The Merge Intervals pattern is a technique for solving problems that involve overlapping intervals. Given a collection of intervals, the goal is often to consolidate them into a minimal set of non-overlapping intervals. This pattern is fundamental in areas like scheduling, resource allocation, and geometric problems.

### The Problem: Merging Overlapping Intervals

Let's use the canonical problem for this pattern: "Given a collection of intervals, merge all overlapping intervals. Each interval is represented as a pair of integers `[start, end]`." For example, given `[[1,4], [2,5], [7,9]]`, the intervals `[1,4]` and `[2,5]` overlap and should be merged into `[1,5]`, resulting in `[[1,5], [7,9]]`.

### Step 1: Deconstruction and The Brute-Force Baseline

We begin with our analytical framework.
*   **Core Task:** Consolidate a set of intervals into the smallest possible set of non-overlapping intervals.
*   **Input:** A list of intervals, where each interval has a start and an end time. A crucial, unstated assumption is that the intervals are **not sorted**.
*   **Output:** A new list of merged, non-overlapping intervals.
*   **Constraints:** An efficient solution is required, as the number of intervals `N` can be large. An O(N²) approach should be considered a slow baseline to be improved upon.

A brute-force approach might involve comparing every interval with every other interval. For each pair of intervals, we would check if they overlap. If they do, we would merge them into a new interval and replace the original two. This process would need to be repeated until no more merges are possible. This approach is complex to implement correctly (managing the list as it changes) and is very inefficient, likely resulting in a time complexity of O(N²) or worse due to repeated comparisons. The clear bottleneck is the disorganized, all-pairs comparison.

### Step 2: The Insight: The Power of Sorting

The chaos of the brute-force approach comes from the unpredictable order of the intervals. What if we could impose an order on them? This is the critical insight that unlocks an efficient solution.

Let's sort the intervals based on their **start times**.

Once sorted, the problem becomes much simpler. As we iterate through the sorted intervals, we only need to consider the relationship between the current interval and the *last* merged interval we created. There are only two possibilities:
1.  The current interval **overlaps** with the last merged interval. An overlap occurs if the current interval's `start` is less than or equal to the last merged interval's `end`. If they overlap, we don't add a new interval. Instead, we **update** the `end` of the last merged interval to be the maximum of its current end and the current interval's end. This effectively extends our last merged interval to encompass the current one.
2.  The current interval **does not overlap** with the last merged interval. This means we have finished processing a consolidated block. We can now add the current interval to our list of merged intervals as a new, separate block to be considered for future merges.

By sorting the intervals first, we transform the problem from a complex N-to-N comparison into a simple, linear scan.

### Step 3: The Optimized C Implementation

Implementing this pattern in C requires careful handling of data structures, as C does not have built-in dynamic arrays like C++ vectors or Python lists. The process involves sorting the input array and then iterating through it to build a new array of merged intervals.

First, let's define our Interval structure and the comparison function needed for `qsort`.

```c
#include <stdio.h>
#include <stdlib.h>

// Define the structure for an interval.
typedef struct {
    int start;
    int end;
} Interval;

// Comparison function for qsort.
// It will sort intervals based on their start times.
int compare_intervals(const void *a, const void *b) {
    Interval *interval_a = (Interval *)a;
    Interval *interval_b = (Interval *)b;
    return interval_a->start - interval_b->start;
}
```

Now, we can write the main `merge` function.

```c
Interval* merge(Interval* intervals, int n, int* return_size) {
    if (n <= 1) {
        *return_size = n;
        return intervals;
    }

    // Step 1: Sort the intervals based on their start times.
    qsort(intervals, n, sizeof(Interval), compare_intervals);

    // We will build our result in a new array.
    // In a real-world scenario, you might need to reallocate this.
    // For simplicity, we allocate enough space for the worst case (no merges).
    Interval *merged = (Interval *)malloc(n * sizeof(Interval));
    int merged_count = 0;

    // Add the first interval to our result list.
    merged = intervals;
    merged_count++;

    // Step 2: Iterate through the sorted intervals.
    for (int i = 1; i < n; i++) {
        Interval current = intervals[i];
        Interval *last_merged = &merged[merged_count - 1];

        // Check for overlap: current.start <= last_merged.end
        if (current.start <= last_merged->end) {
            // Overlap exists. Update the end of the last merged interval.
            if (current.end > last_merged->end) {
                last_merged->end = current.end;
            }
        } else {
            // No overlap. Add the current interval as a new one.
            merged[merged_count] = current;
            merged_count++;
        }
    }

    *return_size = merged_count;
    return merged;
}

void print_intervals(Interval* intervals, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("[%d, %d]", intervals[i].start, intervals[i].end);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    Interval intervals[] = {{1, 4}, {2, 5}, {7, 9}, {10, 12}};
    int n = sizeof(intervals) / sizeof(intervals);
    int merged_size = 0;

    printf("Original Intervals: ");
    print_intervals(intervals, n);

    Interval *result = merge(intervals, n, &merged_size);

    printf("Merged Intervals: ");
    print_intervals(result, merged_size);

    free(result); // Clean up the dynamically allocated memory.
    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N log N). The dominant operation in this algorithm is the initial sort, which takes O(N log N) time. The subsequent linear scan to merge the intervals takes O(N) time. Therefore, the total complexity is dictated by the sort.
*   **Space Complexity:** O(N) or O(log N), depending on the implementation of the sort algorithm. In our C implementation, we also allocate a new array of size N for the result, leading to O(N) auxiliary space.

The Merge Intervals pattern is a clear demonstration of how imposing order on a seemingly chaotic input can drastically simplify a problem, reducing a complex, multi-pass comparison into an elegant and efficient single-pass algorithm.

Of course. Here is the next document, focusing on the **Top 'K' Elements Pattern**.

---

# The Top 'K' Elements Pattern

The Top 'K' Elements pattern provides an efficient way to find the 'k' largest, smallest, or most frequent elements in a collection. The core of this pattern is recognizing that we do not need to fully sort the entire collection. Instead, we can use a data structure that maintains a partially sorted set of the 'k' elements we care about. The ideal tool for this job is a Heap.

### The Problem: Find the 'K' Largest Numbers

Let's analyze a classic problem that exemplifies this pattern: "Given an unsorted array of numbers, find the 'k' largest numbers in it."

### Step 1: Deconstruction and The Brute-Force Baseline

We will start with our standard analytical framework.

First, the deconstruction.
*   **Core Task:** Find a subset of `k` elements that are the largest in the collection.
*   **Input:** An unsorted array of numbers and an integer `k`.
*   **Output:** A list containing the `k` largest numbers. The order within this final list does not matter.
*   **Constraints:** The size of the array `N` can be very large, while `k` might be relatively small. This `k << N` relationship is a strong signal.

The most straightforward, brute-force solution is to bring full order to the chaos. We can sort the entire array in descending order and then simply pick the first `k` elements. This is correct, but it does far more work than necessary.

The C implementation would use the standard library's `qsort` function.

```c
#include <stdio.h>
#include <stdlib.h>

// Comparison function for qsort to sort in descending order.
int compare_desc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

void find_k_largest_sorting(int arr[], int n, int k) {
    if (k > n || k <= 0) {
        printf("Invalid value of k.\n");
        return;
    }

    // Sort the entire array. Time complexity: O(N log N).
    qsort(arr, n, sizeof(int), compare_desc);

    printf("The %d largest elements are: ", k);
    for (int i = 0; i < k; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
```

The time complexity of this solution is O(N log N), dominated entirely by the sorting step. The **bottleneck is the sort itself**. We are spending resources to order all `N` elements perfectly, including the `N-k` elements at the bottom that we have no interest in. This is extreme overkill, especially when `N` is a million and `k` is ten.

### Step 2: The Insight: A Bouncer for an Exclusive Club

The inefficiency of sorting comes from maintaining a complete order. We only need to separate the top `k` elements from the rest. Let's re-frame the problem. Imagine we are building an exclusive club that only has room for `k` members, where membership is based on being the largest numbers.

We can use a **Min-Heap** data structure to act as the "club." A Min-Heap is a binary tree-based structure that can hold `k` elements and always provides instant access (O(1)) to the *smallest* element within it.

Here is the process:
1.  We create a Min-Heap that will be our club. It has a strict capacity of `k`.
2.  We start by admitting the first `k` numbers from the array into our club. The heap now contains the `k` largest numbers seen *so far*. The smallest of these "club members" is at the entrance (the root of the Min-Heap).
3.  Now, we iterate through the rest of the array, from element `k` to `n-1`. Each new number is a "candidate" trying to get into the club.
4.  The candidate number is compared to the bouncer—the smallest number currently in the club (the heap's root).
5.  If the candidate is **larger** than the bouncer, it deserves a spot. The bouncer is kicked out (we remove the min element from the heap), and the new, larger candidate is admitted (we insert the new element into the heap).
6.  If the candidate is **smaller or equal** to the bouncer, it is not worthy of joining. We reject it and move on.

After checking every number in the array, our club (the Min-Heap) will contain exactly the `k` largest numbers from the entire collection.

### Step 3: The Optimized C Implementation

Since C does not have a standard library for heaps, we must implement a basic one. The core operations are `heapify_down` (to maintain the heap property after removing the root) and `heapify_up` (to maintain it after insertion). For this specific problem, we can simplify things by building a heap and then replacing the root and re-heapifying.

```c
#include <stdio.h>
#include <stdlib.h>

// Helper to swap two integers.
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Maintains the Min-Heap property from a given root index.
void min_heapify(int heap[], int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left] < heap[smallest])
        smallest = left;

    if (right < size && heap[right] < heap[smallest])
        smallest = right;

    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        min_heapify(heap, size, smallest);
    }
}

// Builds a Min-Heap from an array.
void build_min_heap(int heap[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        min_heapify(heap, size, i);
    }
}

void find_k_largest_heap(int arr[], int n, int k) {
    if (k > n || k <= 0) {
        printf("Invalid value of k.\n");
        return;
    }

    // Create a Min-Heap of size k with the first k elements.
    int* heap = (int*)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        heap[i] = arr[i];
    }
    build_min_heap(heap, k);

    // Iterate through the rest of the array.
    for (int i = k; i < n; i++) {
        // If the current element is larger than the smallest element in the heap...
        if (arr[i] > heap) {
            // ...replace the smallest element and fix the heap.
            heap = arr[i];
            min_heapify(heap, k, 0);
        }
    }

    printf("The %d largest elements are: ", k);
    for (int i = 0; i < k; i++) {
        printf("%d ", heap[i]);
    }
    printf("\n");

    free(heap);
}

int main() {
    int arr[] = {3, 1, 5, 12, 2, 11, 8, 7};
    int k = 3;
    int n = sizeof(arr) / sizeof(arr);

    find_k_largest_heap(arr, n, k); // Expected: 12, 11, 8 (in any order)
    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N log K). We build the initial heap in O(K) time. Then, we iterate through the remaining `N-K` elements. For each of these elements, we may perform a heap operation (a `min_heapify` from the root) which takes O(log K) time, as the heap size is always `k`. This gives a total complexity of O(K + (N-K) \* log K), which simplifies to O(N log K). This is a significant improvement over O(N log N) when `k` is small.
*   **Space Complexity:** O(K). We need to store `k` elements in our heap, so the memory usage is proportional to `k`, not `N`.

By identifying that a full sort was unnecessary, we chose a more appropriate tool for the job. The heap allows us to maintain the "best-of" list efficiently, solving the problem with superior time and space complexity.

Of course. Here is the next document, which introduces the first of the fundamental traversal patterns: **Breadth-First Search (BFS)**.

---

# The Breadth-First Search (BFS) Pattern

Breadth-First Search (BFS) is a foundational traversal algorithm used to explore trees and graphs. It is not merely an algorithm but a pattern for solving a class of problems where the goal is to explore nodes in layers or levels. The core principle of BFS is to visit all neighbors at the present depth before moving on to the nodes at the next depth level. This makes it the natural choice for finding the shortest path in an unweighted graph.

### The Problem: Binary Tree Level Order Traversal

To understand BFS, we will use a classic problem: "Given the root of a binary tree, traverse the nodes of the tree in a level-by-level order. For each level, the nodes should be read from left to right." For example, the output should group nodes by their depth.

### Step 1: Deconstruction and The Conceptual Hurdle

Let's apply our analytical framework.
*   **Core Task:** Traverse a tree by exploring each level completely before moving to the next.
*   **Input:** The `root` node of a binary tree.
*   **Output:** A representation of the nodes organized by level. For this document, we will print them level by level.
*   **Constraints:** The traversal must be efficient, visiting each node once.

Unlike previous patterns, there isn't a simple "brute-force" nested loop to compare against. The challenge here is not about reducing complexity but about managing the state of the traversal. The core question is: as we stand on a node, we can see its immediate children. But how do we keep track of all these children from all the nodes on the current level, and ensure we visit them *after* we finish our current level, but *before* their own children?

A simple recursive approach, which naturally follows a branch down to its end, will result in a Depth-First Search. To achieve a breadth-first traversal, we need a data structure that allows us to store the nodes we need to visit later in a specific order.

### Step 2: The Insight: A Waiting Line for Nodes

The problem requires us to process nodes in the order they are discovered. The nodes on Level 0 (the root) are discovered first. Then, their children on Level 1 are discovered. Then, the children of all nodes on Level 1 are discovered, forming Level 2. This "first-discovered, first-processed" behavior is the exact definition of a **Queue** data structure, which operates on a First-In, First-Out (FIFO) principle.

The queue will act as our "waiting room" for nodes to be visited. The BFS algorithm proceeds as follows:
1.  Initialize an empty queue.
2.  Add the `root` node to the queue. This is our starting point.
3.  Start a loop that continues as long as the queue is not empty.
4.  Inside the loop, we want to process one full level at a time. To do this, we first determine the number of nodes currently in the queue. Let's call this `level_size`. These are all the nodes for the current level.
5.  We then run an inner loop `level_size` times.
6.  In this inner loop, we perform the core action:
    a.  **Dequeue** a node from the front of the queue.
    b.  **Visit** it (e.g., print its value).
    c.  **Enqueue** its children (if they exist, typically left then right) to the back of the queue. These children will be processed in a future level.

Because we only loop `level_size` times, we guarantee that we process all nodes from the current level before moving on. The children we add to the queue will only be processed in the *next* iteration of the main outer loop. The FIFO nature of the queue ensures order is perfectly maintained.

### Step 3: The C Implementation

Implementing BFS in C requires us to first create a Queue data structure, as one is not provided by the standard library. For this example, we will use a simple linked-list-based queue.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 1. Define the Tree Node structure
typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 2. Define the Queue Node and Queue structures
typedef struct QueueNode {
    TreeNode *tree_node;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

// Helper to create a new tree node
TreeNode* new_tree_node(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 3. Implement Queue operations
Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tn) {
    QueueNode *new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->tree_node = tn;
    new_node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
        return;
    }
    q->rear->next = new_node;
    q->rear = new_node;
}

TreeNode* dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    QueueNode *temp = q->front;
    TreeNode *result = temp->tree_node;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return result;
}

bool is_queue_empty(Queue *q) {
    return q->front == NULL;
}

// 4. Implement the BFS Level Order Traversal
void level_order_traversal(TreeNode* root) {
    if (root == NULL) return;

    Queue *q = create_queue();
    enqueue(q, root);
    int level = 0;

    while (!is_queue_empty(q)) {
        // First, get the number of nodes at the current level
        int level_size = 0;
        QueueNode* temp = q->front;
        while(temp != NULL){
            level_size++;
            temp = temp->next;
        }

        printf("Level %d: ", level);

        // Process all nodes for this level
        for (int i = 0; i < level_size; i++) {
            TreeNode* current = dequeue(q);
            printf("%d ", current->value);

            // Enqueue children for the next level
            if (current->left != NULL) enqueue(q, current->left);
            if (current->right != NULL) enqueue(q, current->right);
        }
        printf("\n");
        level++;
    }
    free(q);
}

int main() {
    // Create a sample tree:
    //      3
    //     / \
    //    9   20
    //       /  \
    //      15   7
    TreeNode *root = new_tree_node(3);
    root->left = new_tree_node(9);
    root->right = new_tree_node(20);
    root->right->left = new_tree_node(15);
    root->right->right = new_tree_node(7);

    level_order_traversal(root);

    // Free tree memory... (omitted for brevity)
    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N), where N is the number of nodes in the tree. This is because every node is enqueued and dequeued exactly one time.
*   **Space Complexity:** O(W), where W is the maximum width of the tree. The space is determined by the maximum number of nodes that can be in the queue at any one time. In the worst-case scenario of a complete binary tree, the last level contains roughly N/2 nodes, making the space complexity O(N).

BFS provides a structured, layer-by-layer exploration of a data structure. Its reliance on a queue is not arbitrary; it is the essential tool that enforces the "visit neighbors first" discipline required by the pattern.

Of course. Here is the next document, covering the counterpart to BFS: the **Depth-First Search (DFS) Pattern**.

---

# The Depth-First Search (DFS) Pattern

Depth-First Search (DFS) is the other major traversal algorithm for trees and graphs. In direct contrast to the level-by-level exploration of BFS, the core philosophy of DFS is to go as deep as possible down one path before backtracking and exploring an alternative path. This makes it the natural choice for problems involving pathfinding, checking for connectivity, or exploring an entire state space (like a maze or puzzle solution).

### The Problem: Path Sum in a Binary Tree

A simple traversal doesn't fully capture the power of DFS. Let's use a problem that highlights its path-finding and backtracking nature: "Given the root of a binary tree and a target sum, determine if the tree has a root-to-leaf path such that adding up all the values along the path equals the given sum."

### Step 1: Deconstruction and The Conceptual Hurdle

We start with our analytical framework.
*   **Core Task:** Find if a specific root-to-leaf path exists.
*   **Input:** The `root` of a binary tree and an integer `targetSum`.
*   **Output:** A boolean value (`true` if such a path exists, `false` otherwise).
*   **Constraints:** We must check all possible root-to-leaf paths to be certain.

The conceptual challenge is managing the state of the traversal. As we move down a path, we need to keep a running total of the sum. If we hit a leaf and the sum is not correct, or we hit a dead end (`NULL`), how do we "go back" to a previous node and try its other branch? Furthermore, how do we correctly revert the running sum when we backtrack? Trying to manage this state iteratively with loops is complex and requires an explicit stack data structure.

### Step 2: The Insight: Recursion as a Pathfinding Engine

The "go deep, then backtrack" behavior is the exact model of **recursion**. The function call stack is the perfect, implicit data structure to manage the state of a DFS traversal.

Here is how we can frame the problem recursively:
A path sum exists in a tree if:
*   The current node is not null, AND
*   When we reach a leaf node, its value is exactly what we need to complete the sum, OR
*   A valid path sum exists in its left subtree, OR
*   A valid path sum exists in its right subtree.

Let's refine this logic. Instead of passing down an increasing `current_sum`, it's cleaner to subtract the node's value from the `targetSum` at each level. The problem then becomes finding a path to a leaf where the remaining `targetSum` becomes zero.

The algorithm looks like this:
1.  **Base Case (Failure):** If the current node is `NULL`, this path is a dead end. It cannot contribute to a solution, so we return `false`.
2.  **Update State:** Subtract the current node's value from the `targetSum`.
3.  **Base Case (Success):** Check if the current node is a leaf (it has no left and no right child). If it is, and the updated `targetSum` is now `0`, we have found a valid path. We can return `true`.
4.  **Recursive Step (Explore):** If it's not a leaf, we continue the search. We recursively call the function on the left child with the updated sum. If that call returns `true`, we have found a solution and can immediately pass this `true` value up the call stack.
5.  If the left child did not yield a solution, we do the same for the right child.
6.  **Backtrack:** If neither the left nor the right recursive call finds a solution, it means the current node is not part of a valid path. The function simply returns `false`. The return from the function call *is* the backtracking step—the call stack unwinds automatically, restoring the state of the parent node.

### Step 3: The C Implementation

This recursive logic translates into a concise and elegant C function. The call stack handles all the complex state management for us.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define the Tree Node structure
typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Helper to create a new tree node
TreeNode* new_tree_node(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// The recursive DFS function
bool has_path_sum(TreeNode* root, int sum) {
    // Base Case 1: A null node cannot be part of a path.
    if (root == NULL) {
        return false;
    }

    // Subtract the current node's value from the sum we're looking for.
    sum -= root->value;

    // Base Case 2: Check for success at a leaf node.
    // A leaf is a node with no children.
    if (root->left == NULL && root->right == NULL) {
        // If the remaining sum is 0, we found a valid path.
        return (sum == 0);
    }

    // Recursive Step: Explore the left and right subtrees.
    // If either subtree finds a valid path, the result is true.
    return has_path_sum(root->left, sum) || has_path_sum(root->right, sum);
}

int main() {
    // Create a sample tree:
    //      5
    //     / \
    //    4   8
    //   /   / \
    //  11  13  4
    // /  \      \
    // 7   2      1
    TreeNode *root = new_tree_node(5);
    root->left = new_tree_node(4);
    root->right = new_tree_node(8);
    root->left->left = new_tree_node(11);
    root->left->left->left = new_tree_node(7);
    root->left->left->right = new_tree_node(2);
    root->right->left = new_tree_node(13);
    root->right->right = new_tree_node(4);
    root->right->right->right = new_tree_node(1);

    int targetSum = 22; // Path is 5 -> 4 -> 11 -> 2

    if (has_path_sum(root, targetSum)) {
        printf("A path with sum %d was found.\n", targetSum);
    } else {
        printf("No path with sum %d was found.\n", targetSum);
    }
    
    // Free tree memory... (omitted for brevity)
    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N), where N is the number of nodes in the tree. In the worst case, we must visit every node to confirm that no path exists.
*   **Space Complexity:** O(H), where H is the height of the tree. This space is consumed by the function call stack. For a perfectly balanced tree, H is O(log N). For a completely unbalanced (skewed) tree, H can be O(N) in the worst case. This is a key trade-off when compared to BFS, whose space complexity is O(W), the maximum width of the tree.

DFS, especially when implemented with recursion, is the go-to pattern for problems that require exploring the full depth of a state space. The implicit LIFO (Last-In, First-Out) nature of the call stack makes it the perfect tool for remembering which paths to try next after a current path is exhausted.```

Of course. Here is the next document in the series, which builds directly on the concepts of DFS to introduce the **Backtracking Pattern**.

---

# The Backtracking Pattern

Backtracking is a refined and powerful algorithmic technique built upon the principles of Depth-First Search. It is not used to find just one path or to visit every node; instead, it is an methodical approach for finding **all possible solutions** to a problem by incrementally building candidates and abandoning ("backtracking" from) any candidate as soon as it is determined that it cannot possibly lead to a valid solution. This pattern is the go-to strategy for problems that ask for all permutations, combinations, or subsets.

### The Problem: Generating All Subsets (The Power Set)

Let's explore backtracking with a fundamental combinatorial problem: "Given a set of unique integers, find all possible subsets (the power set)." For example, if the input is `[1, 2]`, the output should be `[[], [1], [2], [1, 2]]`.

### Step 1: Deconstruction and The Conceptual Hurdle

We begin with our standard analysis.
*   **Core Task:** Generate every possible combination of elements from the input set.
*   **Input:** An array of unique integers.
*   **Output:** A list containing all the subsets, which themselves are lists of integers.
*   **Constraints:** The input size `N` will be small (e.g., typically N <= 20). This is a massive clue. The number of subsets is 2^N, an exponential complexity that is only feasible for small inputs.

The conceptual challenge is immense for a simple iterative approach. How can we systematically construct every single subset without missing any or creating duplicates? A few nested loops might work for a fixed input size of 2 or 3, but the solution must scale for any `N`. This requires a dynamic process that can explore a branching set of choices.

### Step 2: The Insight: A Tree of Decisions

The key to solving this is to re-frame the problem as a sequence of decisions. For each number in our input array, we face a choice:
1.  **Include** the number in the subset we are currently building.
2.  **Exclude** the number from the subset.

This sequence of binary choices forms a decision tree. The traversal of this tree, exploring every possible path from the root to a leaf, will generate every possible subset. A leaf in this tree represents a state where we have made a decision for every single number.

This is a perfect scenario for a DFS-like recursive exploration. We can define a function that explores this decision tree. The "backtracking" step is the critical piece that makes this work. After we explore a path where we *included* an element, we must undo that choice to correctly explore the path where we *exclude* it.

This leads to the core mantra of backtracking: **Make a choice, recurse, and then undo the choice.**

The algorithm unfolds as follows:
1.  We define a recursive helper function that keeps track of the current `index` of the input array we are considering and the `current_subset` we are building.
2.  **Base Case:** When our `index` reaches the end of the array, we have made a decision for every element. The `current_subset` is now a complete, valid subset. We add a copy of it to our final list of solutions and return.
3.  **Recursive Step (The Two Choices):**
    a.  **The "Include" Path:** Add the element `arr[index]` to our `current_subset`. Then, make a recursive call to process the next element: `helper(index + 1, ...)`.
    b.  **The "Undo" Step (Backtrack):** After the recursive call returns, we must remove `arr[index]` from `current_subset`. This is the most crucial step. It cleans the slate, allowing us to explore the other branch from the same decision point.
    c.  **The "Exclude" Path:** Make a second recursive call, but this time without adding the element: `helper(index + 1, ...)`.

By following this "add, recurse, remove, recurse" pattern, we guarantee that every path in the decision tree is explored exactly once.

### Step 3: The C Implementation

The C implementation of backtracking can be verbose due to the manual memory management required for creating a list of lists. We must be careful about copying subsets into our results, as simply passing pointers would lead to all results pointing to the same modified `current_subset`.

```c
#include <stdio.h>
#include <stdlib.h>

// Global variables to store results. In a better design, this would be encapsulated.
int** results;
int* column_sizes;
int result_count;

// A temporary array to build the current subset.
int* current_subset;
int current_size;

void print_all_subsets() {
    printf("[\n");
    for (int i = 0; i < result_count; i++) {
        printf("  [");
        for (int j = 0; j < column_sizes[i]; j++) {
            printf("%d", results[i][j]);
            if (j < column_sizes[i] - 1) printf(", ");
        }
        printf("]\n");
    }
    printf("]\n");
}

void find_subsets_recursive(int* nums, int n, int index) {
    // Base Case: A decision has been made for every element.
    // Add the current subset to our results.
    if (index == n) {
        // Add a copy of the current_subset to results
        results[result_count] = (int*)malloc(current_size * sizeof(int));
        for (int i = 0; i < current_size; i++) {
            results[result_count][i] = current_subset[i];
        }
        column_sizes[result_count] = current_size;
        result_count++;
        return;
    }

    // --- The Backtracking Logic ---

    // Choice 1: Include the element at nums[index]
    current_subset[current_size++] = nums[index];
    find_subsets_recursive(nums, n, index + 1); // Recurse

    // UNDO: Backtrack by removing the element
    current_size--;

    // Choice 2: Exclude the element at nums[index]
    find_subsets_recursive(nums, n, index + 1); // Recurse
}

void subsets(int* nums, int n) {
    // There are 2^n subsets. We allocate memory for all of them.
    int num_subsets = 1 << n; // 2 to the power of n
    results = (int**)malloc(num_subsets * sizeof(int*));
    column_sizes = (int*)malloc(num_subsets * sizeof(int));
    current_subset = (int*)malloc(n * sizeof(int));
    result_count = 0;
    current_size = 0;

    find_subsets_recursive(nums, n, 0);

    print_all_subsets();

    // Clean up memory
    for (int i = 0; i < result_count; i++) free(results[i]);
    free(results);
    free(column_sizes);
    free(current_subset);
}

int main() {
    int nums[] = {1, 2, 3};
    int n = sizeof(nums) / sizeof(nums);
    subsets(nums, n);
    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N \* 2^N). There are 2^N subsets in total. For each of these subsets, we perform a copy operation that can take up to O(N) time.
*   **Space Complexity:** O(N \* 2^N). This is the space required to store the complete output. The recursion depth of the call stack contributes an additional O(N), but this is dwarfed by the size of the output itself.

Backtracking is the definitive pattern for problems requiring the generation of all valid configurations. Its power lies in its systematic, recursive exploration of a decision space, pruning branches implicitly by "undoing" choices to explore new possibilities.

Of course. Here is the next document in the series, focusing on a more specialized technique: the **Monotonic Stack Pattern**.

---

# The Monotonic Stack Pattern

The Monotonic Stack is a powerful and specific pattern used to solve problems where you need to find the "next greater" or "previous smaller" element for all elements in an array. It is a niche but highly efficient tool that can solve these problems in linear time. The core idea is to maintain a stack where the elements are always in a sorted order (either strictly increasing or decreasing), hence the name "monotonic".

### The Problem: Next Greater Element

Let's explore this pattern with its most representative problem: "Given an array of integers, for each element, find the first element to its right that is greater than it. If no such element exists, the answer is -1." For the input `[4, 5, 2, 10]`, the output should be `[5, 10, 10, -1]`.

### Step 1: Deconstruction and The Brute-Force Baseline

We begin with our standard analytical framework.
*   **Core Task:** For every element, find the next larger value to its right.
*   **Input:** An array of integers.
*   **Output:** A new array of the same size containing the corresponding "next greater" values.
*   **Constraints:** The input array `N` can be large, making a quadratic O(N²) solution too slow.

The brute-force solution is immediate and intuitive. For each element at index `i`, we can simply scan the rest of the array to its right, from index `i+1` to the end, looking for the first element that is larger. This requires two nested loops.

Here is the brute-force implementation in C.

```c
#include <stdio.h>
#include <stdlib.h>

void next_greater_element_brute_force(int arr[], int n) {
    int* results = (int*)malloc(n * sizeof(int));

    // Outer loop picks the element.
    for (int i = 0; i < n; i++) {
        results[i] = -1; // Default value if no greater element is found.
        // Inner loop scans to the right.
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[i]) {
                results[i] = arr[j];
                break; // Found the first one, so stop scanning for this 'i'.
            }
        }
    }

    printf("Results: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", results[i]);
    }
    printf("\n");
    free(results);
}
```

This code works perfectly but has a time complexity of O(N²). The **bottleneck is the inner loop**. For each element, we are potentially re-scanning a large portion of the array. The algorithm has no memory; the work done to scan for a greater element for `arr[i]` provides no help in finding the greater element for `arr[i+1]`.

### Step 2: The Insight: A Waiting List for a Savior

The inefficiency comes from forgetting past information. Let's re-frame the problem. As we iterate through the array, some elements are "waiting" for a greater element to appear. Who are these waiting elements? They are the ones we've passed but haven't yet found an answer for.

This sounds like a job for a **Stack**. We can use a stack to keep track of the indices of elements that are waiting. When we process a new element `current = arr[i]`, it might be the "savior" (the next greater element) for some of the elements waiting on the stack.

Here's the logic:
1.  Initialize an empty stack. This stack will store the **indices** of elements.
2.  Iterate through the array from left to right with index `i`. Let `current = arr[i]`.
3.  Now, look at the index at the top of the stack, let's call it `top_index`.
4.  **While** the stack is not empty AND `current` is greater than the element at `top_index` (`arr[top_index]`), we have found a solution! The `current` element is the next greater element for `arr[top_index]`. So, we set `result[top_index] = current`, and **pop** the index from the stack. We repeat this check with the new top of the stack.
5.  After the `while` loop finishes, it means either the stack is empty or `current` is less than or equal to the element at the new top. The waiting elements are still waiting.
6.  Now, push the current index `i` onto the stack. It is now one of the elements waiting for its own "next greater element".

This process naturally enforces a property on the stack: the values corresponding to the indices on the stack will always be in **monotonically decreasing** order from bottom to top. An element can only be pushed on top of a larger element. When a new `current` element arrives that is larger than the top, it breaks this property, forcing us to pop all smaller elements before it can be pushed.

### Step 3: The Optimized C Implementation

This logic can be implemented using a simple array-based stack.

```c
#include <stdio.h>
#include <stdlib.h>

// For simplicity, we assume N is within a reasonable limit for the stack size.
#define STACK_SIZE 10000

void next_greater_element_monotonic_stack(int arr[], int n) {
    int* results = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        results[i] = -1; // Initialize all results to -1.
    }

    // A simple stack implementation using an array.
    int stack[STACK_SIZE];
    int top = -1; // Stack is empty.

    // Iterate through the array.
    for (int i = 0; i < n; i++) {
        // While stack is not empty and current element is greater than
        // the element at the index on top of the stack.
        while (top != -1 && arr[i] > arr[stack[top]]) {
            int waiting_index = stack[top--]; // Pop the index.
            results[waiting_index] = arr[i];  // Set its result.
        }
        
        // Push the current index onto the stack.
        stack[++top] = i;
    }

    printf("Results: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", results[i]);
    }
    printf("\n");
    free(results);
}

int main() {
    int arr[] = {4, 5, 2, 10, 8};
    int n = sizeof(arr) / sizeof(arr);

    printf("Input:\n");
    printf("Brute Force -> ");
    next_greater_element_brute_force(arr, n); // Expected: [5, 10, 10, -1, -1]
    printf("Monotonic Stack -> ");
    next_greater_element_monotonic_stack(arr, n); // Expected: [5, 10, 10, -1, -1]

    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N). This is the non-intuitive but critical takeaway. Although we have a `while` loop nested inside a `for` loop, each element's index is pushed onto the stack exactly once and popped from the stack exactly once. Every number is processed a constant number of times. Therefore, the total time complexity is linear.
*   **Space Complexity:** O(N). In the worst-case scenario (an array that is strictly decreasing, like `[10, 9, 8, 7]`), every index will be pushed onto the stack before any are popped, leading to a space usage proportional to the input size.

The Monotonic Stack pattern provides a remarkably efficient solution by maintaining a structured "waiting list" that avoids the redundant comparisons of the brute-force approach.

Of course. Here is the final document in the series, covering the powerful **Union-Find Pattern**.

---

# The Union-Find Pattern

The Union-Find pattern, also known as the Disjoint Set Union (DSU) data structure, is a specialized tool for solving problems related to partitioning a set of elements into a number of disjoint (non-overlapping) subsets. It is exceptionally efficient at tracking connectivity. If you can frame your problem in terms of "grouping" or "connecting" elements and then asking "are these two elements in the same group?", the Union-Find pattern is likely the optimal solution.

### The Problem: Number of Connected Components

Let's explore this pattern with a quintessential graph problem: "You have a graph of `n` nodes, labeled from `0` to `n-1`. You are given an array of `edges` where `edges[i] = [a, b]` indicates that there is an edge between nodes `a` and `b`. Find the number of connected components in the graph."

### Step 1: Deconstruction and A Traversal-Based Baseline

We begin with our analysis.
*   **Core Task:** Count the number of distinct, unconnected islands or groups of nodes.
*   **Input:** An integer `n` for the number of nodes, and a list of `edges`.
*   **Output:** A single integer representing the count of components.
*   **Constraints:** The number of nodes and edges can be large, requiring an efficient solution.

A standard approach for this problem is to use a graph traversal algorithm like DFS or BFS. We can maintain a `visited` array for all `n` nodes. We would iterate from node `0` to `n-1`. If we find a node that we haven't visited yet, we know it's the start of a new component. We increment our component counter and then begin a full traversal (DFS or BFS) from this node, marking every node we can reach as visited. When the traversal is complete, we continue our main loop, searching for the next unvisited node.

This approach is perfectly valid and has a time complexity of O(V + E) (Vertices + Edges). However, the Union-Find pattern offers a different, data-structure-centric way of thinking about connectivity that can be even faster and is more adaptable for problems where connections are made dynamically.

### Step 2: The Insight: Modeling Groups as a Forest

Instead of exploring the graph, let's directly model the "grouping" process.
1.  **Initialization:** We start by assuming every node is its own isolated component. We have `n` components.
2.  **Processing Edges:** We then iterate through the `edges`. An edge `[a, b]` is a piece of information telling us that `a` and `b` belong to the same component. Our task is to **merge** the groups containing `a` and `b`.
3.  **Merging Logic:** If `a` and `b` were already in the same group, this edge is redundant and changes nothing. If they were in different groups, merging them reduces the total number of components by one.

This leads to the two fundamental operations that give the pattern its name:
*   **`find(i)`:** Determines which group (or set) element `i` belongs to. It does this by returning a single, consistent "representative" or "root" element for that entire group.
*   **`union(i, j)`:** Merges the groups containing elements `i` and `j`.

We can represent these groups as a forest of trees using a simple `parent` array. `parent[i]` stores the parent of element `i`. An element is the representative (the root of its tree) if it is its own parent (`parent[i] == i`). The `find(i)` operation works by traversing up from `i` to its parent, and its parent's parent, until it reaches the root. The `union(i, j)` operation first finds the roots of `i` and `j`. If the roots are different, it merges the two trees by setting one root's parent to be the other.

**Crucial Optimizations:** A naive implementation of this can lead to very tall, skinny trees, making the `find` operation slow (O(N)). Two optimizations transform this data structure into something incredibly fast:
1.  **Union by Size/Rank:** When merging two trees, instead of arbitrarily linking them, we always attach the smaller tree to the root of the larger tree. This helps keep the trees from getting too deep. We track the size or rank of each tree to make this decision.
2.  **Path Compression:** During a `find(i)` operation, after we locate the ultimate root of the set, we re-trace our steps and make every node on that path a direct child of the root. This dramatically flattens the tree, making future `find` operations for any of those nodes nearly instantaneous.

### Step 3: The Optimized C Implementation

Here is a C implementation of the Union-Find data structure with both key optimizations.

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* parent;
    int* size;
    int count; // Number of disjoint sets
} DSU;

// Initializes the DSU structure for 'n' elements.
DSU* create_dsu(int n) {
    DSU* dsu = (DSU*)malloc(sizeof(DSU));
    dsu->parent = (int*)malloc(n * sizeof(int));
    dsu->size = (int*)malloc(n * sizeof(int));
    dsu->count = n; // Initially, n elements means n sets.

    for (int i = 0; i < n; i++) {
        dsu->parent[i] = i; // Each element is its own parent.
        dsu->size[i] = 1;   // Each set is of size 1.
    }
    return dsu;
}

// Find the representative of a set, with path compression.
int find(DSU* dsu, int i) {
    if (dsu->parent[i] == i) {
        return i;
    }
    // Path Compression: Set the parent directly to the root.
    dsu->parent[i] = find(dsu, dsu->parent[i]);
    return dsu->parent[i];
}

// Union of two sets, with union by size.
void union_sets(DSU* dsu, int i, int j) {
    int root_i = find(dsu, i);
    int root_j = find(dsu, j);

    if (root_i != root_j) { // Only union if they are in different sets.
        // Union by Size: Attach the smaller tree to the root of the larger tree.
        if (dsu->size[root_i] < dsu->size[root_j]) {
            dsu->parent[root_i] = root_j;
            dsu->size[root_j] += dsu->size[root_i];
        } else {
            dsu->parent[root_j] = root_i;
            dsu->size[root_i] += dsu->size[root_j];
        }
        dsu->count--; // A successful union decreases the number of sets.
    }
}

void free_dsu(DSU* dsu) {
    free(dsu->parent);
    free(dsu->size);
    free(dsu);
}

int count_components(int n, int** edges, int num_edges) {
    DSU* dsu = create_dsu(n);

    for (int i = 0; i < num_edges; i++) {
        union_sets(dsu, edges[i], edges[i]);
    }

    int result = dsu->count;
    free_dsu(dsu);
    return result;
}

int main() {
    int n = 5;
    int edges_data[] = {{0, 1}, {1, 2}, {3, 4}};
    int num_edges = 3;
    
    // C requires a bit of work to handle 2D arrays for functions.
    int** edges = (int**)malloc(num_edges * sizeof(int*));
    for(int i=0; i<num_edges; i++) {
        edges[i] = (int*)malloc(2 * sizeof(int));
        edges[i] = edges_data[i];
        edges[i] = edges_data[i];
    }
    
    // Components are {0, 1, 2} and {3, 4}. Expected count is 2.
    int components = count_components(n, edges, num_edges);
    printf("Number of connected components: %d\n", components);

    for(int i=0; i<num_edges; i++) free(edges[i]);
    free(edges);

    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(E \* α(N)). With both path compression and union by size/rank, the amortized time complexity of the `find` and `union` operations is nearly constant. It is technically O(α(N)), where α(N) is the extremely slow-growing inverse Ackermann function. For any conceivable input size, this function's value is less than 5. Therefore, the total time to process `E` edges is effectively linear.
*   **Space Complexity:** O(N). We need two arrays of size `N` to store the `parent` and `size` information for each node.

The Union-Find pattern provides an exceptionally fast and elegant solution for problems involving dynamic connectivity. It abstracts the complex graph structure into a simple set-based model, allowing for near-constant time updates and queries about group membership.

Excellent choice. We will begin with **Path 1**, taking a deeper dive into the Sliding Window pattern, and then proceed to the specialized patterns.

This document explores the more advanced and common variant of the pattern: the **Dynamic Sliding Window**.

---

# The Dynamic Sliding Window Pattern

While the fixed-size window is a good introduction, the true power of the Sliding Window pattern is revealed when the window size is dynamic. In these problems, the window does not maintain a constant size; instead, it grows and shrinks based on a specific condition. This makes it a tool for finding the smallest or largest subarray that satisfies some property.

### The Problem: Smallest Subarray with a Given Sum

Let's analyze a classic problem that requires a dynamic window: "Given an array of positive integers and a positive number `S`, find the length of the smallest contiguous subarray whose sum is greater than or equal to `S`. Return 0 if no such subarray exists."

### Step 1: Deconstruction and The Brute-Force Baseline

We apply our analytical framework.
*   **Core Task:** Find the minimum length of a contiguous subarray that meets a sum condition.
*   **Input:** An array of positive integers and a target sum `S`.
*   **Output:** An integer representing the minimum length.
*   **Constraints:** An O(N²) solution will be too slow for large inputs. The "positive integers" constraint is important; it guarantees that adding an element will always increase the sum.

The brute-force solution is to generate every possible contiguous subarray, check if its sum is `>= S`, and keep track of the minimum length found so far. This is a classic nested loop approach.

```c
#include <stdio.h>
#include <limits.h> // For INT_MAX

int find_smallest_subarray_brute_force(int arr[], int n, int S) {
    int min_length = INT_MAX;

    // Outer loop selects the starting point of the subarray.
    for (int i = 0; i < n; i++) {
        int current_sum = 0;
        // Inner loop expands the subarray to the right.
        for (int j = i; j < n; j++) {
            current_sum += arr[j];
            // If the condition is met, we have a candidate subarray.
            if (current_sum >= S) {
                int current_length = j - i + 1;
                if (current_length < min_length) {
                    min_length = current_length;
                }
                break; // Optimization: no need to make this subarray longer.
            }
        }
    }
    return (min_length == INT_MAX) ? 0 : min_length;
}
```
The time complexity is O(N²), and the bottleneck is the re-summation of elements in the inner loop. The "contiguous subarray" signal is screaming for a sliding window, but we cannot use a fixed size. The window must adapt.

### Step 2: The Insight: The Grow and Shrink Strategy

The core logic of the dynamic window is a two-phase process:
1.  **Grow the Window:** We expand the window by moving its right end (`window_end`) forward. We keep doing this, adding the new element's value to a running sum, until the condition (`window_sum >= S`) is met.
2.  **Shrink the Window:** Once the condition is met, we have a valid—but not necessarily minimal—subarray. Now, we must try to optimize it. We record its length and then attempt to shrink the window from the left by moving its start (`window_start`) forward. After each shrink, we check if the condition still holds. If it does, we have found an even smaller valid subarray, so we update our minimum length. We continue shrinking until the condition is no longer met.

When shrinking breaks the condition, the window is no longer valid. What now? We go back to Step 1. We start growing the window from the right again, looking for the next valid subarray. This rhythmic cycle of growing until valid, then shrinking until invalid, continues until we have passed through the entire array.

### Step 3: The Optimized C Implementation

This grow-and-shrink logic is implemented with a `for` loop to control the right end of the window and a nested `while` loop to control the left end.

```c
#include <stdio.h>
#include <limits.h> // For INT_MAX

int find_smallest_subarray_dynamic_window(int arr[], int n, int S) {
    int min_length = INT_MAX;
    int window_sum = 0;
    int window_start = 0;

    // The 'window_end' pointer expands the window to the right.
    for (int window_end = 0; window_end < n; window_end++) {
        // --- GROW PHASE ---
        window_sum += arr[window_end];

        // --- SHRINK PHASE ---
        // Once the condition is met, try to shrink the window from the left.
        while (window_sum >= S) {
            // We have a valid window. Update our minimum length.
            int current_length = window_end - window_start + 1;
            if (current_length < min_length) {
                min_length = current_length;
            }

            // Shrink the window by removing the leftmost element and advancing the start pointer.
            window_sum -= arr[window_start];
            window_start++;
        }
    }

    // If min_length was never updated, no valid subarray was found.
    return (min_length == INT_MAX) ? 0 : min_length;
}

int main() {
    int arr[] = {2, 1, 5, 2, 3, 2};
    int S = 7;
    int n = sizeof(arr) / sizeof(arr);

    int result = find_smallest_subarray_dynamic_window(arr, n, S);
    // Walkthrough:
    // -> sum=8 >= 7. length=3. min_length=3. Shrink: remove 2 ->, sum=6 < 7. Stop shrinking.
    // -> sum=8 >= 7. length=3. min_length=3. Shrink: remove 1 ->, sum=7 >= 7. length=2. min_length=2. Shrink: remove 5 ->, sum=2 < 7. Stop.
    // ...and so on.
    printf("The length of the smallest subarray is: %d\n", result); // Expected: 2 (for)

    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N). This is the key benefit. Although there is a nested loop, it is not O(N²). The `window_end` pointer moves forward N times, and the `window_start` pointer also moves forward N times at most. Each element is visited by each pointer exactly once. The complexity is effectively O(N + N), which is O(N).
*   **Space Complexity:** O(1). We only use a few variables to track the window's state, regardless of the input size.

The dynamic sliding window is an elegant and highly efficient pattern. It solves a class of optimization problems (finding the min/max subarray satisfying a condition) by intelligently expanding to find a valid solution and then contracting to find the optimal one.

Of course. Now we will begin **Path 3** with the first of the specialized patterns: the **Cyclic Sort Pattern**.

---

# The Cyclic Sort Pattern

The Cyclic Sort pattern is a fascinating and highly efficient technique for a specific class of problems: those involving an array containing numbers in a given range. Typically, the problem will state that the input array contains numbers from `1` to `N`, and it may have duplicates or be missing numbers. The pattern provides a way to sort this array in-place in linear time, which often solves the problem as a side effect.

### The Problem: Find the Missing Number

Let's use a classic problem to demonstrate this pattern: "Given an array containing `n` distinct numbers taken from the range `0` to `n`, find the one number that is missing from the array." For example, if the input is `[3, 0, 1]`, `n` is 3, the range is `[0, 1, 2, 3]`, and the missing number is `2`.

### Step 1: Deconstruction and The Naive Approaches

We begin with our analysis.
*   **Core Task:** Find the single missing integer in a sequence.
*   **Input:** An unsorted array of `n` distinct numbers. The key signal is the range: `0` to `n`.
*   **Output:** The single missing integer.
*   **Constraints:** An optimal solution should be efficient in both time and space, ideally O(N) time and O(1) space.

A common brute-force approach would be to sort the array first. An array `[3, 0, 1]` would become `[0, 1, 3]`. We could then iterate through this sorted array and find the first index that does not match its value. Here, index `2` contains the value `3`. Therefore, `2` is the missing number. This solution works, but its time complexity is O(N log N) due to the sort.

Another common approach uses a hash set to achieve O(N) time. We can add all numbers from the input array to a hash set. Then, we can loop from `0` to `n` and check if each number exists in the set. The first number we cannot find is our missing number. This is fast, but it requires O(N) extra space for the hash set, which violates the ideal O(1) space constraint.

### Step 2: The Insight: A Number's Correct Home

The core insight of Cyclic Sort is that for an array containing numbers from `0` to `n`, if there were no missing numbers and the array was sorted, the number `x` would be at index `x`. For example, in `[0, 1, 2, 3]`, the number `0` is at index `0`, `1` is at index `1`, and so on.

This gives us a powerful idea: we can iterate through the array and try to place each number in its correct home.

The algorithm works as follows:
1.  Initialize a pointer `i` at the beginning of the array.
2.  Look at the number at the current position, `num = arr[i]`.
3.  We know that the correct index for this number is `j = num`.
4.  If the number is not already at its correct index (`num != arr[j]`), we swap `arr[i]` with `arr[j]`. This places the number `num` one step closer to its correct home. **Crucially, we do not increment `i` after a swap**. We stay at the current index `i` because the new number that just arrived might also be in the wrong place. We must process it before moving on.
5.  If the number `num` is already at its correct index (`num == i`), it's where it belongs. We can now move on to the next position by incrementing `i`.
6.  There's one edge case: what if `num` is out of our range? In this specific problem, the number `n` will be present, but its "correct" index `n` is out of bounds for an array of size `n`. We should simply ignore this number and move on.

After this first pass, the array will be *almost* sorted. Every number will be at its correct index, except for the index where the missing number should have been. That index will hold the out-of-place number `n`.

The final step is to iterate through the modified array one more time. The first index `i` where `arr[i] != i` is the index of the missing number. If we iterate through the whole array and find every number in its correct place, it must mean the missing number is `n`.

### Step 3: The Optimized C Implementation

```c
#include <stdio.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int find_missing_number_cyclic_sort(int arr[], int n) {
    int i = 0;
    while (i < n) {
        int num = arr[i];
        // The number should be at index 'num'.
        // We only swap if the number is within the valid index range
        // and is not already at its correct place.
        if (num < n && num != i) {
            swap(&arr[i], &arr[num]);
        } else {
            // The number is either out of range (it's 'n') or already correct.
            // Move to the next index.
            i++;
        }
    }

    // Now, find the first discrepancy.
    for (int j = 0; j < n; j++) {
        if (arr[j] != j) {
            return j; // This is the missing number's index.
        }
    }

    // If all numbers from 0 to n-1 are in their correct places,
    // then 'n' must be the missing number.
    return n;
}

int main() {
    int arr[] = {3, 0, 1};
    int n = sizeof(arr) / sizeof(arr);
    int missing = find_missing_number_cyclic_sort(arr, n);
    printf("The missing number is: %d\n", missing); // Expected: 2

    int arr2[] = {9, 6, 4, 2, 3, 5, 7, 0, 1};
    int n2 = sizeof(arr2) / sizeof(arr2);
    missing = find_missing_number_cyclic_sort(arr2, n2);
    printf("The missing number is: %d\n", missing); // Expected: 8

    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(N). Although we have a nested-like structure with the `while` loop, the logic is more subtle. In the worst case, each number is swapped at most once before it lands in its correct position. The total number of swaps cannot exceed `N-1`. Therefore, the first pass is O(N). The second pass to find the discrepancy is also O(N). The total complexity is O(N + N), which is O(N).
*   **Space Complexity:** O(1). All operations are performed in-place within the original array. We use no auxiliary data structures that scale with the input size.

Cyclic Sort is a non-comparison-based sorting algorithm. It doesn't care about the relative order of elements; it only cares about placing each element in its one "correct" home. This makes it a uniquely powerful and efficient pattern for a specific but surprisingly common category of array problems.

Of course. Here is the next document in our series on specialized patterns: **Topological Sort**.

---
# The Topological Sort Pattern

Topological Sort is not a typical sorting algorithm in the sense of ordering numbers. Instead, it is an algorithm for ordering the vertices of a **Directed Acyclic Graph (DAG)**. It produces a linear ordering of vertices such that for every directed edge from vertex `u` to vertex `v`, vertex `u` comes before vertex `v` in the ordering. This pattern is essential for problems involving dependencies, scheduling, or prerequisite tasks.

### The Problem: Course Schedule

Let's explore this pattern with a very common and direct application: "You are given a total of `numCourses` courses you have to take, labeled from `0` to `numCourses-1`. You are given an array `prerequisites` where `prerequisites[i] = [a, b]` indicates that you must take course `b` before you can take course `a`. Is it possible for you to finish all courses?"

### Step 1: Deconstruction and The Graph Model

We begin with our analysis.
*   **Core Task:** Determine if a set of tasks with dependencies can be completed. This is equivalent to asking if the dependency graph contains a cycle.
*   **Input:** A number of courses and a list of prerequisite pairs.
*   **Output:** A boolean value (`true` if a valid ordering exists, `false` otherwise).
*   **Constraints:** The number of courses and prerequisites can be large, so an efficient graph traversal is needed.

The first and most critical step is to re-frame the problem as a graph. The courses are the **vertices** of the graph. The prerequisite pairs represent the **directed edges**. If you must take course `b` before `a`, we draw a directed edge from `b` to `a` (`b -> a`). This edge signifies that `b` is a prerequisite for `a`.

Once we have this graph model, the problem becomes clear: a valid schedule is possible if and only if the graph has no cycles. If there is a cycle, for example `A -> B -> C -> A`, it represents a circular dependency that is impossible to satisfy. Topological Sort is an algorithm that will either produce a valid ordering or detect a cycle.

### Step 2: The Insight: The "No Prerequisites" Rule

How can we start building a valid ordering? The only logical place to start is with a course that has **no prerequisites**. If every course requires another, we are in a cycle.

This leads to the core logic of the Kahn's algorithm version of Topological Sort:
1.  **Model the Graph:** First, we must build a formal graph representation. We need two key pieces of information:
    *   An **adjacency list**, where for each vertex, we have a list of its neighbors (i.e., `graph[u]` contains a list of all `v` for edges `u -> v`).
    *   An **in-degree array**, where `in_degree[i]` stores the count of incoming edges for vertex `i`. This is the number of prerequisites for course `i`.

2.  **Find the Sources:** Identify all vertices with an in-degree of `0`. These are our starting points, the courses with no prerequisites. We add all of them to a **queue**.

3.  **Process and Detach:** We begin a loop that continues as long as the queue is not empty.
    a.  **Dequeue** a vertex `u` from the front of the queue. This is the next course in our valid topological order.
    b.  Add `u` to our sorted list.
    c.  For each neighbor `v` of `u` (i.e., for each course that has `u` as a prerequisite), we "detach" the edge. We do this by **decrementing the in-degree of `v`**.
    d.  After decrementing, if the in-degree of `v` becomes `0`, it means all of its prerequisites have now been met. It has become a new source, so we **enqueue** `v`.

4.  **Check for Cycles:** After the loop finishes, we check if the number of courses in our sorted list is equal to the total number of courses.
    *   If they are equal, we have successfully processed every vertex, and we have a valid topological ordering. No cycle exists.
    *   If the count is less than the total number of courses, it means there were some vertices that never made it into the queue. This can only happen if their in-degree never reached `0`, which implies they are part of a cycle.

### Step 3: The C Implementation

The C implementation requires manually building the graph structures (adjacency list and in-degree array) and a queue.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// For simplicity, we assume max courses for static allocation.
// A dynamic approach would be better for a real application.
#define MAX_COURSES 2000

typedef struct {
    int* neighbors;
    int count;
} AdjacencyList;

bool can_finish(int numCourses, int** prerequisites, int num_prerequisites) {
    // 1. Build the graph model
    AdjacencyList graph[MAX_COURSES];
    int in_degree[MAX_COURSES] = {0};
    for(int i=0; i<numCourses; i++) {
        graph[i].neighbors = NULL;
        graph[i].count = 0;
    }

    for (int i = 0; i < num_prerequisites; i++) {
        int course = prerequisites[i];
        int prereq = prerequisites[i];
        
        // Add edge prereq -> course
        graph[prereq].count++;
        graph[prereq].neighbors = realloc(graph[prereq].neighbors, graph[prereq].count * sizeof(int));
        graph[prereq].neighbors[graph[prereq].count - 1] = course;
        
        in_degree[course]++;
    }

    // 2. Find all sources (in-degree == 0) and add to queue
    int queue[MAX_COURSES];
    int head = 0, tail = 0;
    for (int i = 0; i < numCourses; i++) {
        if (in_degree[i] == 0) {
            queue[tail++] = i;
        }
    }

    // 3. Process the sources
    int sorted_count = 0;
    while (head < tail) {
        int u = queue[head++]; // Dequeue
        sorted_count++;

        // For each neighbor of u...
        for (int i = 0; i < graph[u].count; i++) {
            int v = graph[u].neighbors[i];
            in_degree[v]--; // ...decrement in-degree.
            
            // If it becomes a new source...
            if (in_degree[v] == 0) {
                queue[tail++] = v; // ...enqueue it.
            }
        }
    }

    // Free allocated memory
    for(int i=0; i<numCourses; i++) {
        if(graph[i].neighbors != NULL) free(graph[i].neighbors);
    }
    
    // 4. Check for cycles
    return sorted_count == numCourses;
}

int main() {
    int numCourses = 4;
    int prereqs_data[] = {{1, 0}, {2, 0}, {3, 1}, {3, 2}};
    int num_prereqs = 4;
    
    // Convert to int** for function call
    int** prereqs = (int**)malloc(num_prereqs * sizeof(int*));
    for(int i=0; i<num_prereqs; i++) prereqs[i] = prereqs_data[i];
    
    // Possible order: 0, 1, 2, 3 or 0, 2, 1, 3
    if (can_finish(numCourses, prereqs, num_prereqs)) {
        printf("It is possible to finish all courses.\n");
    } else {
        printf("It is not possible to finish all courses.\n");
    }

    // Example with a cycle: 1 -> 0 -> 1
    int numCourses2 = 2;
    int prereqs_data2[] = {{1,0}, {0,1}};
    for(int i=0; i<2; i++) prereqs[i] = prereqs_data2[i];
    
    if (can_finish(numCourses2, prereqs, 2)) {
        printf("It is possible to finish all courses.\n");
    } else {
        printf("It is not possible to finish all courses.\n");
    }

    free(prereqs);
    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** O(V + E), where V is the number of vertices (courses) and E is the number of edges (prerequisites). We visit each vertex and each edge exactly once. The graph building takes O(E), finding initial sources takes O(V), and the main loop processes each vertex and its outgoing edges once, which is O(V + E) in total.
*   **Space Complexity:** O(V + E). We need space for the adjacency list (O(E)), the in-degree array (O(V)), and the queue (O(V) in the worst case).

Topological Sort is the definitive algorithm for resolving dependencies. By modeling the problem as a graph and systematically processing nodes that have no unmet prerequisites, it provides a robust and efficient way to either find a valid linear order or prove that one is impossible due to a cyclical dependency.

Of course. Here is the final document in our series on specialized, high-impact patterns: the **Two Heaps Pattern**.

---

# The Two Heaps Pattern

The Two Heaps pattern is a clever and powerful technique used to solve problems that involve partitioning a set of elements into two parts and efficiently finding the "middle" elements. It is the definitive pattern for problems like finding the median of a data stream, where we need to maintain a balanced, partially-sorted structure as new data arrives.

### The Problem: Find the Median of a Data Stream

Let's explore this pattern with its most famous application: "Design a data structure that supports the following two operations: `addNum(int num)` - Add an integer from a data stream to the data structure. `findMedian()` - Return the median of all elements seen so far."

### Step 1: Deconstruction and The Naive Approaches

We begin our analysis.
*   **Core Task:** Efficiently maintain and report the median of a continuously growing collection of numbers.
*   **Input:** A stream of integers, arriving one at a time.
*   **Output:** The median value after each `addNum` operation.
*   **Constraints:** The number of calls to `addNum` can be very large, so each operation must be faster than O(N).

A simple approach would be to store all the numbers in a list. Every time `findMedian` is called, we could sort the entire list and find the middle element(s). Adding a number would be O(1) (if we append), but finding the median would be O(N log N), which is far too slow.

Alternatively, we could keep the list sorted at all times. Finding the median would then be an O(1) operation. However, inserting a new number into its correct position in a sorted array takes O(N) time. Again, this is too slow for a large stream of data.

The bottleneck is clear: we need a way to find the middle elements without the high cost of a full sort or a linear-time insertion.

### Step 2: The Insight: Two Halves of a Whole

The median is the number that conceptually divides a sorted set into two equal halves.
*   A **smaller half**, containing all the numbers less than or equal to the median.
*   A **larger half**, containing all the numbers greater than or equal to the median.

This structure is the key insight. What if we use two separate data structures to maintain these two halves? The ideal data structures for this are **Heaps**.
1.  We can use a **Max-Heap** to store the smaller half of the numbers. A Max-Heap is perfect because it gives us instant O(1) access to its largest element. The largest element in the smaller half is a candidate for the median.
2.  We can use a **Min-Heap** to store the larger half of the numbers. A Min-Heap is perfect because it gives us O(1) access to its smallest element. The smallest element in the larger half is the other candidate for the median.

By maintaining these two heaps, the median will always be either the top of the Max-Heap or the average of the tops of both heaps.

To make this work, we must enforce two rules at all times:
*   **The Property Rule:** Every number in the Max-Heap (`small_half`) must be less than or equal to every number in the Min-Heap (`large_half`).
*   **The Balance Rule:** The two heaps must be kept as close in size as possible. We can allow the Max-Heap (`small_half`) to hold at most one more element than the Min-Heap.

The algorithm for `addNum(num)` is a dance to maintain these two rules:
1.  **Offer to the Max-Heap:** First, add the new `num` to the `small_half` (Max-Heap).
2.  **Maintain the Property:** The number we just added might be larger than some numbers that are supposed to be in the `large_half`. To fix this, we take the largest element from `small_half` (its root) and move it to the `large_half` (Min-Heap).
3.  **Maintain the Balance:** The previous step might have made the `large_half` too big. If `large_half` now has more elements than `small_half`, we take the smallest element from `large_half` (its root) and move it back to `small_half`.

After these steps, both the property and balance rules are restored, and we are ready to find the median.

### Step 3: The C Implementation

This is a complex implementation in C, as it requires building two separate heap structures from scratch.

```c
#include <stdio.h>
#include <stdlib.h>

// --- Generic Heap Implementation ---
typedef struct {
    int* arr;
    int size;
    int capacity;
    int is_max_heap; // 1 for max-heap, 0 for min-heap
} Heap;

void swap(int* a, int* b) { int t = *a; *a = *b; *b = t; }

Heap* create_heap(int capacity, int is_max_heap) {
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->arr = (int*)malloc(capacity * sizeof(int));
    h->size = 0;
    h->capacity = capacity;
    h->is_max_heap = is_max_heap;
    return h;
}

void heapify_down(Heap* h, int i) {
    int extreme = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (h->is_max_heap) {
        if (left < h->size && h->arr[left] > h->arr[extreme]) extreme = left;
        if (right < h->size && h->arr[right] > h->arr[extreme]) extreme = right;
    } else {
        if (left < h->size && h->arr[left] < h->arr[extreme]) extreme = left;
        if (right < h->size && h->arr[right] < h->arr[extreme]) extreme = right;
    }
    if (extreme != i) {
        swap(&h->arr[i], &h->arr[extreme]);
        heapify_down(h, extreme);
    }
}

void heapify_up(Heap* h, int i) {
    int p = (i - 1) / 2;
    if (h->is_max_heap) {
        if (i > 0 && h->arr[i] > h->arr[p]) { swap(&h->arr[i], &h->arr[p]); heapify_up(h, p); }
    } else {
        if (i > 0 && h->arr[i] < h->arr[p]) { swap(&h->arr[i], &h->arr[p]); heapify_up(h, p); }
    }
}

void insert(Heap* h, int val) {
    if (h->size == h->capacity) return; // Or realloc
    h->size++;
    h->arr[h->size - 1] = val;
    heapify_up(h, h->size - 1);
}

int extract_top(Heap* h) {
    if (h->size == 0) return -1; // Error
    int top = h->arr;
    h->arr = h->arr[h->size - 1];
    h->size--;
    heapify_down(h, 0);
    return top;
}

int get_top(Heap* h) { return h->size > 0 ? h->arr : -1; }

// --- MedianFinder Implementation ---
typedef struct {
    Heap* small_half; // Max-Heap
    Heap* large_half; // Min-Heap
} MedianFinder;

MedianFinder* create_median_finder() {
    MedianFinder* mf = (MedianFinder*)malloc(sizeof(MedianFinder));
    // Assuming max 10000 elements in stream for capacity
    mf->small_half = create_heap(10000, 1);
    mf->large_half = create_heap(10000, 0);
    return mf;
}

void add_num(MedianFinder* mf, int num) {
    insert(mf->small_half, num);
    insert(mf->large_half, extract_top(mf->small_half));
    if (mf->large_half->size > mf->small_half->size) {
        insert(mf->small_half, extract_top(mf->large_half));
    }
}

double find_median(MedianFinder* mf) {
    if (mf->small_half->size > mf->large_half->size) {
        return (double)get_top(mf->small_half);
    } else {
        return (get_top(mf->small_half) + get_top(mf->large_half)) / 2.0;
    }
}

int main() {
    MedianFinder* mf = create_median_finder();
    add_num(mf, 1);
    add_num(mf, 2);
    printf("Median is: %.1f\n", find_median(mf)); // (1+2)/2 = 1.5
    add_num(mf, 3);
    printf("Median is: %.1f\n", find_median(mf)); // 2.0
    add_num(mf, 4);
    printf("Median is: %.1f\n", find_median(mf)); // (2+3)/2 = 2.5
    add_num(mf, 5);
    printf("Median is: %.1f\n", find_median(mf)); // 3.0
    // free memory...
    return 0;
}
```

#### Final Analysis

*   **Time Complexity:** Adding a number takes O(log N) time. The heap insertions and extractions all operate on heaps of size N/2, which is O(log N). Finding the median is an O(1) operation as it only involves looking at the tops of the heaps.
*   **Space Complexity:** O(N), where N is the total number of elements in the stream, as we need to store all of them in the two heaps.

The Two Heaps pattern provides a highly optimized solution for maintaining a central dividing point in a dynamic set of data. It brilliantly balances the need for partial order with the efficiency of logarithmic-time insertions, solving a class of problems that are intractable with simpler data structures.


### 1. Longest Substring Without Repeating Characters

*   **Problem Description:** Given a string, find the length of the longest substring that does not contain any repeating characters. For `s = "abcabcbb"`, the answer is `"abc"`, with the length of 3.

*   **Key Signals & Analysis:** The problem asks for the **longest substring** that satisfies a specific condition (no repeating characters). "Substring" implies a **contiguous** block of elements. The combination of "longest" and "contiguous" is a massive signal for the **Dynamic Sliding Window** pattern. The window will grow as long as the condition is met and shrink when it's violated.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Sliding Window.
    2.  **Tool:** Use a hash map (or an integer array of size 256 for ASCII) to keep track of the characters currently inside the window and their last seen indices.
    3.  **Approach:**
        *   Initialize a `window_start` pointer at 0 and a `max_length` variable.
        *   Iterate through the string with a `window_end` pointer.
        *   For each character `c` at `window_end`:
            *   **Check for Violation:** If `c` is already in our hash map and its last seen index is *within* the current window (i.e., `index >= window_start`), the window is invalid.
            *   **Shrink:** To fix the violation, we must shrink the window from the left. Move `window_start` to `index + 1`, effectively discarding the previous occurrence of `c` and everything before it.
            *   **Update:** Add/update the character `c` with its current index `window_end` in the hash map.
            *   **Calculate Length:** The current valid window length is `window_end - window_start + 1`. Update `max_length` if this is greater.

### 2. Find All Anagrams in a String

*   **Problem Description:** Given a string `s` and a non-empty string `p`, find all the start indices of `p`'s anagrams in `s`. An anagram is a rearrangement of letters. For `s = "cbaebabacd"`, `p = "abc"`, the output is `[0, 6]`.

*   **Key Signals & Analysis:** We are looking for a substring in `s` that is a permutation of `p`. This means we are looking for a substring in `s` of a **fixed length** (the length of `p`) that has the exact same character counts as `p`. "Fixed-length contiguous block" is a perfect signal for the **Fixed-Size Sliding Window** pattern.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Fixed-Size Sliding Window.
    2.  **Tool:** Use two hash maps (or integer arrays of size 26 for lowercase English letters), one for the frequency of characters in `p` (`p_map`) and one for the frequency in the current window (`window_map`).
    3.  **Approach:**
        *   Pre-calculate the `p_map`.
        *   Initialize a window of size `len(p)` in `s` and populate `window_map`.
        *   Compare `p_map` and `window_map`. If they are identical, store the window's start index.
        *   Slide the window one position at a time:
            *   **Add:** Add the new character entering the window to `window_map`.
            *   **Subtract:** Subtract the character leaving the window from `window_map`.
            *   **Compare:** After each slide, compare the updated `window_map` with `p_map`. If they match, store the new start index.

### 3. Number of Islands

*   **Problem Description:** Given a 2D grid map of `'1'`s (land) and `'0'`s (water), count the number of islands. An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically.

*   **Key Signals & Analysis:** The problem involves finding **connected components** in a grid. A grid can be thought of as a graph where each cell is a node and adjacent cells with '1' have an edge between them. "Connected components" is a classic problem for graph traversal. Either **BFS** or **DFS** is the correct tool.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Graph Traversal (BFS or DFS).
    2.  **Tool:** A `visited` 2D array of the same dimensions to keep track of cells we have already processed.
    3.  **Approach:**
        *   Initialize an `island_count` to 0.
        *   Iterate through every cell (`row`, `col`) of the grid.
        *   If the cell is `'1'` and has not been visited:
            *   This is the start of a new island. Increment `island_count`.
            *   Begin a traversal (BFS or DFS) from this cell.
            *   The traversal function should explore all connected `'1'`s (up, down, left, right) and mark them as visited. This "sinks" the entire island so we don't count its parts again.
        *   Continue iterating until all cells have been checked. The final `island_count` is the answer.

### 4. Meeting Rooms II

*   **Problem Description:** Given an array of meeting time intervals `[[start, end]]`, find the minimum number of conference rooms required to hold all meetings. For `[[0, 30], [5, 10], [15, 20]]`, the answer is 2.

*   **Key Signals & Analysis:** This is an interval problem. We need to find the maximum number of overlapping intervals at any single point in time. A brute-force approach would be to check overlaps for all pairs, but that's inefficient. The key is to process events in chronological order. The two types of events are "meeting starts" and "meeting ends".

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** This is a combination of **Merge Intervals** (sorting is key) and the **Top 'K' Elements** logic (using a heap).
    2.  **Tool:** A **Min-Heap**. The heap will store the end times of all meetings currently in progress.
    3.  **Approach:**
        *   Sort the intervals based on their **start times**.
        *   Initialize a Min-Heap and a `max_rooms` counter.
        *   Iterate through the sorted meetings:
            *   Look at the meeting at the top of the heap (the one that ends the soonest).
            *   If the current meeting's `start` time is after or at the same time as the `end` time of the meeting at the top of the heap, it means a room has freed up. Pop from the heap.
            *   Push the current meeting's `end` time onto the heap. This represents allocating a room for this new meeting.
            *   The current size of the heap represents the number of rooms currently in use. Update `max_rooms = max(max_rooms, heap.size())`.
        *   The final `max_rooms` is the answer.

### 5. Generate Parentheses

*   **Problem Description:** Given `n` pairs of parentheses, write a function to generate all combinations of well-formed parentheses. For `n = 3`, the output is `["((()))", "(()())", "(())()", "()(())", "()()()"]`.

*   **Key Signals & Analysis:** The problem asks for **all possible combinations** that satisfy certain rules (well-formed parentheses). This is the definitive signal for **Backtracking**. We need to build a solution step-by-step, making choices, and backtracking when a choice leads to an invalid state.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive function that builds the parenthesis string.
    3.  **Approach:**
        *   Define a recursive function `generate(current_string, open_count, close_count)`.
        *   **Base Case:** If the `current_string` length is `2*n`, it's a complete combination. Add it to the results and return.
        *   **Constraint 1 (Add Open):** If `open_count < n`, we are allowed to add an opening parenthesis. Make the choice: call `generate(current_string + '(', open_count + 1, close_count)`.
        *   **Constraint 2 (Add Close):** If `close_count < open_count`, we are allowed to add a closing parenthesis (to ensure it's well-formed). Make the choice: call `generate(current_string + ')', open_count, close_count + 1)`.
        *   The recursive calls handle the "explore" part. The "undo" is implicit because we pass new strings; we don't modify a shared one.

### 6. Coin Change

*   **Problem Description:** Given an array of coin denominations and a total amount, find the fewest number of coins that you need to make up that amount. If it's not possible, return -1. For `coins = [1, 2, 5]`, `amount = 11`, the answer is 3 (`5+5+1`).

*   **Key Signals & Analysis:** This is a classic optimization problem: find the "fewest" or "minimum" way to do something. A greedy approach (always taking the largest coin) fails here (e.g., for `amount = 6` and `coins = [1, 3, 4]`, greedy picks `4+1+1=3` coins, but the optimal is `3+3=2` coins). The problem can be broken down into smaller, overlapping subproblems: `minCoins(amount) = 1 + min(minCoins(amount-coin))` for all available coins. This structure is the hallmark of **Dynamic Programming**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming (Bottom-Up).
    2.  **Tool:** A DP array, `dp`, of size `amount + 1`, where `dp[i]` stores the minimum coins needed to make amount `i`.
    3.  **Approach:**
        *   Initialize `dp` array with a large value (like `amount + 1`) to represent infinity, and set `dp[0] = 0`.
        *   Iterate from `i = 1` to `amount`. For each amount `i`:
            *   Iterate through each `coin` in the `coins` array.
            *   If `i - coin >= 0`, it means we can use this coin.
            *   The number of coins to make `i` would be `1 + dp[i - coin]`.
            *   Update `dp[i] = min(dp[i], 1 + dp[i - coin])`.
        *   The final answer is `dp[amount]`. If `dp[amount]` is still the large value we initialized with, it means the amount is unreachable.

---

### 7. Product of Array Except Self

*   **Problem Description:** Given an integer array `nums`, return an array `answer` such that `answer[i]` is equal to the product of all the elements of `nums` except `nums[i]`. You must solve this without using the division operator and in O(N) time. For `nums = [1, 2, 3, 4]`, the output is `[24, 12, 8, 6]`.

*   **Key Signals & Analysis:** The O(N) time constraint without division is the major signal. A naive O(N²) approach would be a nested loop. The no-division rule prevents the simple solution of "calculate total product and then divide by `nums[i]`". This forces a more creative approach. The product for index `i` is essentially `(product of all numbers to the left of i) * (product of all numbers to the right of i)`. This hints at a multi-pass approach.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Prefix/Postfix Calculation (a variation of Prefix Sum, but with products).
    2.  **Tool:** A `results` array to store the final answers.
    3.  **Approach:**
        *   **Pass 1 (Left to Right):** Create a `results` array. Initialize `results[0]` to 1. Iterate from `i = 1` to `n-1`, calculating a running product of elements to the left. Set `results[i] = nums[i-1] * results[i-1]`. After this pass, `results[i]` will hold the product of all elements to its left.
        *   **Pass 2 (Right to Left):** Introduce a variable `right_product`, initialized to 1. Iterate from `i = n-1` down to `0`. For each `i`:
            *   Multiply the current `results[i]` (which holds the left product) by `right_product`. This `results[i] = results[i] * right_product` is the final answer for this index.
            *   Update `right_product` by multiplying it with the current element: `right_product = right_product * nums[i]`. This prepares it for the next element to the left.
        *   This two-pass approach calculates the left and right products for every position and combines them in O(N) time and O(1) extra space (the output array doesn't count as extra space).

### 8. Kth Smallest Element in a Sorted Matrix

*   **Problem Description:** Given an `n x n` matrix where each of the rows and columns is sorted in ascending order, find the `k`-th smallest element in the matrix. Note that it is the `k`-th smallest element in the sorted order, not the `k`-th distinct element.

*   **Key Signals & Analysis:** A naive solution would be to flatten the N\*N matrix into an array and sort it, which would take O(N² log(N²)) time. This is too slow. The "sorted rows and columns" property is the key. The smallest element is at `(0, 0)`. The next smallest must be one of its neighbors, `(0, 1)` or `(1, 0)`. This suggests we always need to find the "next smallest" element among a set of candidates. This is a classic use case for a heap.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Top 'K' Elements (using a heap).
    2.  **Tool:** A **Min-Heap**. The heap will store candidates for the next smallest element.
    3.  **Approach:**
        *   Initialize a Min-Heap.
        *   Push the first element of each row into the Min-Heap. Store them as tuples `(value, row_index, col_index)`.
        *   We need to find the k-th smallest element, so we will perform `k` extractions from the heap.
        *   Loop `k` times:
            *   Extract the minimum element `(value, row, col)` from the heap. This is the smallest element we haven't processed yet.
            *   If this is the k-th extraction, this `value` is our answer.
            *   If the extracted element has a next element in its own row (`col + 1 < n`), push that next element `(matrix[row][col+1], row, col+1)` into the heap. This adds a new candidate for the next smallest element.
        *   This approach ensures we always have the smallest unseen elements in the heap, and its time complexity is O(K log N) because we do K pushes and pops on a heap that holds at most N elements.

### 9. Word Break

*   **Problem Description:** Given a string `s` and a dictionary of strings `wordDict`, return `true` if `s` can be segmented into a space-separated sequence of one or more dictionary words. For `s = "leetcode"`, `wordDict = ["leet", "code"]`, the output is `true`.

*   **Key Signals & Analysis:** This problem asks if a solution exists based on breaking the problem down. Can we solve for `"leetcode"`? That depends on if we can solve for `"leet"` and `"code"`, or `"l"` and `"eetcode"`, etc. We are breaking the problem into smaller, overlapping subproblems. For example, solving for `"applepenapple"` involves solving for `"apple"` twice. This is a very strong signal for **Dynamic Programming**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A boolean DP array, `dp`, of size `s.length() + 1`. `dp[i]` will be `true` if the substring `s[0...i-1]` can be segmented.
    3.  **Approach:**
        *   Initialize `dp[0] = true` (an empty string can always be segmented).
        *   Iterate through the string from `i = 1` to `s.length()`. This `i` represents the length of the prefix we are trying to segment.
        *   For each `i`, iterate from `j = 0` to `i-1`. This `j` is a potential split point.
        *   We check two conditions:
            *   `dp[j]` is `true` (meaning the prefix `s[0...j-1]` is already known to be breakable).
            *   The substring `s.substring(j, i)` exists in the `wordDict`.
        *   If both are true, it means we can successfully segment the string up to index `i`. We set `dp[i] = true` and can `break` the inner loop to move to the next `i`.
        *   The final answer is `dp[s.length()]`.

### 10. Find Minimum in Rotated Sorted Array

*   **Problem Description:** Suppose an array of unique elements sorted in ascending order is rotated at some pivot unknown to you beforehand. Find the minimum element. For `[3, 4, 5, 1, 2]`, the minimum is `1`. You must solve this in O(log N) time.

*   **Key Signals & Analysis:** The O(log N) time complexity on a sorted (or partially sorted) array is an unmistakable, flashing neon sign for **Binary Search**. The standard binary search won't work directly, so we need to modify its logic based on the properties of the rotated array.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Modified Binary Search.
    2.  **Tool:** Two pointers, `left` and `right`, to manage the search space.
    3.  **Approach:**
        *   Initialize `left = 0` and `right = n - 1`.
        *   The core insight is that in a rotated sorted array, one half will always be properly sorted. We can use the middle element to determine which half is sorted.
        *   While `left < right`:
            *   Calculate `mid = left + (right - left) / 2`.
            *   **Check the right half:** If `nums[mid] < nums[right]`, it means the right half (from `mid` to `right`) is perfectly sorted. The inflection point (and thus the minimum element) must be in the left half, *or it could be `nums[mid]` itself*. So, we shrink our search space to the left half: `right = mid`.
            *   **Check the left half:** If `nums[mid] > nums[right]`, it means the inflection point lies in the right half (the part from `mid+1` to `right`). So, we shrink our search space to the right half: `left = mid + 1`.
        *   The loop terminates when `left == right`, and that pointer will be at the minimum element in the array. Return `nums[left]`.

---

### 11. Course Schedule II

*   **Problem Description:** This is a follow-up to "Course Schedule". You are given `numCourses` and a list of `prerequisites`. Instead of returning `true/false`, you must now return one valid order in which you can take the courses. If it's impossible (due to a cycle), return an empty array.

*   **Key Signals & Analysis:** The problem is a direct extension of cycle detection in a directed graph. It explicitly asks for the **linear ordering** of vertices based on dependencies. This is the exact definition and primary use case for the **Topological Sort** pattern. We don't just need to detect a cycle; we need to capture the valid order produced by the algorithm.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Topological Sort (Kahn's Algorithm).
    2.  **Tool:** An in-degree array, an adjacency list graph representation, a queue for source nodes, and a list to store the final topological order.
    3.  **Approach:**
        *   Build the graph (adjacency list) and calculate the in-degrees of all courses from the prerequisites list.
        *   Initialize a queue and add all courses with an in-degree of 0 (these are the "source" courses with no prerequisites).
        *   Initialize an empty list `sorted_order`.
        *   While the queue is not empty:
            *   Dequeue a course `u`. Add `u` to the `sorted_order` list.
            *   For each neighbor `v` of `u` (i.e., for each course that depends on `u`), decrement its in-degree.
            *   If a neighbor `v`'s in-degree becomes 0, it is now a new source, so enqueue `v`.
        *   **Final Check:** After the loop, if the `sorted_order` list contains `numCourses` elements, it is a valid topological sort. Return it. Otherwise, the graph has a cycle, and it's impossible to finish the courses, so return an empty array.

### 12. Lowest Common Ancestor of a Binary Tree

*   **Problem Description:** Given the root of a binary tree and two nodes `p` and `q` that are in the tree, find the lowest common ancestor (LCA). The LCA is defined as the lowest node in the tree that has both `p` and `q` as descendants.

*   **Key Signals & Analysis:** The problem is inherently about exploring paths in a tree. The LCA is the point where the paths from the root to `p` and from the root to `q` diverge. This structure strongly suggests a recursive, **Depth-First Search** approach. We can use the return value of the recursive calls to pass information up the tree.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Depth-First Search (Recursive).
    2.  **Tool:** A recursive function that traverses the tree.
    3.  **Approach:**
        *   Define a recursive function `findLCA(node, p, q)`.
        *   **Base Cases:**
            *   If `node` is `NULL`, return `NULL`.
            *   If `node` is `p` or `node` is `q`, it means we've found one of the target nodes. Return `node`.
        *   **Recursive Step:**
            *   Recursively call `left_lca = findLCA(node.left, p, q)`.
            *   Recursively call `right_lca = findLCA(node.right, p, q)`.
        *   **Combine Results:**
            *   If `left_lca` and `right_lca` are both non-NULL, it means `p` was found in one subtree and `q` was found in the other. Therefore, the current `node` is the split point and is the LCA. Return `node`.
            *   If only `left_lca` is non-NULL, it means both `p` and `q` are in the left subtree. Return `left_lca`.
            *   If only `right_lca` is non-NULL, it means both `p` and `q` are in the right subtree. Return `right_lca`.
            *   If both are `NULL`, neither `p` nor `q` were found in this subtree. Return `NULL`.

### 13. Merge K Sorted Lists

*   **Problem Description:** You are given an array of `k` linked lists, where each linked list is sorted in ascending order. Merge all the linked lists into one sorted linked list and return it.

*   **Key Signals & Analysis:** We need to repeatedly find the smallest node among the current heads of `k` different lists. This task of efficiently finding the minimum element from a collection of `k` items is a perfect job for a heap. A naive approach would be to collect all nodes and sort them, but that's inefficient (O(N log N) where N is the total number of nodes).

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** K-way Merge (an application of the **Top 'K' Elements** pattern).
    2.  **Tool:** A **Min-Heap**. The heap will store the head nodes of the `k` lists.
    3.  **Approach:**
        *   Initialize a Min-Heap. The heap should be able to compare list nodes based on their values.
        *   Add the head node of each of the `k` linked lists to the Min-Heap (if the list is not empty).
        *   Create a dummy `head` node and a `tail` pointer to build the new sorted list.
        *   While the Min-Heap is not empty:
            *   Extract the node with the smallest value from the heap. Let this be `smallest_node`.
            *   Append `smallest_node` to the new list by setting `tail.next = smallest_node` and advancing `tail`.
            *   If `smallest_node` has a next node in its original list (`smallest_node.next != NULL`), add that next node to the heap. This introduces the next candidate from that list.
        *   Return the `dummy_head.next`, which is the start of the fully merged list. This has a time complexity of O(N log K), where N is the total number of nodes.

### 14. Subsets II

*   **Problem Description:** Given an integer array `nums` that may contain duplicates, return all possible subsets (the power set). The solution set must not contain duplicate subsets.

*   **Key Signals & Analysis:** This is another "generate all combinations" problem, so **Backtracking** is the clear choice. The twist is handling duplicates. If we use the standard backtracking approach, an input like `[1, 2, 2]` will generate the subset `[1, 2]` twice. We need a mechanism to ensure that we only consider each unique number once at each level of our decision tree.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive function, but with an added check to handle duplicates.
    3.  **Approach:**
        *   **First, sort the input array `nums`**. This is critical, as it places all duplicate numbers next to each other.
        *   Define a recursive function `findSubsets(current_subset, start_index)`.
        *   Add the `current_subset` to the results list.
        *   Iterate from `i = start_index` to the end of `nums`.
        *   **The Duplicate Handling Logic:** Inside the loop, add a condition: `if (i > start_index && nums[i] == nums[i-1]) continue;`. This is the key. It says: "If I am not the first element in this level of recursion (`i > start_index`), and I am the same as the element before me, then the previous recursive call has already generated all subsets starting with this number. Skip me to avoid duplicates."
        *   **Make a choice:** Add `nums[i]` to `current_subset`.
        *   **Recurse:** Call `findSubsets(current_subset, i + 1)`.
        *   **Undo the choice:** Remove `nums[i]` from `current_subset` to backtrack.

### 15. Reorder List

*   **Problem Description:** You are given the head of a singly linked list. The list can be reordered in-place as follows: `L0 → Ln → L1 → Ln-1 → L2 → Ln-2 → …`. For `[1,2,3,4,5]`, the reordered list is `[1,5,2,4,3]`.

*   **Key Signals & Analysis:** The problem requires taking the second half of the list, reversing it, and then weaving it into the first half. This is not one single pattern but a sequence of linked list manipulations. It combines three distinct sub-problems.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Combination of **Fast & Slow Pointers**, **In-place Reversal of a Linked List**, and a two-pointer merge.
    2.  **Tool:** Pointer manipulation.
    3.  **Approach:**
        *   **Step 1: Find the Middle of the List.** Use the **Fast & Slow Pointers** pattern. When the `fast` pointer reaches the end, the `slow` pointer will be at (or just before) the midpoint. This splits the list into two halves.
        *   **Step 2: Reverse the Second Half.** Take the second half of the list (starting from `slow.next`) and reverse it in-place. This is a standard linked list reversal algorithm.
        *   **Step 3: Merge the Two Halves.** Now you have two lists: the first half (`head` to `slow`) and the reversed second half. Use two pointers, one for each list head, and "weave" them together by alternating `next` pointers until the second list is exhausted.        

---

### 16. Longest Palindromic Substring

*   **Problem Description:** Given a string `s`, return the longest palindromic substring in `s`. For `s = "babad"`, the answer is `"bab"` (or `"aba"`). For `s = "cbbd"`, the answer is `"bb"`.

*   **Key Signals & Analysis:** A naive approach of generating all O(N²) substrings and checking if each is a palindrome O(N) would be O(N³). This is too slow. The problem has optimal substructure: if `s[i+1...j-1]` is a palindrome and `s[i] == s[j]`, then `s[i...j]` is also a palindrome. This hints at **Dynamic Programming**. An alternative, often simpler and faster in practice, is to recognize that a palindrome is symmetric around its center. There are 2N-1 possible centers (N characters and N-1 spaces between characters).

*   **Strategic Pattern & Approach (Expand From Center):**
    1.  **Pattern:** Expand From Center (a specialized string pattern).
    2.  **Tool:** A helper function and variables to track the longest palindrome's start and length.
    3.  **Approach:**
        *   Iterate through the string with an index `i` from `0` to `n-1`.
        *   For each `i`, treat it as the center of a potential palindrome and "expand" outwards. There are two cases to check:
            *   **Odd length palindrome:** The center is the character `s[i]` itself. Call `expand(i, i)`.
            *   **Even length palindrome:** The center is the space between `s[i]` and `s[i+1]`. Call `expand(i, i + 1)`.
        *   The `expand(left, right)` helper function works as follows:
            *   While `left >= 0`, `right < n`, and `s[left] == s[right]`, decrement `left` and increment `right`.
            *   The palindrome found is `s[left+1...right-1]`. Return its length.
        *   Keep track of the maximum length found and the starting index to determine the final answer. This approach is O(N²).

### 17. 01 Knapsack Problem

*   **Problem Description:** You are given `n` items, where the `i`-th item has a weight `w[i]` and a value `v[i]`. You are also given a knapsack with a maximum weight capacity `W`. You cannot break an item. Choose a subset of items to put into the knapsack such that the total value is maximized, and the total weight does not exceed `W`.

*   **Key Signals & Analysis:** This is the canonical **Dynamic Programming** problem. You have to make a discrete choice for each item (either take it or leave it) to achieve an optimal result (maximum value). The state of the problem can be defined by two parameters: which items you are considering and the remaining capacity of the knapsack. This naturally leads to a 2D DP solution.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming (0/1 Knapsack).
    2.  **Tool:** A 2D DP table, `dp[i][j]`, where `dp[i][j]` stores the maximum value achievable using the first `i` items with a knapsack capacity of `j`.
    3.  **Approach:**
        *   Create a `dp` table of size `(n+1) x (W+1)`.
        *   Iterate from `i = 1` to `n` (for each item).
        *   Iterate from `j = 1` to `W` (for each capacity).
        *   For each cell `dp[i][j]`, we make a choice regarding item `i`:
            *   **Case 1: Don't take item `i`.** The maximum value is the same as the maximum value achievable with the first `i-1` items at the same capacity. So, `dp[i][j] = dp[i-1][j]`.
            *   **Case 2: Take item `i`.** This is only possible if `j >= w[i-1]`. The value would be `v[i-1]` plus the maximum value achievable with the first `i-1` items and the remaining capacity `j - w[i-1]`. So, the value is `v[i-1] + dp[i-1][j - w[i-1]]`.
        *   `dp[i][j]` is the maximum of these two cases.
        *   The final answer is in `dp[n][W]`.

### 18. Clone Graph

*   **Problem Description:** Given a reference to a node in a connected undirected graph, return a deep copy (clone) of the graph. Each node in the graph contains a value and a list of its neighbors.

*   **Key Signals & Analysis:** The problem requires traversing the entire graph and creating a new copy of every node and every edge. A simple traversal is not enough; we need to handle cycles correctly. If we just traverse recursively, we might get stuck in an infinite loop. We need a way to keep track of the nodes that have already been cloned.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Graph Traversal (either DFS or BFS).
    2.  **Tool:** A **Hash Map** to store the mapping from original nodes to their corresponding cloned nodes. This map serves as both a `visited` set and a way to retrieve already created clones.
    3.  **Approach (using DFS):**
        *   Create a hash map `cloned_nodes`.
        *   Define a recursive function `clone(node)`.
        *   **Base Case:** If `node` is `NULL`, return `NULL`.
        *   **Check Visited:** If `node` is already in the `cloned_nodes` map, it means we've already created a copy for it. Return the copy from the map to avoid cycles.
        *   **Create Clone:** If not visited, create a new node `copy` with the same value as the original `node`.
        *   **Store in Map:** Add the mapping to the hash map: `cloned_nodes[node] = copy`. This is crucial to do *before* recurring.
        *   **Recurse on Neighbors:** Iterate through the `neighbors` of the original `node`. For each `neighbor`, recursively call `clone(neighbor)`. Add the result of this call to the `copy`'s neighbor list.
        *   Return the `copy`.

### 19. Validate Binary Search Tree

*   **Problem Description:** Given the root of a binary tree, determine if it is a valid binary search tree (BST). A valid BST is defined as follows: the left subtree of a node contains only nodes with values less than the node's value; the right subtree contains only nodes with values greater than the node's value; both the left and right subtrees must also be binary search trees.

*   **Key Signals & Analysis:** The definition of a BST is inherently recursive. However, a simple check like `node.left.val < node.val` is not sufficient. The constraint must hold for the *entire* subtree. For example, a right child of a node must be greater than its parent, but it must also be less than its grandparent if it's in the grandparent's left subtree. This means we need to pass down range constraints (`min_val`, `max_val`) to each node.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Depth-First Search (Recursive).
    2.  **Tool:** A recursive helper function that carries the valid range for each node.
    3.  **Approach:**
        *   Define a recursive function `isValid(node, min_bound, max_bound)`.
        *   **Base Case:** If `node` is `NULL`, it's a valid BST. Return `true`.
        *   **Check Constraints:** If `node.val <= min_bound` or `node.val >= max_bound`, the BST property is violated. Return `false`.
        *   **Recurse:** Recursively check the left and right subtrees, tightening the bounds:
            *   For the left child, the new `max_bound` is the current node's value: `isValid(node.left, min_bound, node.val)`.
            *   For the right child, the new `min_bound` is the current node's value: `isValid(node.right, node.val, max_bound)`.
        *   Return `true` only if both recursive calls return `true`.
        *   The initial call would be `isValid(root, -infinity, +infinity)`.

### 20. Implement Trie (Prefix Tree)

*   **Problem Description:** A trie is a tree-like data structure that stores a dynamic set of strings. Implement a trie with three methods: `insert(word)`, `search(word)`, and `startsWith(prefix)`.

*   **Key Signals & Analysis:** This problem explicitly asks you to implement a specific data structure. It's a test of your knowledge of non-standard data structures and your ability to work with pointers/references and object-oriented concepts.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Trie Data Structure Implementation.
    2.  **Tool:** A `TrieNode` class/struct.
    3.  **Approach:**
        *   **`TrieNode` Structure:** Each `TrieNode` should contain:
            *   An array or hash map of children, e.g., `children[26]` for lowercase English letters.
            *   A boolean flag, `isEndOfWord`, to mark if a node represents the end of a complete word.
        *   **`Trie` Structure:** The `Trie` class itself will have a single member: a `root` `TrieNode`.
        *   **`insert(word)`:**
            *   Start at the `root`.
            *   For each character `c` in the `word`:
                *   Check if a child node for `c` exists. If not, create it.
                *   Move to that child node.
            *   After the loop, mark the final node's `isEndOfWord` as `true`.
        *   **`search(word)`:**
            *   Traverse the trie as in `insert`. If at any point a character's node doesn't exist, return `false`.
            *   After the loop, return `true` only if the final node's `isEndOfWord` is also `true`.
        *   **`startsWith(prefix)`:**
            *   Traverse the trie as in `search`. If you can successfully traverse for the entire `prefix`, return `true`. You don't need to check the `isEndOfWord` flag.

---

### 21. Longest Increasing Subsequence

*   **Problem Description:** Given an integer array `nums`, return the length of the longest strictly increasing subsequence. A subsequence is a sequence that can be derived from an array by deleting some or no elements without changing the order of the remaining elements. For `nums = [10, 9, 2, 5, 3, 7, 101, 18]`, the LIS is `[2, 3, 7, 101]`, so the length is 4.

*   **Key Signals & Analysis:** This is another classic optimization problem. To find the length of the LIS ending at index `i`, we need to know the lengths of all increasing subsequences ending at indices `j < i`. This "relying on optimal solutions to subproblems" structure is a clear signal for **Dynamic Programming**. A more advanced solution achieves O(N log N), but the O(N²) DP approach is the standard starting point.

*   **Strategic Pattern & Approach (O(N²) DP):**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A DP array, `dp`, of size `n`. `dp[i]` will store the length of the longest increasing subsequence that *ends* with the element `nums[i]`.
    3.  **Approach:**
        *   Initialize the `dp` array with all 1s, since every element by itself is an increasing subsequence of length 1.
        *   Initialize a `max_length = 1`.
        *   Iterate through the `nums` array from `i = 1` to `n-1`.
        *   For each `i`, iterate from `j = 0` to `i-1`.
        *   **Check for Extension:** If `nums[i] > nums[j]`, it means we can potentially extend the subsequence ending at `j`. The new length would be `dp[j] + 1`.
        *   **Update:** We update `dp[i]` to be the maximum of its current value and the new potential length: `dp[i] = max(dp[i], dp[j] + 1)`.
        *   After the inner loop, update the overall `max_length` with `dp[i]`.
        *   The final answer is `max_length`.

### 22. Container With Most Water

*   **Problem Description:** You are given an integer array `height` of length `n`. There are `n` vertical lines drawn such that the two endpoints of the `i`-th line are `(i, 0)` and `(i, height[i])`. Find two lines that together with the x-axis form a container, such that the container contains the most water.

*   **Key Signals & Analysis:** A brute-force O(N²) solution would check every pair of lines. To optimize, we need a way to discard possibilities intelligently. Consider a wide container formed by a pointer at the far left and one at the far right. The area is `width * min(height[left], height[right])`. To find a potentially larger area, we must either increase the width or increase the minimum height. Since we start with the maximum possible width, we can only find a better solution by increasing the minimum height. This logic leads to a greedy approach.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Two Pointers.
    2.  **Tool:** A `left` pointer starting at index 0 and a `right` pointer starting at index `n-1`.
    3.  **Approach:**
        *   Initialize `max_area = 0`, `left = 0`, `right = n-1`.
        *   While `left < right`:
            *   Calculate the current area: `area = (right - left) * min(height[left], height[right])`.
            *   Update `max_area = max(max_area, area)`.
            *   **The Greedy Choice:** We need to move one of the pointers inwards. To have any chance of increasing the area, we must get rid of the shorter of the two lines, hoping to find a taller line to replace it.
            *   If `height[left] < height[right]`, move the left pointer: `left++`.
            *   Else, move the right pointer: `right--`.
        *   The loop terminates when the pointers meet. The `max_area` is the answer. This is an O(N) solution.

### 23. Task Scheduler

*   **Problem Description:** Given a characters array `tasks` representing tasks a CPU needs to do, where each letter represents a different task, and a non-negative integer `n` representing the cooldown period between two same tasks. Return the least number of units of time that the CPU will take to finish all the given tasks. For `tasks = ["A","A","A","B","B","B"]`, `n = 2`, the output is 8 (`A -> B -> idle -> A -> B -> idle -> A -> B`).

*   **Key Signals & Analysis:** This is a scheduling problem. To minimize the total time, the CPU should always try to work on the most frequent task that is currently available (not in cooldown). This "most frequent" signal points towards a greedy approach. We need a way to efficiently retrieve the most frequent task.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Greedy Algorithm with a Heap.
    2.  **Tool:** A **Max-Heap** to store the frequencies of the tasks, and a **Queue** to manage the cooldown period.
    3.  **Approach:**
        *   Count the frequency of each task and put these frequencies into a Max-Heap.
        *   Initialize `time = 0`.
        *   While the Max-Heap is not empty or the cooldown queue is not empty:
            *   Increment `time`.
            *   If the cooldown queue is not empty and the task at the front is ready to be scheduled again (`queue.front().time <= time`), pop it and add its frequency back to the Max-Heap.
            *   If the Max-Heap is not empty:
                *   Extract the max frequency `count` from the heap.
                *   If `count - 1 > 0`, this task needs to be scheduled again. Push it onto the cooldown queue with its remaining count and its next available time `time + n`.
        *   The final `time` is the answer.

### 24. Edit Distance

*   **Problem Description:** Given two strings `word1` and `word2`, return the minimum number of operations (insert, delete, or replace a character) required to convert `word1` to `word2`.

*   **Key Signals & Analysis:** This is a classic optimization problem that can be solved by breaking it down into subproblems. To find the edit distance between `word1` and `word2`, we can consider the subproblem for their prefixes. The solution for `editDistance("horse", "ros")` depends on the solutions for smaller versions like `editDistance("hors", "ro")`. This overlapping subproblem structure is a perfect fit for **Dynamic Programming**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A 2D DP table, `dp[i][j]`, storing the edit distance between the first `i` characters of `word1` and the first `j` characters of `word2`.
    3.  **Approach:**
        *   Create a `dp` table of size `(len1 + 1) x (len2 + 1)`.
        *   **Initialize:** The first row `dp[0][j]` is `j` (j insertions). The first column `dp[i][0]` is `i` (i deletions).
        *   Iterate from `i = 1` to `len1` and `j = 1` to `len2`.
        *   **Case 1: `word1[i-1] == word2[j-1]`**. The last characters match, so no operation is needed for them. The cost is the same as the subproblem without these characters: `dp[i][j] = dp[i-1][j-1]`.
        *   **Case 2: `word1[i-1] != word2[j-1]`**. The last characters differ. We have three choices, and we take the minimum:
            *   **Insert:** `1 + dp[i][j-1]` (insert a character into word1 to match word2's last char).
            *   **Delete:** `1 + dp[i-1][j]` (delete word1's last char).
            *   **Replace:** `1 + dp[i-1][j-1]` (replace word1's last char with word2's).
        *   `dp[i][j]` is the minimum of these three values.
        *   The final answer is `dp[len1][len2]`.

### 25. Trapping Rain Water

*   **Problem Description:** Given `n` non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it can trap after raining.

*   **Key Signals & Analysis:** A naive approach for each bar would be to find the tallest bar to its left and the tallest bar to its right, which is O(N²). To optimize, we can pre-calculate these values. The amount of water trapped above any bar `i` is determined by `min(max_left_height, max_right_height) - height[i]`. This suggests a multi-pass approach or a more clever single-pass method.

*   **Strategic Pattern & Approach (Two Pointers):**
    1.  **Pattern:** Two Pointers.
    2.  **Tool:** `left` and `right` pointers, and `max_left` and `max_right` variables.
    3.  **Approach:**
        *   Initialize `left=0`, `right=n-1`, `max_left=0`, `max_right=0`, `total_water=0`.
        *   While `left < right`:
            *   The core insight is that the water level at the shorter of the two ends is what matters. If `height[left] < height[right]`:
                *   The bottleneck is the left side. Update `max_left = max(max_left, height[left])`.
                *   The water trapped at this `left` position is `max_left - height[left]`. Add this to `total_water`.
                *   Move the pointer: `left++`.
            *   Else (if `height[right] <= height[left]`):
                *   The bottleneck is the right side. Update `max_right = max(max_right, height[right])`.
                *   The water trapped at this `right` position is `max_right - height[right]`. Add this to `total_water`.
                *   Move the pointer: `right--`.
        *   The `total_water` is the final answer. This is an elegant O(N) time, O(1) space solution.

---

### 26. Serialize and Deserialize Binary Tree

*   **Problem Description:** Design an algorithm to serialize a binary tree to a string and deserialize it back to the tree. Serialization is the process of converting a data structure into a format that can be stored or transmitted.

*   **Key Signals & Analysis:** The core challenge is encoding the tree's structure, including the `NULL` children, into a linear format. A simple in-order traversal is insufficient because it can produce the same output for different tree structures. **Pre-order traversal (a form of DFS)** is a natural choice because the root is always processed first, which simplifies reconstruction. We must invent a special marker (like "N" or "#") to represent `NULL` nodes.

*   **Strategic Pattern & Approach (DFS/Pre-order):**
    1.  **Pattern:** Tree Traversal (specifically Pre-order DFS).
    2.  **Tool:** A recursive helper function for serialization and a queue or an iterator for deserialization.
    3.  **Approach:**
        *   **`serialize(root)`:**
            *   Define a recursive helper `build_string(node)`.
            *   **Base Case:** If `node` is `NULL`, append a special marker (e.g., "#") and a delimiter (e.g., ",") to the string builder.
            *   **Pre-order Logic:** Append `node.val` and a delimiter. Then, recursively call `build_string(node.left)` and `build_string(node.right)`.
        *   **`deserialize(data)`:**
            *   Split the input string by the delimiter to get a list or queue of values.
            *   Define a recursive helper `build_tree()`.
            *   **Base Case:** Dequeue the next value. If it's the `NULL` marker, return `NULL`.
            *   **Pre-order Logic:** If it's a number, create a new `TreeNode` with that value.
            *   Set the node's left child by recursively calling `node.left = build_tree()`.
            *   Set the node's right child by recursively calling `node.right = build_tree()`.
            *   Return the created node.

### 27. Find Median from Data Stream

*   **Problem Description:** Design a data structure that supports adding numbers from a data stream and finding the median of all elements seen so far.

*   **Key Signals & Analysis:** This was covered in detail previously, but it's a crucial competitive problem. The need to efficiently find the "middle" element of a constantly growing dataset rules out sorting-based approaches (O(N log N) or O(N) per operation). We need a structure that maintains a "halfway" point. This immediately signals the **Two Heaps Pattern**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Two Heaps.
    2.  **Tool:** A **Max-Heap** (to store the smaller half of numbers) and a **Min-Heap** (to store the larger half).
    3.  **Approach:**
        *   Maintain two heaps: `small_half` (Max-Heap) and `large_half` (Min-Heap).
        *   **Balance Rule:** Keep the heaps' sizes either equal or with `small_half` having one more element.
        *   **`addNum(num)`:**
            *   Add `num` to the `small_half` (Max-Heap).
            *   To maintain the property that everything in `small_half` <= everything in `large_half`, move the largest element from `small_half` to `large_half`.
            *   To maintain the balance rule, if `large_half` is now bigger than `small_half`, move the smallest element from `large_half` back to `small_half`.
        *   **`findMedian()`:**
            *   If the heap sizes are different, the median is the top of the `small_half` (Max-Heap).
            *   If the heap sizes are equal, the median is the average of the tops of both heaps.

### 28. Word Ladder

*   **Problem Description:** Given two words, `beginWord` and `endWord`, and a dictionary `wordList`, return the length of the shortest transformation sequence from `beginWord` to `endWord`, such that only one letter can be changed at a time, and each transformed word must exist in the `wordList`.

*   **Key Signals & Analysis:** The word **"shortest"** is the most important signal in the problem description. When you see "shortest path" in a graph-like problem where all edges have the same weight (here, each transformation has a "cost" of 1), the go-to algorithm is **Breadth-First Search (BFS)**. The words are the nodes, and an edge exists between two words if they are one letter apart.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Breadth-First Search (BFS).
    2.  **Tool:** A queue to store `(word, level)` pairs and a `visited` set to avoid cycles and redundant work.
    3.  **Approach:**
        *   Pre-process the `wordList` into a set for fast O(1) lookups.
        *   Initialize a queue and add `(beginWord, 1)`.
        *   Initialize a `visited` set and add `beginWord`.
        *   While the queue is not empty:
            *   Dequeue `(current_word, level)`.
            *   If `current_word` is the `endWord`, return `level`.
            *   **Find Neighbors:** Generate all possible "next words" by changing one character of `current_word` at a time (from 'a' to 'z').
            *   For each `next_word`:
                *   If `next_word` is in the `wordList` and has not been `visited`:
                    *   Add it to the `visited` set.
                    *   Enqueue `(next_word, level + 1)`.
        *   If the queue becomes empty and the `endWord` was not found, no such path exists. Return 0.

### 29. Maximum Subarray

*   **Problem Description:** Given an integer array `nums`, find the contiguous subarray (containing at least one number) which has the largest sum and return its sum.

*   **Key Signals & Analysis:** This is a famous problem that can be solved with a clever greedy insight. As you iterate through the array, you keep a `current_sum`. If this `current_sum` ever becomes negative, it's impossible for it to contribute positively to any future subarray. A negative prefix will always reduce the sum of a subsequent sequence. Therefore, if the `current_sum` becomes negative, you should discard it and start a new subarray sum from the next element.

*   **Strategic Pattern & Approach (Kadane's Algorithm):**
    1.  **Pattern:** Dynamic Programming / Greedy (Kadane's Algorithm).
    2.  **Tool:** Two variables: one to track the maximum sum so far (`max_so_far`) and one for the current subarray sum (`current_max`).
    3.  **Approach:**
        *   Initialize `max_so_far` and `current_max` to the first element of the array.
        *   Iterate from the second element (`i = 1` to `n-1`):
            *   For each element, make a choice: either extend the current subarray or start a new one.
            *   `current_max = max(nums[i], current_max + nums[i])`.
            *   Update the overall maximum: `max_so_far = max(max_so_far, current_max)`.
        *   Return `max_so_far`. This is an elegant O(N) time, O(1) space solution.

### 30. Accounts Merge

*   **Problem Description:** Given a list of `accounts` where each account is `[name, email1, email2, ...]`, merge all accounts that belong to the same person. Two accounts belong to the same person if they share at least one common email. The output should be a list of merged accounts, sorted by email.

*   **Key Signals & Analysis:** This is fundamentally a problem about finding **connected components**. The elements are the emails, and the accounts provide the links between them. If `emailA` and `emailB` are in the same account, they are connected. If `emailB` and `emailC` are in another account, then `emailA`, `emailB`, and `emailC` are all transitively connected and part of the same component. This is a perfect scenario for the **Union-Find (Disjoint Set Union)** data structure, which is designed for efficiently merging sets and tracking component membership.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Union-Find (DSU).
    2.  **Tool:** A Union-Find data structure, a hash map to map emails to an integer index, and another map to group emails by their component root.
    3.  **Approach:**
        *   Create two maps: `email_to_name` and `email_to_id`. Iterate through all emails to populate these maps and assign a unique integer ID to each email.
        *   Initialize a DSU structure with the total number of unique emails.
        *   Iterate through the `accounts` again. For each account, take the first email and call `union` on it with every other email in that same account. This links all emails within an account into a single component.
        *   After processing all accounts, create a result map: `merged_accounts`, where the key is the integer `root` of a component.
        *   Iterate through all unique emails. For each email, find its component `root` using the `find` operation. Add the email to the list associated with that `root` in the `merged_accounts` map.
        *   Finally, format the `merged_accounts` map into the required output format, adding the correct name and sorting the emails.

---

### 31. Combination Sum

*   **Problem Description:** Given an array of **distinct** integers `candidates` and a target integer `target`, return a list of all unique combinations of `candidates` where the chosen numbers sum to `target`. You may return the combinations in any order. The **same number may be chosen from `candidates` an unlimited number of times**.

*   **Key Signals & Analysis:** The problem asks for **all possible combinations**, which is a primary signal for **Backtracking**. The key variations here are that the input numbers are distinct, but we can reuse the same number multiple times. This changes the recursive step slightly compared to a standard subset problem.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive helper function to build the combinations.
    3.  **Approach:**
        *   Define a recursive function `findCombinations(current_combination, remaining_target, start_index)`.
        *   **Base Case (Success):** If `remaining_target` is 0, we have found a valid combination. Add a copy of `current_combination` to the results and return.
        *   **Base Case (Failure):** If `remaining_target` is less than 0, this path is invalid. Return.
        *   **Recursive Step:**
            *   Iterate through the `candidates` array from `i = start_index` to the end.
            *   **The Re-use Logic:** The crucial part is how we recurse.
                *   **Make a choice:** Add `candidates[i]` to `current_combination`.
                *   **Recurse:** Call `findCombinations(current_combination, remaining_target - candidates[i], i)`. Notice that we pass `i` as the next `start_index`, not `i+1`. This allows the same number to be chosen again in the next level of recursion.
                *   **Undo the choice:** Remove `candidates[i]` from `current_combination` to backtrack and explore other possibilities.

### 32. Permutations

*   **Problem Description:** Given an array `nums` of **distinct** integers, return all the possible permutations. You can return the answer in any order.

*   **Key Signals & Analysis:** Again, the problem asks for **all possible** arrangements, which is a classic **Backtracking** problem. The core challenge in generating permutations is ensuring that each number is used exactly once in each permutation. We need a mechanism to track which numbers have already been included in the current permutation being built.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive helper function and a way to track used elements (either a boolean `used` array or by swapping elements).
    3.  **Approach (using a `used` array):**
        *   Define a recursive function `findPermutations(current_permutation, used_array)`.
        *   **Base Case:** If the size of `current_permutation` equals the size of `nums`, we have a complete permutation. Add a copy to the results and return.
        *   **Recursive Step:**
            *   Iterate through the `nums` array from `i = 0` to `n-1`.
            *   **Check Used:** If `used[i]` is `true`, skip this number as it's already in the current permutation.
            *   **Make a choice:**
                *   Mark `used[i] = true`.
                *   Add `nums[i]` to `current_permutation`.
            *   **Recurse:** Call `findPermutations(current_permutation, used_array)`.
            *   **Undo the choice (Backtrack):**
                *   Remove `nums[i]` from `current_permutation`.
                *   Mark `used[i] = false`.

### 33. Search in Rotated Sorted Array

*   **Problem Description:** You are given a sorted integer array `nums` with distinct values, which has been rotated at an unknown pivot. Given an integer `target`, return the index of `target` if it is in `nums`, or -1 if it is not. You must write an algorithm with O(log n) runtime complexity.

*   **Key Signals & Analysis:** The O(log N) complexity on a partially sorted array is an absolute giveaway for **Modified Binary Search**. The standard binary search logic of `if (target < mid)` go left, else go right, is broken. We must first determine which half of the array (relative to the `mid` pointer) is properly sorted, and then we can make an informed decision.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Modified Binary Search.
    2.  **Tool:** `left` and `right` pointers to manage the search space.
    3.  **Approach:**
        *   Initialize `left = 0`, `right = n-1`.
        *   While `left <= right`:
            *   Calculate `mid`.
            *   If `nums[mid] == target`, return `mid`.
            *   **Determine Sorted Half:**
                *   **Case 1: The left half is sorted (`nums[left] <= nums[mid]`).**
                    *   Check if the `target` lies within this sorted left half (`target >= nums[left] && target < nums[mid]`).
                    *   If yes, search the left half: `right = mid - 1`.
                    *   If no, the target must be in the unsorted right half: `left = mid + 1`.
                *   **Case 2: The right half is sorted (`nums[left] > nums[mid]`).**
                    *   Check if the `target` lies within this sorted right half (`target > nums[mid] && target <= nums[right]`).
                    *   If yes, search the right half: `left = mid + 1`.
                    *   If no, the target must be in the unsorted left half: `right = mid - 1`.
        *   If the loop finishes, the target was not found. Return -1.

### 34. Palindrome Partitioning

*   **Problem Description:** Given a string `s`, partition `s` such that every substring of the partition is a palindrome. Return all possible palindrome partitionings of `s`.

*   **Key Signals & Analysis:** This is another "find all possible" problem, indicating **Backtracking**. We need to make a series of choices. The choices are "where do I make the next cut?". We can make a cut after the first character, check if the resulting substring `s[0...0]` is a palindrome, and then recursively try to partition the rest of the string `s[1...]`. Then we backtrack and try making the cut after the second character, and so on.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive helper function and a function to check if a substring is a palindrome.
    3.  **Approach:**
        *   Define a recursive function `findPartitions(current_partition, start_index)`.
        *   **Base Case:** If `start_index` reaches the end of the string, it means we have successfully partitioned the entire string. Add a copy of `current_partition` to the results.
        *   **Recursive Step:**
            *   Iterate from `i = start_index` to the end of the string.
            *   Consider the substring from `start_index` to `i`.
            *   **Check Palindrome:** If this substring is a palindrome:
                *   **Make a choice:** Add this substring to `current_partition`.
                *   **Recurse:** Call `findPartitions(current_partition, i + 1)`.
                *   **Undo the choice:** Remove the substring from `current_partition` to backtrack.
        *   (Optimization: You can pre-compute all possible palindrome substrings using DP to make the palindrome check O(1)).

### 35. Unique Paths

*   **Problem Description:** A robot is located at the top-left corner of a `m x n` grid. The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid. How many possible unique paths are there?

*   **Key Signals & Analysis:** This is a classic combinatorics problem that can be modeled with **Dynamic Programming**. The number of ways to reach a cell `(i, j)` is the sum of the number of ways to reach the cell above it `(i-1, j)` and the number of ways to reach the cell to its left `(i, j-1)`. This reliance on the solutions to smaller subproblems is the core of DP.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A 2D DP grid, `dp[i][j]`, storing the number of unique paths to reach cell `(i, j)`.
    3.  **Approach:**
        *   Create a `dp` grid of size `m x n`.
        *   **Initialize:** Every cell in the first row (`dp[0][j]`) can only be reached from the left, so there is only 1 path. Initialize all to 1. Similarly, every cell in the first column (`dp[i][0]`) can only be reached from above, so there is only 1 path. Initialize all to 1.
        *   Iterate through the rest of the grid, from `i = 1` to `m-1` and `j = 1` to `n-1`.
        *   For each cell `dp[i][j]`, apply the recurrence relation: `dp[i][j] = dp[i-1][j] + dp[i][j-1]`.
        *   The final answer is the value in the bottom-right corner: `dp[m-1][n-1]`.

---

### 36. Word Search

*   **Problem Description:** Given an `m x n` grid of characters `board` and a string `word`, return `true` if `word` exists in the grid. The word can be constructed from letters of sequentially adjacent cells, where "adjacent" cells are horizontally or vertically neighboring. The same letter cell may not be used more than once.

*   **Key Signals & Analysis:** This is a pathfinding problem on a grid (which is a form of a graph). We need to find if *any* path exists that matches the word. This is a classic exploration problem, perfectly suited for **Backtracking** (which is a specialized form of DFS). We start at a potential first letter and try to build the word by exploring its neighbors. If a path leads to a dead end, we must "backtrack" and try another direction.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking / Depth-First Search.
    2.  **Tool:** A recursive helper function. The grid itself can be modified to act as a `visited` set to save space.
    3.  **Approach:**
        *   Iterate through every cell `(row, col)` in the `board`. If `board[row][col]` matches the first letter of `word`, start a search from there.
        *   Define a recursive function `search(row, col, index_in_word)`.
        *   **Base Case (Success):** If `index_in_word` reaches the end of the `word`, we have successfully found the entire word. Return `true`.
        *   **Base Case (Failure):** Check for out-of-bounds conditions (`row` or `col` are invalid) or if the character at `board[row][col]` does not match `word[index_in_word]`. If any of these are true, return `false`.
        *   **Make a choice & Mark as Visited:**
            *   Temporarily "mark" the current cell as visited to avoid reusing it. A common trick is to change `board[row][col]` to a special character like `#`.
        *   **Recurse on Neighbors:**
            *   Recursively call `search` for all four neighbors (up, down, left, right) with `index_in_word + 1`. If any of these recursive calls return `true`, it means a path was found. Immediately return `true`.
        *   **Undo the choice (Backtrack):**
            *   If none of the neighbors led to a solution, this path is a dead end. Restore the original character to `board[row][col]` before returning `false`. This is the critical backtracking step.

### 37. Non-overlapping Intervals

*   **Problem Description:** Given an array of intervals `intervals` where `intervals[i] = [starti, endi]`, return the minimum number of intervals you need to remove to make the rest of the intervals non-overlapping.

*   **Key Signals & Analysis:** This is an interval-based optimization problem. The word "minimum" suggests a potential for DP or a greedy approach. A greedy strategy is often effective for interval problems. The question is, what should we be greedy about? Sorting by start times is a common first step. After sorting, if we have two intervals, which one should we keep? It's always optimal to keep the one that **ends earlier**, as it leaves more room for subsequent intervals.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Greedy Algorithm with Sorting (similar to Merge Intervals).
    2.  **Tool:** Sorting the input array.
    3.  **Approach:**
        *   Sort the `intervals` array based on their **end times**. This is the crucial greedy choice. (Sorting by start times also works, but the logic is slightly more complex).
        *   Initialize `removed_count = 0`.
        *   Initialize `last_end_time` to the end time of the first interval.
        *   Iterate through the intervals starting from the second one.
        *   For each `current_interval`:
            *   **Check for Overlap:** If the `current_interval.start` is less than `last_end_time`, it overlaps with the last interval we decided to keep. This `current_interval` must be removed. Increment `removed_count`.
            *   **No Overlap:** If the `current_interval.start` is greater than or equal to `last_end_time`, it doesn't overlap. We can keep it. Update `last_end_time` to be the end time of this `current_interval`.
        *   The final `removed_count` is the answer.

### 38. Rotting Oranges

*   **Problem Description:** You are given an `m x n` grid where each cell can have one of three values: `0` (empty), `1` (fresh orange), or `2` (rotten orange). Every minute, any fresh orange that is 4-directionally adjacent to a rotten orange becomes rotten. Return the minimum number of minutes that must elapse until no cell has a fresh orange. If this is impossible, return -1.

*   **Key Signals & Analysis:** The problem asks for the "minimum time" for a process to spread simultaneously from multiple sources (the initial rotten oranges). This process happens in layers or "minutes." This is a classic sign for **Breadth-First Search (BFS)**, as BFS explores a graph level by level, which directly corresponds to the minutes passing.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Breadth-First Search (Multi-source BFS).
    2.  **Tool:** A queue to store the locations of rotten oranges.
    3.  **Approach:**
        *   Initialize a queue and a `fresh_oranges_count`.
        *   Iterate through the grid once to:
            *   Add the coordinates of all initially rotten (`2`) oranges to the queue.
            *   Count the total number of fresh (`1`) oranges.
        *   Initialize `minutes_passed = -1`. A special value `-1` is used so that the first level of rotting corresponds to minute 0.
        *   Start the BFS loop, which proceeds in levels (minutes).
        *   While the queue is not empty:
            *   Get the `level_size` (the number of oranges that became rotten in the previous minute).
            *   Increment `minutes_passed`.
            *   Loop `level_size` times:
                *   Dequeue a rotten orange at `(row, col)`.
                *   Check its four neighbors. If a neighbor is a fresh orange (`1`):
                    *   Change it to rotten (`2`).
                    *   Decrement `fresh_oranges_count`.
                    *   Enqueue the neighbor's coordinates.
        *   **Final Check:** After the BFS is complete, if `fresh_oranges_count` is 0, it means all reachable oranges have rotted. Return `minutes_passed`. If `fresh_oranges_count` is greater than 0, it means some oranges were unreachable. Return -1. If there were no fresh oranges to begin with, return 0.

### 39. Gas Station

*   **Problem Description:** There are `n` gas stations along a circular route, where the amount of gas at station `i` is `gas[i]`. You have a car with an unlimited gas tank and it costs `cost[i]` gas to travel from station `i` to `i+1`. You begin the journey with an empty tank at one of the gas stations. Return the starting gas station's index if you can travel around the circuit once in the clockwise direction, otherwise return -1.

*   **Key Signals & Analysis:** A naive O(N²) solution would be to try starting at every station and simulating the trip. To optimize, we need a greedy insight. Let `diff[i] = gas[i] - cost[i]`. The problem is to find a starting point `s` such that the running sum of `diff` from `s` never goes below zero.
    *   **Insight 1:** If the total sum of `gas` is less than the total sum of `cost`, the trip is impossible.
    *   **Insight 2 (Greedy):** If we start at station `A` and run out of gas before reaching station `B`, it means that not only can we not start at `A`, but we also cannot start at any station *between* `A` and `B`. Why? Because we arrived at each of those intermediate stations with a non-negative tank from `A`. If we had started at one of them, our tank would have been even lower. Therefore, if we fail at `B`, the next possible starting point must be `B+1`.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Greedy Algorithm.
    2.  **Tool:** A few variables to track total fuel balance and current tank level.
    3.  **Approach:**
        *   First, check if `sum(gas) < sum(cost)`. If so, return -1.
        *   Initialize `start_station = 0` and `tank = 0`.
        *   Iterate through the stations from `i = 0` to `n-1`.
        *   Update the tank: `tank += gas[i] - cost[i]`.
        *   **Check for Failure:** If `tank < 0`:
            *   It means we cannot complete the trip starting from the current `start_station`.
            *   Based on Insight 2, the next possible start is `i + 1`. So, set `start_station = i + 1`.
            *   Reset the tank for the new attempt: `tank = 0`.
        *   Because we already confirmed a solution exists (the total sum is non-negative), the `start_station` we are left with at the end of the loop is the guaranteed correct answer. Return `start_station`.

### 40. LRU Cache

*   **Problem Description:** Design a data structure that follows the constraints of a Least Recently Used (LRU) cache. Implement the `LRUCache` class which supports `get(key)` and `put(key, value)` operations. The cache should be initialized with a positive `capacity`.

*   **Key Signals & Analysis:** This is a classic data structure design problem. We need two things:
    1.  Fast lookups by key. This immediately suggests a **Hash Map**.
    2.  An ordered structure to keep track of "recency." We need to be able to quickly identify and remove the *least* recently used item. A standard list or array would be too slow for moving items to the "most recent" position (O(N)). A structure that allows O(1) additions/removals from both ends is ideal. This is the exact use case for a **Doubly Linked List**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Combined Data Structure (Hash Map + Doubly Linked List).
    2.  **Tool:** A hash map and a custom doubly linked list implementation.
    3.  **Approach:**
        *   **Data Structures:**
            *   **Hash Map:** `map<key, DoublyLinkedListNode*>`. This gives O(1) access to any node in the list.
            *   **Doubly Linked List:** The nodes will store the `(key, value)` pairs. We will also maintain `head` and `tail` pointers.
        *   **`get(key)`:**
            *   Check if `key` exists in the hash map. If not, return -1.
            *   If it exists, get the `node` from the map.
            *   This node is now the most recently used. **Move it to the front** of the linked list. This involves re-wiring the `prev` and `next` pointers of its neighbors and then making it the new `head`.
            *   Return the `node.value`.
        *   **`put(key, value)`:**
            *   Check if `key` already exists in the hash map.
            *   If yes, update its `value` and move the corresponding node to the front of the list.
            *   If no:
                *   Check if the cache is at full `capacity`. If so, evict the least recently used item. This is the node at the **tail** of the list. Remove it from the linked list and from the hash map.
                *   Create a new node with the `(key, value)`.
                *   Add the new node to the front of the list.
                *   Add the new node to the hash map.

---

### 41. Climbing Stairs

*   **Problem Description:** You are climbing a staircase. It takes `n` steps to reach the top. Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

*   **Key Signals & Analysis:** This is a classic introductory **Dynamic Programming** problem. The number of ways to reach step `n`, let's call it `ways(n)`, can be determined by the ways to reach the steps just before it. To get to step `n`, you must have come from either step `n-1` (by taking a 1-step) or step `n-2` (by taking a 2-step). Therefore, `ways(n) = ways(n-1) + ways(n-2)`. This is the exact recurrence relation for the Fibonacci sequence.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A DP array, or just two variables to save space.
    3.  **Approach (Space Optimized):**
        *   Handle base cases: if `n <= 2`, return `n`.
        *   Initialize two variables representing the two previous steps: `one_step_before = 2` and `two_steps_before = 1`.
        *   Iterate from `i = 3` to `n`.
        *   In each iteration, calculate the ways for the current step: `current_ways = one_step_before + two_steps_before`.
        *   Update the variables for the next iteration: `two_steps_before = one_step_before` and `one_step_before = current_ways`.
        *   The final answer is `one_step_before`. This is an O(N) time, O(1) space solution.

### 42. House Robber

*   **Problem Description:** You are a professional robber planning to rob houses along a street. Each house has a certain amount of money. The only constraint stopping you is that adjacent houses have security systems connected, and it will automatically contact the police if two adjacent houses are robbed on the same night. Given an integer array `nums` representing the amount of money of each house, return the maximum amount of money you can rob tonight without alerting the police.

*   **Key Signals & Analysis:** This is another optimization problem with a clear "choice" at each step. For any house `i`, you have two choices:
    1.  **Rob house `i`:** In this case, you cannot rob house `i-1`. The total money would be `nums[i] +` the max money you could have robbed up to house `i-2`.
    2.  **Don't rob house `i`:** In this case, the total money is simply the max money you could have robbed up to house `i-1`.
    The optimal solution for house `i` depends on the optimal solutions for houses `i-1` and `i-2`. This is a perfect setup for **Dynamic Programming**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A DP array or two variables for space optimization.
    3.  **Approach (Space Optimized):**
        *   Handle base cases (e.g., if the array is empty or has one element).
        *   Initialize two variables: `rob_next` (representing the max money if we rob the *next* house in the sequence) and `rob_next_plus_one` (representing the max money if we rob the house *after* next). Let's initialize `rob_next = 0`, `rob_next_plus_one = 0`.
        *   Iterate through the `nums` array *backwards*.
        *   For each `num` in the reversed array, calculate the `current_max = max(num + rob_next_plus_one, rob_next)`.
        *   Update the variables: `rob_next_plus_one = rob_next`, and `rob_next = current_max`.
        *   The final answer is `rob_next`.

### 43. Number of 1 Bits

*   **Problem Description:** Write a function that takes an unsigned integer and returns the number of '1' bits it has (also known as the Hamming weight).

*   **Key Signals & Analysis:** This is a direct test of your knowledge of **Bit Manipulation**. While you could convert the number to a binary string and count the '1's, a much more efficient bitwise solution is expected.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Bit Manipulation.
    2.  **Tool:** Bitwise operators, specifically `&` (AND) and `>>` (right shift) or a clever `n & (n-1)` trick.
    3.  **Approach 1 (Loop and Shift):**
        *   Initialize `count = 0`.
        *   Loop 32 times (for a 32-bit integer).
        *   In each iteration, check if the last bit is a 1: `if (n & 1)`. If it is, increment `count`.
        *   Right-shift the number to process the next bit: `n = n >> 1`.
        *   Return `count`.
    4.  **Approach 2 (Brian Kernighan's Algorithm - more optimal):**
        *   Initialize `count = 0`.
        *   While `n != 0`:
            *   The operation `n = n & (n - 1)` cleverly flips the least significant '1' bit to a '0'.
            *   Increment `count`.
        *   Return `count`. This loop runs exactly as many times as there are '1' bits.

### 44. Pacific Atlantic Water Flow

*   **Problem Description:** You are given an `m x n` integer matrix `heights` representing the height of each cell. Water can flow from any cell to an adjacent cell (up, down, left, right) with an equal or lower height. Water can flow from the Pacific Ocean (top and left borders) and the Atlantic Ocean (bottom and right borders). Return a list of `[row, col]` coordinates where water can flow to **both** the Pacific and the Atlantic.

*   **Key Signals & Analysis:** The problem is about reachability. We need to find all cells that can reach the Pacific AND can reach the Atlantic. Instead of checking from every cell outwards (which would be inefficient), it's much faster to start from the oceans and flow "inwards". We can find all cells reachable from the Pacific, then all cells reachable from the Atlantic, and the intersection of these two sets is our answer.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Graph Traversal (Multi-source DFS or BFS).
    2.  **Tool:** Two boolean 2D arrays, `can_reach_pacific` and `can_reach_atlantic`, to store the results of our traversals.
    3.  **Approach:**
        *   Create the two boolean grids, initialized to `false`.
        *   **Pacific Flow:**
            *   Start a traversal (e.g., DFS) from every cell in the first row (top border) and every cell in the first column (left border).
            *   The DFS should only visit neighbors whose height is *greater than or equal to* the current cell's height (since we are flowing "uphill" from the ocean).
            *   Mark every cell visited during this traversal as `true` in the `can_reach_pacific` grid.
        *   **Atlantic Flow:**
            *   Start a new traversal from every cell in the last row (bottom border) and the last column (right border).
            *   Perform the same "uphill" DFS, marking visited cells as `true` in the `can_reach_atlantic` grid.
        *   **Find Intersection:** Iterate through the entire `heights` matrix. Any cell `(i, j)` where `can_reach_pacific[i][j]` AND `can_reach_atlantic[i][j]` are both `true` is part of the final result.

### 45. Longest Consecutive Sequence

*   **Problem Description:** Given an unsorted array of integers `nums`, return the length of the longest consecutive elements sequence. You must write an algorithm that runs in O(n) time.

*   **Key Signals & Analysis:** An O(N log N) solution is easy: sort the array and then iterate through it to find the longest sequence. The O(N) constraint forces a more clever approach. We can't use sorting. This suggests using a data structure that provides fast lookups, like a **Hash Set**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Array Manipulation using a Hash Set.
    2.  **Tool:** A hash set for O(1) lookups.
    3.  **Approach:**
        *   Insert all numbers from the `nums` array into a hash set for fast `contains` checks.
        *   Initialize `max_length = 0`.
        *   Iterate through each `num` in the `nums` array (or the set).
        *   **The Key Insight:** We only want to start counting a sequence from its *true beginning*. A number `num` is the start of a sequence if `num - 1` is **not** in the hash set.
        *   If `num - 1` is not in the set:
            *   This is the start of a new sequence. Initialize `current_length = 1`.
            *   Start a `while` loop to check for the next numbers in the sequence: `while (set.contains(num + 1))`.
            *   In the loop, increment `current_length` and `num`.
            *   After the loop, update `max_length = max(max_length, current_length)`.
        *   This approach is O(N) because although there is a nested `while` loop, that inner loop only ever runs for the elements that are part of a sequence, and each element is visited at most twice (once in the outer loop, and once in an inner loop).

---

### 46. Sliding Window Maximum

*   **Problem Description:** You are given an array of integers `nums` and an integer `k` representing the size of a sliding window. The sliding window moves from the very left of the array to the very right. You can only see the `k` numbers in the window. Return an array of the maximum values for each window.

*   **Key Signals & Analysis:** A naive approach would be to iterate through each window and find its maximum, leading to an O(N\*K) solution. The "sliding window" part is explicit, but the standard pattern isn't enough. We need a way to find the maximum in a moving window in better than O(K) time. This suggests a data structure that can efficiently add elements to the back and remove elements from the front while providing quick access to the maximum. This is a perfect use case for a **Deque (Double-Ended Queue)**, which forms the basis of the **Monotonic Queue** pattern.

*   **Strategic pattern & approach:**
    1.  **Pattern:** Monotonic Queue (a specialized sliding window pattern).
    2.  **Tool:** A Deque that will store the *indices* of elements in the current window.
    3.  **Approach:**
        *   Initialize a Deque and a `results` array.
        *   The Deque will be kept in monotonically decreasing order of element values. The front of the Deque will always hold the index of the maximum element in the current window.
        *   Iterate through the `nums` array with index `i`:
            *   **Maintain Monotonicity:** While the Deque is not empty and the element at the back of the Deque (`nums[deque.back()]`) is less than or equal to the current element (`nums[i]`), pop from the back. This removes smaller elements that can no longer be the maximum.
            *   **Add Current Element:** Push the current index `i` to the back of the Deque.
            *   **Remove Out-of-Bounds Elements:** If the index at the front of the Deque is outside the current window (i.e., `deque.front() <= i - k`), pop from the front.
            *   **Store Result:** Once the window is full (`i >= k - 1`), the maximum element for the current window is `nums[deque.front()]`. Add this to the `results` array.
        *   This provides an O(N) solution because each index is pushed and popped from the deque at most once.

### 47. Word Search II

*   **Problem Description:** Given an `m x n` `board` of characters and a list of strings `words`, return all words on the board. Each word must be constructed from letters of sequentially adjacent cells, and the same letter cell may not be used more than once in a word.

*   **Key Signals & Analysis:** This is a significant step up from "Word Search I". A naive solution would be to iterate through each word in the `words` list and perform a full backtracking search on the board for that word. This is extremely inefficient as we would be re-traversing the same paths repeatedly. The key insight is to search for all words *simultaneously*. This is a job for a **Trie (Prefix Tree)**.

*   **Strategic pattern & approach:**
    1.  **Pattern:** Backtracking / DFS combined with a Trie.
    2.  **Tool:** A Trie built from the `words` list, and a recursive backtracking function.
    3.  **Approach:**
        *   **Build Trie:** Insert all the words from the `words` list into a Trie. It's helpful to store the full word in the Trie node where it ends.
        *   **Backtracking Search:**
            *   Iterate through every cell `(row, col)` on the board to act as a starting point.
            *   Define a recursive function `search(row, col, trie_node)`.
            *   **Check Pruning:** In the search function, get the character `c` at `board[row][col]`. Check if `trie_node` has a child corresponding to `c`. If not, this path cannot form any word in our dictionary. Prune the search and return.
            *   **Move Down Trie:** If a child exists, move to that child node: `next_node = trie_node.children[c]`.
            *   **Check for Word:** If `next_node` marks the end of a word, we've found a match. Add the word to our results list. To avoid adding the same word multiple times, you can set the word marker in the Trie node to `null` after finding it.
            *   **Mark and Recurse:** Mark the current board cell as visited (e.g., change to `#`) and recursively call `search` for all four neighbors with the `next_node`.
            *   **Backtrack:** After the recursive calls return, restore the character on the board.

### 48. Decode Ways

*   **Problem Description:** A message containing letters from A-Z is being encoded to numbers using the mapping 'A' -> 1, 'B' -> 2, ..., 'Z' -> 26. Given a string `s` containing only digits, return the number of ways to decode it. For `s = "226"`, it could be decoded as "BZ" (2 26), "VF" (22 6), or "BBF" (2 2 6). The answer is 3.

*   **Key Signals & Analysis:** The problem asks for the "number of ways" and can be broken down into subproblems. The number of ways to decode a string of length `i` depends on the number of ways to decode strings of length `i-1` and `i-2`. This is a classic signal for **Dynamic Programming**.

*   **Strategic pattern & approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A DP array, `dp`, where `dp[i]` stores the number of ways to decode the prefix of the string of length `i`.
    3.  **Approach:**
        *   Create a `dp` array of size `n+1`.
        *   **Initialize:** `dp[0] = 1` (one way to decode an empty string) and `dp[1] = 1` if the first character is not '0'.
        *   Iterate from `i = 2` to `n`.
        *   For each `i`, consider the last one and two digits:
            *   **One-digit number:** Look at the `(i-1)`-th character of `s`. If it's a valid digit (1-9), we can add the number of ways to decode the string up to `i-1`. So, `dp[i] += dp[i-1]`.
            *   **Two-digit number:** Look at the substring from `(i-2)` to `(i-1)`. If this forms a valid number (10-26), we can add the number of ways to decode the string up to `i-2`. So, `dp[i] += dp[i-2]`.
        *   The final answer is `dp[n]`. Be careful with edge cases like leading zeros.

### 49. Partition Equal Subset Sum

*   **Problem Description:** Given a non-empty array `nums` containing only positive integers, find if the array can be partitioned into two subsets such that the sum of elements in both subsets is equal.

*   **Key Signals & Analysis:** This is a variation of the knapsack problem. First, if the total sum of the array is odd, partitioning into two equal-sum subsets is impossible. If the sum is even, let the `target_sum = total_sum / 2`. The problem is now reduced to: "Is there a subset of `nums` that sums up to `target_sum`?". This is a classic **Dynamic Programming (Subset Sum)** problem.

*   **Strategic pattern & approach:**
    1.  **Pattern:** Dynamic Programming (Subset Sum).
    2.  **Tool:** A boolean DP array, `dp`, of size `target_sum + 1`. `dp[i]` will be true if a sum of `i` can be achieved.
    3.  **Approach:**
        *   Calculate the `total_sum`. If it's odd, return `false`. Calculate `target_sum = total_sum / 2`.
        *   Create a boolean `dp` array of size `target_sum + 1`.
        *   **Initialize:** `dp[0] = true` (a sum of 0 is always achievable with an empty set).
        *   Iterate through each `num` in the `nums` array.
        *   For each `num`, iterate through the `dp` array *backwards*, from `j = target_sum` down to `num`.
            *   Update the `dp` table: `dp[j] = dp[j] || dp[j - num]`. This means a sum `j` is possible if it was already possible, OR if a sum `j-num` was possible (and we can now add `num` to it). The backward iteration is crucial to avoid using the same element multiple times within the same subset calculation.
        *   The final answer is `dp[target_sum]`.

### 50. Alien Dictionary

*   **Problem Description:** There is a new alien language that uses the English alphabet. However, the order among letters is unknown. You are given a list of `words` from the dictionary, which are sorted lexicographically by the rules of this new language. Derive the order of letters in this language. If the order is invalid, return an empty string.

*   **Key Signals & Analysis:** The problem is about finding a valid ordering based on prerequisite rules. The lexicographical sorting gives us dependency information. For example, if `"wrt"` comes before `"wrf"`, it implies that `'t'` must come before `'f'`. These dependency rules (`t -> f`) form a directed graph where the letters are the nodes. The problem then becomes finding a valid linear ordering of these nodes, which is exactly **Topological Sort**.

*   **Strategic pattern & approach:**
    1.  **Pattern:** Topological Sort.
    2.  **Tool:** An in-degree map/array and an adjacency list graph representation.
    3.  **Approach:**
        *   **Build the Graph:**
            *   Create an adjacency list and an in-degree map for all unique characters present in the words.
            *   Compare adjacent words in the list (`words[i]` and `words[i+1]`).
            *   Find the first character where they differ. Let this be `char1` from `words[i]` and `char2` from `words[i+1]`.
            *   This implies a dependency: `char1 -> char2`. Add this edge to the graph and increment the in-degree of `char2`.
            *   Handle invalid cases, like `["abc", "ab"]`.
        *   **Perform Topological Sort:**
            *   Initialize a queue with all characters that have an in-degree of 0.
            *   Use the standard Kahn's algorithm (as in Course Schedule II) to build the sorted order string.
            *   After the sort, if the length of the resulting string is equal to the number of unique characters, return the string. Otherwise, a cycle was detected, and the order is invalid, so return `""`.

---

### 31. Combination Sum

*   **Problem Description:** Given an array of **distinct** integers `candidates` and a target integer `target`, return a list of all unique combinations of `candidates` where the chosen numbers sum to `target`. You may return the combinations in any order. The **same number may be chosen from `candidates` an unlimited number of times**.

*   **Key Signals & Analysis:** The problem asks for **all possible combinations**, which is a primary signal for **Backtracking**. The key variations here are that the input numbers are distinct, but we can reuse the same number multiple times. This changes the recursive step slightly compared to a standard subset problem.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive helper function to build the combinations.
    3.  **Approach:**
        *   Define a recursive function `findCombinations(current_combination, remaining_target, start_index)`.
        *   **Base Case (Success):** If `remaining_target` is 0, we have found a valid combination. Add a copy of `current_combination` to the results and return.
        *   **Base Case (Failure):** If `remaining_target` is less than 0, this path is invalid. Return.
        *   **Recursive Step:**
            *   Iterate through the `candidates` array from `i = start_index` to the end.
            *   **The Re-use Logic:** The crucial part is how we recurse.
                *   **Make a choice:** Add `candidates[i]` to `current_combination`.
                *   **Recurse:** Call `findCombinations(current_combination, remaining_target - candidates[i], i)`. Notice that we pass `i` as the next `start_index`, not `i+1`. This allows the same number to be chosen again in the next level of recursion.
                *   **Undo the choice:** Remove `candidates[i]` from `current_combination` to backtrack and explore other possibilities.

### 32. Permutations

*   **Problem Description:** Given an array `nums` of **distinct** integers, return all the possible permutations. You can return the answer in any order.

*   **Key Signals & Analysis:** Again, the problem asks for **all possible** arrangements, which is a classic **Backtracking** problem. The core challenge in generating permutations is ensuring that each number is used exactly once in each permutation. We need a mechanism to track which numbers have already been included in the current permutation being built.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive helper function and a way to track used elements (either a boolean `used` array or by swapping elements).
    3.  **Approach (using a `used` array):**
        *   Define a recursive function `findPermutations(current_permutation, used_array)`.
        *   **Base Case:** If the size of `current_permutation` equals the size of `nums`, we have a complete permutation. Add a copy to the results and return.
        *   **Recursive Step:**
            *   Iterate through the `nums` array from `i = 0` to `n-1`.
            *   **Check Used:** If `used[i]` is `true`, skip this number as it's already in the current permutation.
            *   **Make a choice:**
                *   Mark `used[i] = true`.
                *   Add `nums[i]` to `current_permutation`.
            *   **Recurse:** Call `findPermutations(current_permutation, used_array)`.
            *   **Undo the choice (Backtrack):**
                *   Remove `nums[i]` from `current_permutation`.
                *   Mark `used[i] = false`.

### 33. Search in Rotated Sorted Array

*   **Problem Description:** You are given a sorted integer array `nums` with distinct values, which has been rotated at an unknown pivot. Given an integer `target`, return the index of `target` if it is in `nums`, or -1 if it is not. You must write an algorithm with O(log n) runtime complexity.

*   **Key Signals & Analysis:** The O(log N) complexity on a partially sorted array is an absolute giveaway for **Modified Binary Search**. The standard binary search logic of `if (target < mid)` go left, else go right, is broken. We must first determine which half of the array (relative to the `mid` pointer) is properly sorted, and then we can make an informed decision.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Modified Binary Search.
    2.  **Tool:** `left` and `right` pointers to manage the search space.
    3.  **Approach:**
        *   Initialize `left = 0`, `right = n-1`.
        *   While `left <= right`:
            *   Calculate `mid`.
            *   If `nums[mid] == target`, return `mid`.
            *   **Determine Sorted Half:**
                *   **Case 1: The left half is sorted (`nums[left] <= nums[mid]`).**
                    *   Check if the `target` lies within this sorted left half (`target >= nums[left] && target < nums[mid]`).
                    *   If yes, search the left half: `right = mid - 1`.
                    *   If no, the target must be in the unsorted right half: `left = mid + 1`.
                *   **Case 2: The right half is sorted (`nums[left] > nums[mid]`).**
                    *   Check if the `target` lies within this sorted right half (`target > nums[mid] && target <= nums[right]`).
                    *   If yes, search the right half: `left = mid + 1`.
                    *   If no, the target must be in the unsorted left half: `right = mid - 1`.
        *   If the loop finishes, the target was not found. Return -1.

### 34. Palindrome Partitioning

*   **Problem Description:** Given a string `s`, partition `s` such that every substring of the partition is a palindrome. Return all possible palindrome partitionings of `s`.

*   **Key Signals & Analysis:** This is another "find all possible" problem, indicating **Backtracking**. We need to make a series of choices. The choices are "where do I make the next cut?". We can make a cut after the first character, check if the resulting substring `s[0...0]` is a palindrome, and then recursively try to partition the rest of the string `s[1...]`. Then we backtrack and try making the cut after the second character, and so on.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive helper function and a function to check if a substring is a palindrome.
    3.  **Approach:**
        *   Define a recursive function `findPartitions(current_partition, start_index)`.
        *   **Base Case:** If `start_index` reaches the end of the string, it means we have successfully partitioned the entire string. Add a copy of `current_partition` to the results.
        *   **Recursive Step:**
            *   Iterate from `i = start_index` to the end of the string.
            *   Consider the substring from `start_index` to `i`.
            *   **Check Palindrome:** If this substring is a palindrome:
                *   **Make a choice:** Add this substring to `current_partition`.
                *   **Recurse:** Call `findPartitions(current_partition, i + 1)`.
                *   **Undo the choice:** Remove the substring from `current_partition` to backtrack.
        *   (Optimization: You can pre-compute all possible palindrome substrings using DP to make the palindrome check O(1)).

### 35. Unique Paths

*   **Problem Description:** A robot is located at the top-left corner of a `m x n` grid. The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid. How many possible unique paths are there?

*   **Key Signals & Analysis:** This is a classic combinatorics problem that can be modeled with **Dynamic Programming**. The number of ways to reach a cell `(i, j)` is the sum of the number of ways to reach the cell above it `(i-1, j)` and the number of ways to reach the cell to its left `(i, j-1)`. This reliance on the solutions to smaller subproblems is the core of DP.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A 2D DP grid, `dp[i][j]`, storing the number of unique paths to reach cell `(i, j)`.
    3.  **Approach:**
        *   Create a `dp` grid of size `m x n`.
        *   **Initialize:** Every cell in the first row (`dp[0][j]`) can only be reached from the left, so there is only 1 path. Initialize all to 1. Similarly, every cell in the first column (`dp[i][0]`) can only be reached from above, so there is only 1 path. Initialize all to 1.
        *   Iterate through the rest of the grid, from `i = 1` to `m-1` and `j = 1` to `n-1`.
        *   For each cell `dp[i][j]`, apply the recurrence relation: `dp[i][j] = dp[i-1][j] + dp[i][j-1]`.
        *   The final answer is the value in the bottom-right corner: `dp[m-1][n-1]`.

Of course. Here is the next set of competitive programming problems, focusing on more advanced and challenging applications of core patterns like graphs, dynamic programming, and sliding windows.

---

### 51. Network Delay Time

*   **Problem Description:** You are given a network of `n` nodes, labeled from `1` to `n`. You are also given `times`, a list of directed edges `[u, v, w]`, where `u` is the source node, `v` is the target node, and `w` is the time it takes for a signal to travel from `u` to `v`. We send a signal from a given node `k`. Return the minimum time it takes for all `n` nodes to receive the signal. If it's impossible for all nodes to receive the signal, return -1.

*   **Key Signals & Analysis:** This is a classic **shortest path** problem on a **weighted directed graph**. The "minimum time" for the signal to reach any node is the shortest path from the source `k` to that node. The total time for all nodes to receive the signal is determined by the node that receives it last, i.e., the length of the longest shortest path from the source `k`. The standard algorithm for finding the single-source shortest paths in a weighted graph with non-negative edge weights is **Dijkstra's Algorithm**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dijkstra's Algorithm.
    2.  **Tool:** An adjacency list to represent the graph, a distance array `dist` to store the shortest time from `k` to every other node, and a **Min-Priority Queue (Min-Heap)** to efficiently select the next node to visit.
    3.  **Approach:**
        *   Build the adjacency list from the `times` array. `graph[u] = list of (v, w) pairs`.
        *   Initialize a `dist` array of size `n+1` with infinity for all nodes, and set `dist[k] = 0`.
        *   Push the starting pair `(0, k)` (representing `(time, node)`) into the Min-Heap.
        *   While the heap is not empty:
            *   Pop the node `u` with the smallest time from the heap. If this node has been processed (i.e., its current `dist` is smaller than the popped time), continue.
            *   For each neighbor `v` with weight `w` of `u`:
                *   If `dist[u] + w < dist[v]`, we have found a shorter path to `v`.
                *   Update `dist[v] = dist[u] + w`.
                *   Push the new pair `(dist[v], v)` into the heap.
        *   After the loop, find the maximum value in the `dist` array. If any distance is still infinity, it means some nodes are unreachable, so return -1. Otherwise, return the maximum distance.

### 52. Minimum Window Substring

*   **Problem Description:** Given two strings `s` and `t`, return the minimum window in `s` which will contain all the characters in `t`. If there is no such window in `s` that covers all characters in `t`, return the empty string `""`.

*   **Key Signals & Analysis:** This is the quintessential hard **Dynamic Sliding Window** problem. The problem asks for the **minimum contiguous subarray** (substring) that satisfies a complex condition (contains all characters of `t`, including duplicates). The window will need to grow until the condition is met and then shrink from the left to find the smallest possible valid window.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Sliding Window.
    2.  **Tool:** Two hash maps: one to store the character frequencies of the target string `t` (`t_map`), and one for the current window in `s` (`window_map`). A counter is also needed to track how many characters from `t` we have satisfied.
    3.  **Approach:**
        *   Create `t_map` with character counts from `t`.
        *   Initialize `window_start = 0`, `min_length = infinity`, `result_start_index = 0`.
        *   Initialize `have` (number of characters in the window that match the required frequency in `t`) and `need` (number of unique characters in `t`).
        *   Iterate through `s` with `window_end`:
            *   **Grow Window:** Add `s[window_end]` to the `window_map`. If this character is in `t_map` and its count in `window_map` now matches its count in `t_map`, increment `have`.
            *   **Shrink Window:** While `have == need` (the window is valid):
                *   Update `min_length` and `result_start_index` if the current window is smaller.
                *   Remove `s[window_start]` from the window. If this character was in `t_map` and its count in `window_map` just dropped below its required count in `t_map`, decrement `have`.
                *   Increment `window_start`.
        *   If `min_length` remains infinity, no such window was found. Otherwise, return the substring of `s` using `result_start_index` and `min_length`.

### 53. N-Queens

*   **Problem Description:** The N-Queens puzzle is the problem of placing `n` chess queens on an `n x n` chessboard so that no two queens threaten each other. Given an integer `n`, return all distinct solutions to the N-Queens puzzle.

*   **Key Signals & Analysis:** This is a classic combinatorial problem that requires finding all valid configurations. The constraints (no two queens in the same row, column, or diagonal) mean that placing a queen in one position restricts many other positions. This process of making a choice, exploring the consequences, and then undoing the choice if it leads to a dead end is the definition of **Backtracking**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking.
    2.  **Tool:** A recursive helper function. To efficiently check for valid placements, we need sets or boolean arrays to track occupied columns and diagonals.
    3.  **Approach:**
        *   Initialize sets to track `occupied_cols`, `occupied_pos_diagonals` (where `row + col` is constant), and `occupied_neg_diagonals` (where `row - col` is constant).
        *   Define a recursive function `place_queen(row)`.
        *   **Base Case:** If `row == n`, we have successfully placed all `n` queens. Format the current board configuration and add it to the results.
        *   **Recursive Step:**
            *   Iterate through each `col` from `0` to `n-1` for the current `row`.
            *   **Check Constraints:** Check if the current `col`, `row + col`, and `row - col` are already in our occupied sets. If any are, this position is under attack, so `continue`.
            *   **Make a choice:**
                *   Place a queen at `(row, col)`.
                *   Add the `col`, `row + col`, and `row - col` to their respective occupied sets.
            *   **Recurse:** Call `place_queen(row + 1)`.
            *   **Undo the choice (Backtrack):**
                *   Remove the queen from `(row, col)`.
                *   Remove the `col`, `row + col`, and `row - col` from the sets to free up those lines of attack for other branches of the recursion.

### 54. Burst Balloons

*   **Problem Description:** You are given `n` balloons, indexed from `0` to `n-1`. Each balloon has a number painted on it represented by the array `nums`. You are asked to burst all the balloons. If you burst the `i`-th balloon, you will get `nums[left] * nums[i] * nums[right]` coins, where `left` and `right` are the adjacent indices to `i`. After the burst, `left` and `right` then become adjacent. Find the maximum coins you can collect.

*   **Key Signals & Analysis:** This is a notoriously difficult **Dynamic Programming** problem. A greedy approach of bursting the balloon that gives the most immediate coins fails. The key insight is to re-frame the problem. Instead of thinking "which balloon do I burst first?", think "which balloon do I burst **last**?". If the last balloon to be burst in a range `(i, j)` is `k`, then the problem breaks down into two independent subproblems: finding the max coins in the range `(i, k)` and in `(k, j)`, plus the coins from bursting `k` itself.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming (with a twist on the state definition).
    2.  **Tool:** A 2D DP table, `dp[i][j]`, storing the maximum coins obtainable from bursting all balloons in the range `(i, j)` (exclusive of `i` and `j`).
    3.  **Approach:**
        *   Pre-process `nums` by adding `1`s to both ends. This handles the boundary conditions.
        *   Create the `dp` table.
        *   Iterate over the length of the subarray `len` from 1 to `n`.
        *   Iterate over the left boundary `left` from 0 to `n - len`.
        *   Calculate the right boundary `right = left + len`.
        *   For each range `(left, right)`, iterate through `k` from `left + 1` to `right - 1`. This `k` is the last balloon to be burst in this range.
        *   The coins collected are `dp[left][k] + (nums[left] * nums[k] * nums[right]) + dp[k][right]`.
        *   Update `dp[left][right]` with the maximum value found across all possible `k`s.
        *   The final answer is in `dp[0][n+1]`.

### 55. Largest Rectangle in Histogram

*   **Problem Description:** Given an array of integers `heights` representing the bars of a histogram, find the area of the largest rectangle in the histogram.

*   **Key Signals & Analysis:** A brute-force O(N²) approach checks every bar as the potential shortest bar of the largest rectangle and expands left and right. To optimize to O(N), we need an efficient way to find, for each bar `i`, the first bar to its left that is shorter (`previous smaller element`) and the first bar to its right that is shorter (`next smaller element`). This is the exact problem that the **Monotonic Stack** pattern is designed to solve.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Monotonic Stack.
    2.  **Tool:** A stack that will store the indices of the histogram bars.
    3.  **Approach:**
        *   Initialize a stack and `max_area = 0`.
        *   Iterate through the `heights` array, including a conceptual '0' height bar at the end to flush the stack.
        *   While the stack is not empty and the current bar's height is less than the height of the bar at the stack's top index:
            *   This means the bar at the top of the stack has found its `next smaller element` (the current bar).
            *   Pop the index `h_idx` from the stack. The height is `heights[h_idx]`.
            *   Its `previous smaller element` is the new index at the top of the stack. The width of the rectangle with `heights[h_idx]` as the shortest bar is `i - stack.top() - 1`.
            *   Calculate the area and update `max_area`.
        *   Push the current index `i` onto the stack. The stack will always maintain indices of bars in increasing height order.
        *   Return `max_area`.

---

### 56. 3Sum

*   **Problem Description:** Given an integer array `nums`, return all the triplets `[nums[i], nums[j], nums[k]]` such that `i != j`, `i != k`, and `j != k`, and `nums[i] + nums[j] + nums[k] == 0`. The solution set must not contain duplicate triplets.

*   **Key Signals & Analysis:** A brute-force O(N³) solution would check every possible triplet. To optimize, we can fix one number and search for the other two. If we sort the array first, the problem of finding two numbers that sum to a target becomes the "Pair with Target Sum" problem. This leads to a **Two Pointers** approach nested inside a main loop. Handling duplicates is a key challenge.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Two Pointers.
    2.  **Tool:** Sorting the input array.
    3.  **Approach:**
        *   **Sort the array `nums`**. This is essential.
        *   Iterate through the array with a main pointer `i` from `0` to `n-3`.
        *   **Handle Duplicates (Outer Loop):** If `i > 0` and `nums[i] == nums[i-1]`, `continue` to the next iteration to avoid duplicate triplets starting with the same number.
        *   For each `i`, initialize `left = i + 1` and `right = n - 1`.
        *   While `left < right`:
            *   Calculate `current_sum = nums[i] + nums[left] + nums[right]`.
            *   If `current_sum == 0`, we found a valid triplet. Add it to the results.
                *   **Handle Duplicates (Inner Loop):** Move `left` forward while `left < right` and `nums[left] == nums[left+1]`. Then do a final `left++`. Do the same for `right`. This skips all duplicates for the second and third elements.
            *   If `current_sum < 0`, we need a larger sum, so `left++`.
            *   If `current_sum > 0`, we need a smaller sum, so `right--`.
        *   The time complexity is O(N²) due to the nested loop structure after an O(N log N) sort.

### 57. Rotate Image

*   **Problem Description:** You are given an `n x n` 2D matrix representing an image. Rotate the image by 90 degrees clockwise. You have to rotate the image **in-place**, which means you have to modify the input 2D matrix directly. DO NOT allocate another 2D matrix.

*   **Key Signals & Analysis:** This is a classic in-place matrix manipulation problem. A direct mathematical mapping of indices `(row, col) -> (col, n-1-row)` would require a second matrix. The in-place constraint forces a more clever, multi-step approach. A 90-degree clockwise rotation can be decomposed into two simpler, in-place operations: **transposing** the matrix and then **reversing** each row.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** In-place Matrix Manipulation.
    2.  **Tool:** Nested loops for traversal.
    3.  **Approach:**
        *   **Step 1: Transpose the Matrix.** A transpose swaps `matrix[i][j]` with `matrix[j][i]`.
            *   Iterate with `i` from `0` to `n-1`.
            *   Iterate with `j` from `i` to `n-1` (to avoid swapping elements back).
            *   Swap `matrix[i][j]` and `matrix[j][i]`.
        *   **Step 2: Reverse Each Row.**
            *   Iterate through each row of the now-transposed matrix.
            *   Use a standard two-pointer approach to reverse the elements in that row.

### 58. Find First and Last Position of Element in Sorted Array

*   **Problem Description:** Given an array of integers `nums` sorted in non-decreasing order, find the starting and ending position of a given `target` value. If `target` is not found in the array, return `[-1, -1]`. You must write an algorithm with O(log n) runtime complexity.

*   **Key Signals & Analysis:** The O(log N) complexity on a sorted array is an explicit command to use **Binary Search**. A standard binary search will find *an* instance of the target, but not necessarily the first or last one. Therefore, we need to run a modified binary search twice: once to find the leftmost boundary and once to find the rightmost boundary.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Modified Binary Search.
    2.  **Tool:** Two separate binary search functions or a single one with a flag.
    3.  **Approach:**
        *   **`find_left_boundary(nums, target)`:**
            *   Perform a binary search.
            *   When `nums[mid] == target`, this *could* be the first occurrence. We record the index and try to find an even earlier one by searching in the left half: `right = mid - 1`.
        *   **`find_right_boundary(nums, target)`:**
            *   Perform a binary search.
            *   When `nums[mid] == target`, this *could* be the last occurrence. We record the index and try to find an even later one by searching in the right half: `left = mid + 1`.
        *   Call both functions and return the results in an array.

### 59. Best Time to Buy and Sell Stock

*   **Problem Description:** You are given an array `prices` where `prices[i]` is the price of a given stock on the `i`-th day. You want to maximize your profit by choosing a single day to buy one stock and choosing a different day in the future to sell that stock. Return the maximum profit you can achieve. If you cannot achieve any profit, return 0.

*   **Key Signals & Analysis:** This is an optimization problem. A brute-force O(N²) solution would check every possible pair of buy and sell days. To achieve an O(N) solution, we need a single-pass approach. As we iterate through the `prices` array, we need to keep track of two things: the maximum profit seen so far, and the minimum stock price encountered up to the current day.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Single-pass Greedy / Array Traversal.
    2.  **Tool:** Two variables: `min_price` and `max_profit`.
    3.  **Approach:**
        *   Initialize `min_price` to a very large value and `max_profit` to 0.
        *   Iterate through the `prices` array.
        *   For each `price`:
            *   **Update Minimum:** If the current `price` is less than `min_price`, update `min_price = price`. This is a better day to buy.
            *   **Calculate Potential Profit:** If the current `price` is greater than `min_price`, calculate the potential profit: `profit = price - min_price`.
            *   **Update Maximum Profit:** Update `max_profit = max(max_profit, profit)`.
        *   Return `max_profit`.

### 60. Spiral Matrix

*   **Problem Description:** Given an `m x n` matrix, return all elements of the matrix in spiral order.

*   **Key Signals & Analysis:** This problem is a direct test of careful bookkeeping and boundary management. The core idea is to "peel" the outer layer of the matrix, then move inwards and peel the next layer, and so on, until the center is reached. This is a simulation problem.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Matrix Traversal with Boundary Simulation.
    2.  **Tool:** Four boundary variables: `top`, `bottom`, `left`, `right`.
    3.  **Approach:**
        *   Initialize `top = 0`, `bottom = m-1`, `left = 0`, `right = n-1`.
        *   Initialize a results list.
        *   Start a `while` loop that continues as long as `top <= bottom` and `left <= right`.
        *   **Go Right:** Traverse the top row from `left` to `right`, adding each element to the results. After this, increment `top`.
        *   **Go Down:** Traverse the rightmost column from `top` to `bottom`. After this, decrement `right`.
        *   **Go Left (with check):** If `top <= bottom`, traverse the bottom row from `right` to `left`. After this, decrement `bottom`.
        *   **Go Up (with check):** If `left <= right`, traverse the leftmost column from `bottom` to `top`. After this, increment `left`.
        *   The checks are necessary for non-square matrices where the pointers might cross after a horizontal or vertical pass.
        *   Continue the loop until the boundaries cross. Return the results list.

---

### 61. Clone Graph

*   **Problem Description:** Given a reference to a node in a connected undirected graph, return a deep copy (clone) of the graph. Each node in the graph contains a value and a list of its neighbors.

*   **Key Signals & Analysis:** This problem requires a full traversal of the graph while building a new one. The key challenge is handling cycles and ensuring that each original node maps to exactly one new node. A simple recursive traversal would lead to an infinite loop in a cyclic graph. This necessitates keeping track of visited/cloned nodes.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Graph Traversal (DFS or BFS).
    2.  **Tool:** A **Hash Map** `(original_node -> cloned_node)` to serve as a "visited" set and to store references to already created clones.
    3.  **Approach (DFS):**
        *   Create a hash map `cloned_map`.
        *   Define a recursive function `clone(node)`.
        *   If `node` is already in `cloned_map`, return the stored clone.
        *   Create a `new_node` with the same value as `node`.
        *   **Crucially, add the mapping `cloned_map[node] = new_node` *before* recurring on neighbors.**
        *   For each `neighbor` of the original `node`, recursively call `clone(neighbor)` and add the result to the `new_node`'s neighbor list.
        *   Return `new_node`.

### 62. Number of Islands

*   **Problem Description:** Given an `m x n` 2D grid map of `'1'`s (land) and `'0'`s (water), count the number of distinct islands. An island is formed by connecting adjacent lands horizontally or vertically.

*   **Key Signals & Analysis:** This is the quintessential **connected components** problem on an implicit graph. Each cell is a node, and an edge exists between adjacent land cells. We need to count how many separate "blobs" of land exist.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Graph Traversal (DFS or BFS).
    2.  **Tool:** A `visited` 2D boolean array to avoid re-processing cells.
    3.  **Approach:**
        *   Initialize `count = 0` and a `visited` grid.
        *   Iterate through every cell `(r, c)` of the grid.
        *   If `grid[r][c] == '1'` and the cell has not been visited:
            *   Increment `count`.
            *   Start a traversal (DFS is often simpler to write recursively) from `(r, c)`.
            *   The traversal function should mark the current cell as visited and then recursively call itself for all four adjacent land neighbors. This effectively "sinks" the entire island, ensuring it's only counted once.

### 63. Course Schedule

*   **Problem Description:** You are given `numCourses` and a list of prerequisite pairs `[a, b]`, which means you must take course `b` before course `a`. Determine if it's possible to finish all courses.

*   **Key Signals & Analysis:** The problem describes a set of dependencies. This is a directed graph where an edge `b -> a` means `b` is a prerequisite for `a`. Being able to finish all courses is equivalent to the graph having **no cycles**. Cycle detection in a directed graph is a primary application of **Topological Sort**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Topological Sort (Kahn's Algorithm is common).
    2.  **Tool:** An adjacency list and an in-degree array. A queue to hold nodes with an in-degree of 0.
    3.  **Approach:**
        *   Build the graph and in-degree array from the prerequisite list.
        *   Initialize a queue with all nodes that have an in-degree of 0 (no prerequisites).
        *   Keep a count of processed courses.
        *   While the queue is not empty:
            *   Dequeue a course `u`. Increment the processed course count.
            *   For each neighbor `v` of `u`, decrement its in-degree.
            *   If `v`'s in-degree becomes 0, enqueue it.
        *   After the loop, if the `processed course count` equals `numCourses`, return `true`. Otherwise, a cycle was present, and some nodes were never processed, so return `false`.

### 64. Pacific Atlantic Water Flow

*   **Problem Description:** Given an `m x n` grid of cell heights, find all cells from which water can flow to *both* the Pacific (top/left borders) and Atlantic (bottom/right borders) oceans. Water flows to adjacent cells of equal or lower height.

*   **Key Signals & Analysis:** This is a reachability problem. Instead of checking outwards from every cell, it's more efficient to check "inwards" from the oceans. We can find all cells reachable by the Pacific and all cells reachable by the Atlantic. The answer is the intersection of these two sets.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Multi-source Graph Traversal (DFS or BFS).
    2.  **Tool:** Two boolean grids, `pacific_reachable` and `atlantic_reachable`.
    3.  **Approach:**
        *   Create the two boolean grids.
        *   Start a DFS/BFS from all cells on the Pacific border (top and left rows). The traversal rule is reversed: you can move from a cell to a neighbor if the neighbor's height is **greater than or equal to** the current cell's height. Mark all visited cells in `pacific_reachable`.
        *   Do the same for all cells on the Atlantic border (bottom and right rows), marking visited cells in `atlantic_reachable`.
        *   Iterate through the entire grid and collect all coordinates `(r, c)` where both `pacific_reachable[r][c]` and `atlantic_reachable[r][c]` are true.

### 65. Word Ladder

*   **Problem Description:** Given a `beginWord`, an `endWord`, and a dictionary `wordList`, find the length of the **shortest** transformation sequence from `beginWord` to `endWord`, where each step involves changing one letter and the resulting word must be in the `wordList`.

*   **Key Signals & Analysis:** The keyword **"shortest"** in an unweighted graph (each transformation is one step) is a definitive signal for **Breadth-First Search (BFS)**. The words are the nodes, and an edge exists between two words if they differ by one letter.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Breadth-First Search (BFS).
    2.  **Tool:** A queue to store `(word, level)` pairs and a `visited` set.
    3.  **Approach:**
        *   Add the `wordList` to a set for O(1) lookups.
        *   Push `(beginWord, 1)` to a queue.
        *   While the queue is not empty:
            *   Dequeue `(current_word, level)`.
            *   If `current_word == endWord`, return `level`.
            *   Generate all possible one-letter-change neighbors of `current_word`.
            *   For each neighbor, if it's in the word set and hasn't been visited, add it to the visited set and enqueue it with `level + 1`.

### 66. Rotting Oranges

*   **Problem Description:** In a grid of fresh oranges (1), rotten oranges (2), and empty cells (0), every minute a fresh orange adjacent to a rotten one becomes rotten. Find the **minimum time** until no fresh oranges remain.

*   **Key Signals & Analysis:** This is another "shortest time" problem where a process spreads simultaneously from multiple sources. The level-by-level nature of the rotting process is a perfect match for **Multi-source Breadth-First Search**. Each level in the BFS corresponds to one minute passing.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Multi-source BFS.
    2.  **Tool:** A queue to store the coordinates of rotten oranges.
    3.  **Approach:**
        *   Initialize a queue with the coordinates of all initially rotten oranges.
        *   Count the initial number of fresh oranges.
        *   Perform a level-by-level BFS. In each level (minute):
            *   Process all oranges currently in the queue.
            *   For each rotten orange, turn its fresh neighbors rotten, decrement the fresh orange count, and add them to the queue for the next level.
        *   Keep track of the number of levels (minutes).
        *   After the BFS, if the fresh orange count is 0, return the number of minutes. Otherwise, some were unreachable, so return -1.

### 67. Redundant Connection

*   **Problem Description:** In this problem, a tree is an undirected graph that is connected and has no cycles. You are given a graph that started as a tree with `n` nodes and had one additional edge added. The added edge creates exactly one cycle. Return an edge that can be removed so that the resulting graph is a tree.

*   **Key Signals & Analysis:** The problem is about finding the edge that creates a cycle in an undirected graph. As we add edges one by one, we need an efficient way to check if an edge connects two nodes that are already in the same connected component. This is the exact problem that the **Union-Find (Disjoint Set Union)** data structure is designed to solve.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Union-Find (DSU).
    2.  **Tool:** A Union-Find data structure.
    3.  **Approach:**
        *   Initialize a DSU structure for `n` nodes.
        *   Iterate through each `edge [u, v]` in the input list.
        *   For each edge, perform a `find` operation on both `u` and `v`.
        *   If `find(u)` is the same as `find(v)`, it means `u` and `v` are already in the same component. Adding this edge would create a cycle. This is the redundant edge. Return it.
        *   If they are in different components, perform a `union(u, v)` operation to connect them.

---

### 68. Accounts Merge

*   **Problem Description:** Given a list of accounts, where each account is `[name, email1, email2, ...]`, merge all accounts belonging to the same person. Two accounts are for the same person if they share a common email.

*   **Key Signals & Analysis:** This is a problem about finding **connected components**, but the graph is not explicitly given. The elements are the emails, and an edge exists between any two emails that appear in the same account. All emails in one component belong to the same person. While a standard DFS/BFS approach works, the **Union-Find (DSU)** pattern is often more elegant and efficient for dynamically connecting elements into sets.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Union-Find (DSU) or Graph Traversal (DFS/BFS).
    2.  **Tool:** A DSU data structure, a map from email to its component ID, and a map from email to the person's name.
    3.  **Approach (DSU):**
        *   Assign a unique integer ID to each email and map emails to these IDs. Also, map each email to its owner's name.
        *   Initialize a DSU structure for the total number of unique emails.
        *   Iterate through each account. For each account, perform a `union` operation between the first email's ID and every other email's ID in that account. This groups all emails for one person into a single component.
        *   Create a results map `(component_root_id -> list_of_emails)`.
        *   Iterate through all unique emails. For each email, `find` its component root and add the email to the corresponding list in the results map.
        *   Format the results map into the required output list `[name, sorted_email_1, ...]`.

### 69. Minimum Spanning Tree (MST) - (e.g., Min Cost to Connect All Points)

*   **Problem Description:** You are given an array `points` representing integer coordinates of some points on a 2D-plane. The cost of connecting two points `[xi, yi]` and `[xj, yj]` is their Manhattan distance. Return the minimum cost to make all points connected. All points are connected if there is exactly one simple path between any two points.

*   **Key Signals & Analysis:** The problem asks for the **minimum cost** to connect **all** nodes in a graph. This is the exact definition of a **Minimum Spanning Tree (MST)**. The points are the vertices, and the edges are the connections between all possible pairs of points, with the edge weight being the Manhattan distance. The two main algorithms for finding an MST are Kruskal's and Prim's.

*   **Strategic Pattern & Approach (Kruskal's Algorithm):**
    1.  **Pattern:** Minimum Spanning Tree (Kruskal's Algorithm).
    2.  **Tool:** A list of all possible edges and a **Union-Find (DSU)** data structure.
    3.  **Approach:**
        *   Generate all possible edges between every pair of points, calculating the Manhattan distance as the weight for each edge. This will be a list of `(weight, point1_idx, point2_idx)`.
        *   **Sort all edges** by their weight in ascending order.
        *   Initialize a DSU structure for `n` points, `total_cost = 0`, and `edges_used = 0`.
        *   Iterate through the sorted edges:
            *   For an edge `(w, u, v)`, check if `u` and `v` are already connected using `find(u) == find(v)`.
            *   If they are not connected, this edge will not form a cycle. Add it to the MST:
                *   `total_cost += w`.
                *   Perform a `union(u, v)`.
                *   Increment `edges_used`.
            *   If `edges_used == n - 1`, we have connected all points. Stop and return `total_cost`.

### 70. Is Graph Bipartite?

*   **Problem Description:** Given an undirected graph, return `true` if and only if it is bipartite. A graph is bipartite if we can partition its vertices into two independent sets, `A` and `B`, such that every edge in the graph connects a vertex in set `A` and a vertex in set `B`.

*   **Key Signals & Analysis:** This is equivalent to asking if the graph can be **2-colored**. We can assign each node one of two colors (say, 1 and -1) such that no two adjacent nodes have the same color. This is a coloring problem that can be solved with a graph traversal.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Graph Traversal (DFS or BFS).
    2.  **Tool:** A `colors` array to store the color of each node (0 for uncolored, 1 for color A, -1 for color B).
    3.  **Approach (BFS):**
        *   Initialize a `colors` array with all 0s.
        *   Iterate through all nodes from `0` to `n-1` (to handle disconnected graphs).
        *   If a node is uncolored (`colors[i] == 0`):
            *   Start a BFS from this node.
            *   Assign it the first color: `colors[i] = 1`.
            *   Add it to a queue.
            *   While the queue is not empty:
                *   Dequeue a node `u`.
                *   For each `neighbor` of `u`:
                    *   If the `neighbor` is uncolored, assign it the opposite color of `u` (`colors[neighbor] = -colors[u]`) and enqueue it.
                    *   If the `neighbor` *has* a color and it's the *same* as `u`'s color (`colors[neighbor] == colors[u]`), we have a conflict. The graph is not bipartite. Return `false`.
        *   If the entire process completes without conflict, return `true`.

### 71. Cheapest Flights Within K Stops

*   **Problem Description:** You are given `n` cities connected by some number of flights. You are given an array `flights` where `flights[i] = [from, to, price]`. You are also given three integers `src`, `dst`, and `k`. Return the cheapest price to travel from `src` to `dst` with at most `k` stops. If there is no such route, return -1.

*   **Key Signals & Analysis:** This is a shortest path problem on a weighted graph, but with an added constraint: the number of stops. Standard Dijkstra's algorithm finds the shortest path by distance but doesn't track the number of edges. This structure suggests a modification of a standard shortest path algorithm. The **Bellman-Ford algorithm** is a good fit because it works by iteratively relaxing edges, which naturally corresponds to increasing the number of stops. A modified BFS where the state includes cost and stops also works well.

*   **Strategic Pattern & Approach (Bellman-Ford variant):**
    1.  **Pattern:** Modified Bellman-Ford Algorithm.
    2.  **Tool:** Two arrays to store costs: one for the previous iteration and one for the current.
    3.  **Approach:**
        *   Initialize a `prices` array of size `n` with infinity, and set `prices[src] = 0`.
        *   Loop `k+1` times (representing 0 stops, 1 stop, ..., k stops).
        *   Inside the loop, create a temporary copy of the `prices` array, `temp_prices`.
        *   For each flight `[from, to, price]` in the `flights` list:
            *   If `prices[from]` is not infinity, it means we can reach the `from` city.
            *   Relax the edge: `temp_prices[to] = min(temp_prices[to], prices[from] + price)`.
        *   After iterating through all flights, copy `temp_prices` back to `prices`.
        *   After `k+1` iterations, `prices[dst]` will hold the cheapest price with at most `k` stops. If it's still infinity, the destination is unreachable.

### 72. Alien Dictionary

*   **Problem Description:** You are given a list of words from an alien language, sorted lexicographically. Derive the a possible order of letters in this language.

*   **Key Signals & Analysis:** This was covered before but is a quintessential advanced graph problem worth repeating. The lexicographical order provides dependency rules. If `"apple"` comes before `"apply"`, it tells us nothing. If `"wrt"` comes before `"wrf"`, it tells us `'t'` must come before `'f'`. This forms a directed graph of dependencies, and we need a linear ordering, which is a **Topological Sort**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Topological Sort.
    2.  **Tool:** Adjacency list, in-degree map, and a queue for source nodes.
    3.  **Approach:**
        *   Build the graph by comparing adjacent words to find the first differing character, which gives you a dependency rule (e.g., `t -> f`).
        *   Perform Kahn's algorithm (BFS-based topological sort).
        *   If the final sorted list of characters includes all unique characters found, return it. Otherwise, a cycle exists, and the order is invalid, so return an empty string.

---

### 73. Word Search II

*   **Problem Description:** Given an `m x n` `board` of characters and a list of strings `words`, return all words on the board that can be formed by sequentially adjacent cells. The same letter cell may not be used more than once in a word.

*   **Key Signals & Analysis:** This is a significant step up from "Word Search I". Naively searching for each word one by one is too slow. The key is to search for all words simultaneously. This requires a data structure that can efficiently check if the current path on the board corresponds to a valid prefix of *any* word in the dictionary. This is the prime use case for a **Trie (Prefix Tree)**, combined with a **Backtracking (DFS)** search.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking (DFS) with a Trie.
    2.  **Tool:** A Trie built from the `words` list.
    3.  **Approach:**
        *   **Build Trie:** Insert all words from the `words` list into a Trie. Store the full word at the node where it terminates.
        *   **Search Board:** Iterate through every cell of the board. If the character at a cell is a valid starting letter (i.e., exists as a child of the Trie root), start a DFS from that cell.
        *   **DFS Function `search(row, col, trie_node)`:**
            *   Get the character `c` at `(row, col)`. Check if `trie_node` has a child for `c`. If not, this path is invalid, so prune and return.
            *   Move to the `next_trie_node`.
            *   If `next_trie_node` contains a complete word, add it to the results (and mark it as found in the Trie to avoid duplicates).
            *   Mark the board cell as visited (e.g., `board[r][c] = '#'`).
            *   Recursively call `search` for all valid neighbors.
            *   Backtrack by un-marking the board cell.

### 74. Reconstruct Itinerary

*   **Problem Description:** You are given a list of airline tickets `[from, to]`. All of the tickets belong to a man who departs from "JFK". The itinerary must use all tickets exactly once. Return the itinerary as a list of airports. If there are multiple valid itineraries, you should return the one that has the smallest lexical order.

*   **Key Signals & Analysis:** This is a graph traversal problem where we need to find a specific path that uses every edge exactly once. This is known as finding a **Eulerian Path**. The requirement for the "smallest lexical order" provides a greedy choice: when at an airport, always try to fly to the lexically smallest destination first. A post-order DFS (Hierholzer's algorithm) is a standard and elegant way to solve this.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Hierholzer's Algorithm (Post-order DFS).
    2.  **Tool:** An adjacency list (represented as a map where the values are sorted lists or min-priority queues to handle lexical order) and a result list.
    3.  **Approach:**
        *   Build the graph: `map<string, priority_queue<string, vector<string>, greater<string>>>`. A min-priority queue naturally handles the lexical order requirement.
        *   Define a DFS function `dfs(airport)`.
        *   Inside `dfs(airport)`, while there are still unvisited tickets from this `airport`:
            *   Get the next destination from the priority queue (this is the greedy, lexically smallest choice).
            *   Recursively call `dfs(destination)`.
        *   **Post-order step:** After the while loop finishes (meaning all outgoing paths from the current `airport` have been fully explored), add the current `airport` to the *front* of the result list.
        *   Start the process by calling `dfs("JFK")`.
        *   The final result list will be the correct itinerary.

### 75. Number of Connected Components in an Undirected Graph

*   **Problem Description:** You have a graph of `n` nodes. You are given an integer `n` and an array `edges` where `edges[i] = [ai, bi]` indicates that there is an edge between `ai` and `bi`. Return the number of connected components in the graph.

*   **Key Signals & Analysis:** This is the same fundamental problem as "Number of Islands," but for a general graph instead of a grid. We need to count the distinct, unconnected subgraphs. This can be solved with either a standard graph traversal or, often more efficiently, with Union-Find.

*   **Strategic Pattern & Approach (Union-Find):**
    1.  **Pattern:** Union-Find (DSU).
    2.  **Tool:** A DSU data structure.
    3.  **Approach:**
        *   Initialize a DSU with `n` components (one for each node).
        *   Iterate through each edge `[u, v]` in the `edges` list.
        *   For each edge, perform a `union(u, v)` operation. The DSU will automatically handle the merging of components and will only decrement the component count if `u` and `v` were not already in the same set.
        *   The final number of components is the count maintained by the DSU structure after processing all edges.

### 76. Graph Valid Tree

*   **Problem Description:** You are given `n` nodes labeled from `0` to `n-1` and a list of undirected edges. Return `true` if the edges of the given graph make up a valid tree, and `false` otherwise.

*   **Key Signals & Analysis:** For a graph to be a valid tree, two conditions must be met:
    1.  **It must be fully connected.** All nodes must belong to a single connected component.
    2.  **It must have no cycles.**
    A property of trees is that for `n` nodes, a tree must have exactly `n-1` edges. We can use this as a quick initial check. If the number of edges is not `n-1`, it cannot be a tree. If it is `n-1`, we then only need to check for either full connectivity or the absence of cycles (one implies the other if the edge count is correct).

*   **Strategic Pattern & Approach (Union-Find):**
    1.  **Pattern:** Union-Find (DSU).
    2.  **Tool:** A DSU data structure.
    3.  **Approach:**
        *   Check if `len(edges) != n - 1`. If so, return `false`.
        *   Initialize a DSU with `n` nodes.
        *   Iterate through the edges `[u, v]`:
            *   If `find(u) == find(v)`, it means `u` and `v` are already connected, and this edge creates a cycle. Return `false`.
            *   Otherwise, perform `union(u, v)`.
        *   If the loop completes without finding any cycles, and we already know the edge count is correct, it must be a valid tree. Return `true`.

### 77. Critical Connections in a Network

*   **Problem Description:** You are given a network of servers represented by an undirected graph. A critical connection is an edge that, if removed, will increase the number of connected components in the network. Find all critical connections.

*   **Key Signals & Analysis:** These critical connections are also known as **bridges** in graph theory. A naive approach of removing each edge and checking for connectivity is too slow (O(E \* (V+E))). The efficient solution is a specialized DFS-based algorithm known as **Tarjan's Bridge-Finding Algorithm**. It works by keeping track of the "discovery time" of each node and the "low-link" value, which is the lowest discovery time reachable from that node (including itself).

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Tarjan's Bridge-Finding Algorithm (Advanced DFS).
    2.  **Tool:** A DFS traversal with extra bookkeeping: a `discovery_time` for each node and a `low_link` value for each node.
    3.  **Approach:**
        *   Initialize `discovery_time` and `low_link` arrays, and a global `time` counter.
        *   Start a DFS from an arbitrary node.
        *   **In the DFS for node `u` (with parent `p`):**
            *   Set `discovery_time[u] = low_link[u] = time++`.
            *   For each `neighbor` `v` of `u`:
                *   If `v` is the parent `p`, ignore it.
                *   If `v` has been visited: update `low_link[u] = min(low_link[u], discovery_time[v])`.
                *   If `v` has not been visited:
                    *   Recursively call `dfs(v, u)`.
                    *   After the recursive call returns, update `low_link[u] = min(low_link[u], low_link[v])`.
                    *   **The Bridge Condition:** If `low_link[v] > discovery_time[u]`, it means there is no "back-edge" from `v` or any of its descendants to `u` or any of its ancestors. Therefore, the edge `(u, v)` is a bridge. Add it to the results.

---

### 78. Sliding Puzzle

*   **Problem Description:** On a 2x3 board, there are 5 tiles labeled 1-5 and an empty square labeled 0. A move consists of choosing 0 and a 4-directionally adjacent number and swapping it. Given an initial state of the board, return the least number of moves required to solve the puzzle. The solved state is `[[1,2,3],[4,5,0]]`. If it is impossible, return -1.

*   **Key Signals & Analysis:** This is a classic **shortest path** problem on an implicit graph. Each possible configuration of the board is a "node," and a "move" is a directed edge to a new configuration. Since each move has a weight of 1, the problem of finding the "least number of moves" is a perfect fit for **Breadth-First Search (BFS)**. The main challenge is representing the board state in a way that can be easily stored in a queue and a visited set.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** BFS for Shortest Path in a State-Space Graph.
    2.  **Tool:** A queue to store `(board_state, moves)` tuples and a hash set `visited` to store board states that have already been processed.
    3.  **Approach:**
        *   **State Representation:** Flatten the 2x3 board into a 6-character string (e.g., `[[1,2,3],[4,5,0]]` becomes `"123450"`). This is hashable and easy to store.
        *   Initialize a queue with the starting board state string and 0 moves.
        *   Add the starting state to the `visited` set.
        *   While the queue is not empty:
            *   Dequeue `(current_state, current_moves)`.
            *   If `current_state` is the target `"123450"`, return `current_moves`.
            *   Find the index of '0' in `current_state`.
            *   For each valid swap direction from the '0' position:
                *   Generate the `next_state` string by performing the swap.
                *   If `next_state` has not been visited:
                    *   Add `next_state` to the `visited` set.
                    *   Enqueue `(next_state, current_moves + 1)`.
        *   If the queue becomes empty and the target was not found, it's impossible. Return -1.

### 79. Path with Maximum Probability

*   **Problem Description:** You are given an undirected weighted graph where the weight of an edge is the probability of success of traversing that edge. Given a start and an end node, find the path with the maximum probability of success.

*   **Key Signals & Analysis:** This looks like a shortest path problem, but instead of minimizing a sum of weights (like distance), we want to **maximize a product** of weights (probabilities). Standard shortest path algorithms are designed for additive costs. We can transform this into an additive problem by working with the logarithms of the probabilities. `log(a * b) = log(a) + log(b)`. Maximizing the product `p1 * p2 * ...` is equivalent to maximizing the sum `log(p1) + log(p2) + ...`. Since probabilities are `(0, 1]`, their logs are `(-inf, 0]`. Maximizing this sum is equivalent to minimizing the sum of the negative logs. This transforms the problem into a standard shortest path problem that can be solved with **Dijkstra's Algorithm**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dijkstra's Algorithm (on transformed weights).
    2.  **Tool:** An adjacency list, a `max_probabilities` array, and a **Max-Priority Queue** (or a Min-PQ with negative probabilities).
    3.  **Approach:**
        *   Build an adjacency list where `graph[u]` stores pairs of `(v, probability)`.
        *   Initialize a `max_prob` array with all 0s, and set `max_prob[start] = 1.0`.
        *   Push `(1.0, start)` `(probability, node)` into a Max-Priority Queue.
        *   While the queue is not empty:
            *   Pop the node `u` with the highest probability.
            *   If the popped probability is less than `max_prob[u]`, continue (we found a better path already).
            *   For each neighbor `v` with edge probability `p`:
                *   Calculate `new_prob = max_prob[u] * p`.
                *   If `new_prob > max_prob[v]`:
                    *   Update `max_prob[v] = new_prob`.
                    *   Push `(new_prob, v)` into the queue.
        *   The final answer is `max_prob[end]`.

### 80. Optimize Water Distribution in a Village

*   **Problem Description:** There is a village of `n` houses. You can either build a well in a house `i` with cost `wells[i-1]`, or lay a pipe between two houses `i` and `j` with cost `pipes[k]`. Find the minimum total cost to supply water to all houses.

*   **Key Signals & Analysis:** This problem is about connecting all houses to a water source at the minimum possible cost. This is a classic **Minimum Spanning Tree (MST)** problem, but with a clever twist. Building a well can be modeled as connecting a house to a "virtual" water source node.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Minimum Spanning Tree (Kruskal's or Prim's Algorithm).
    2.  **Tool:** A list of all possible edges and a Union-Find data structure (for Kruskal's).
    3.  **Approach (Kruskal's):**
        *   **Model the Graph:** Create a list of all possible connections (edges) with their costs.
            *   For each pipe `[house1, house2, cost]` in `pipes`, add the edge `(cost, house1, house2)` to the edge list.
            *   For each well cost `wells[i-1]` for house `i`, model this as an edge from a virtual "water source" node (let's say node 0) to house `i`. Add the edge `(wells[i-1], 0, i)` to the edge list.
        *   Now you have a graph with `n+1` nodes and a list of all possible edges.
        *   Sort the entire edge list by cost in ascending order.
        *   Use the standard Kruskal's algorithm: Initialize a DSU, iterate through the sorted edges, and add an edge if it doesn't form a cycle, summing up the costs until `n` edges have been added (to connect all `n+1` nodes). The final sum is the minimum cost.

### 81. All Paths From Source to Target

*   **Problem Description:** Given a directed acyclic graph (DAG) of `n` nodes labeled from `0` to `n-1`, find all possible paths from node `0` to node `n-1` and return them.

*   **Key Signals & Analysis:** The problem asks for **all possible paths**, not just the shortest or a single path. This is a clear indicator for a traversal that explores every possibility, which is a **Depth-First Search (DFS)** or **Backtracking** approach. Since the graph is a DAG, we don't need to worry about infinite cycles, simplifying the `visited` state management.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking / DFS.
    2.  **Tool:** A recursive helper function to build the paths.
    3.  **Approach:**
        *   Build an adjacency list representation of the graph.
        *   Define a recursive function `find_paths(current_node, current_path)`.
        *   **Make a choice:** Add `current_node` to `current_path`.
        *   **Base Case:** If `current_node` is the target node (`n-1`), a complete path has been found. Add a copy of `current_path` to the final results list and return.
        *   **Recurse:** For each `neighbor` of `current_node`:
            *   Call `find_paths(neighbor, current_path)`.
        *   **Undo the choice (Backtrack):** After the loop over neighbors is finished, remove `current_node` from the end of `current_path`. This is crucial for correctly exploring other branches of the search.
        *   Start the process by calling `find_paths(0, [])`.

### 82. Connecting Cities With Minimum Cost

*   **Problem Description:** You are given `n` cities and a list of `connections`, where `connections[i] = [city1, city2, cost]` represents the cost to connect two cities. Return the minimum cost to connect all the `n` cities. If it is impossible to connect all cities, return -1.

*   **Key Signals & Analysis:** This is an even more direct phrasing of the **Minimum Spanning Tree (MST)** problem than problem #80. The goal is to select a subset of edges (connections) that connects all vertices (cities) with the minimum possible total weight (cost).

*   **Strategic Pattern & Approach (Prim's Algorithm):**
    1.  **Pattern:** Minimum Spanning Tree (Prim's Algorithm).
    2.  **Tool:** An adjacency list, a **Min-Priority Queue**, and a `visited` set.
    3.  **Approach:**
        *   Build an adjacency list for the graph.
        *   Initialize a Min-Heap and a `visited` set.
        *   Start with an arbitrary node (e.g., city 1). Add all its edges `(cost, neighbor)` to the Min-Heap. Mark city 1 as visited.
        *   Initialize `total_cost = 0` and `edges_used = 0`.
        *   While the heap is not empty and `edges_used < n - 1`:
            *   Pop the edge with the minimum cost `(cost, city)` from the heap.
            *   If `city` is already visited, continue.
            *   Mark `city` as visited.
            *   `total_cost += cost`.
            *   `edges_used++`.
            *   For each neighbor of the newly visited `city`, add its connecting edge to the heap.
        *   After the loop, if `edges_used == n - 1`, return `total_cost`. Otherwise, the graph was not fully connected, so return -1.

---

### 83. Longest Palindromic Subsequence

*   **Problem Description:** Given a string `s`, find the length of the longest palindromic subsequence of `s`. A subsequence is a sequence that can be derived from another sequence by deleting some or no elements without changing the order of the remaining elements.

*   **Key Signals & Analysis:** This is different from "Longest Palindromic Substring" because the characters do not need to be contiguous. The problem has optimal substructure. The longest palindromic subsequence of a string `s` from index `i` to `j` depends on the solutions for its inner substrings.
    *   If `s[i] == s[j]`, then they form the ends of a palindrome. The length is `2 +` the length of the LPS in the inner string `s[i+1...j-1]`.
    *   If `s[i] != s[j]`, then we can't use both. The answer is the maximum of the LPS of `s[i+1...j]` and `s[i...j-1]`.
    This recurrence relation is a clear signal for **Dynamic Programming**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A 2D DP table, `dp[i][j]`, storing the length of the longest palindromic subsequence in the substring `s[i...j]`.
    3.  **Approach:**
        *   Create a `dp` table of size `n x n`.
        *   **Base Case:** Initialize `dp[i][i] = 1` for all `i` (every single character is a palindrome of length 1).
        *   Iterate through the string with increasing lengths of substrings. `len` from 2 to `n`.
        *   For each `len`, iterate `i` from 0 to `n-len`. The end index is `j = i + len - 1`.
        *   Apply the recurrence:
            *   If `s[i] == s[j]`, `dp[i][j] = 2 + dp[i+1][j-1]`.
            *   If `s[i] != s[j]`, `dp[i][j] = max(dp[i+1][j], dp[i][j-1])`.
        *   The final answer is in `dp[0][n-1]`.

### 84. Maximum Product Subarray

*   **Problem Description:** Given an integer array `nums`, find a contiguous non-empty subarray within the array that has the largest product, and return the product.

*   **Key Signals & Analysis:** This is similar to "Maximum Subarray" (Kadane's Algorithm), but the inclusion of negative numbers creates a twist. A large negative number, when multiplied by another negative number, can become a large positive number. Therefore, we can't just track the maximum product; we also need to track the **minimum product** at each step. The minimum product might be a large negative number that could become the maximum product if we encounter another negative number.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming / Modified Kadane's Algorithm.
    2.  **Tool:** Variables to track the `current_max_product`, `current_min_product`, and an overall `result`.
    3.  **Approach:**
        *   Initialize `current_max`, `current_min`, and `result` to the first element of `nums`.
        *   Iterate through the array from the second element `i = 1`.
        *   For each `num = nums[i]`:
            *   The new `current_max` is the maximum of three values: `num` itself (starting a new subarray), `num * current_max` (extending with a positive), or `num * current_min` (extending with a negative that flips the sign).
            *   Similarly, the new `current_min` is the minimum of `num`, `num * current_max`, or `num * current_min`.
            *   Store the old `current_max` in a temp variable before you update it, as you'll need it for the `current_min` calculation.
            *   Update the overall `result = max(result, current_max)`.
        *   Return `result`.

### 85. Coin Change 2

*   **Problem Description:** You are given an integer array `coins` representing coins of different denominations and an integer `amount`. Return the number of combinations that make up that amount. If that amount of money cannot be made up by any combination of the coins, return 0. You may assume that you have an infinite number of each kind of coin.

*   **Key Signals & Analysis:** The problem asks for the "number of ways" or "number of combinations," not the minimum number of coins. This is a classic **Unbounded Knapsack** style **Dynamic Programming** problem. The state will depend on the amount we are trying to make.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming (Unbounded Knapsack).
    2.  **Tool:** A 1D DP array, `dp`, where `dp[i]` stores the number of ways to make change for amount `i`.
    3.  **Approach:**
        *   Create a `dp` array of size `amount + 1`.
        *   **Initialize:** `dp[0] = 1` (there is exactly one way to make change for amount 0: by using no coins).
        *   Iterate through each `coin` in the `coins` array.
        *   For each `coin`, iterate through the amounts from `j = coin` up to `amount`.
            *   Update `dp[j] = dp[j] + dp[j - coin]`. This means the number of ways to make amount `j` is increased by the number of ways we could make `j - coin` (because we can now add the current `coin` to each of those combinations).
        *   The order of the loops (outer loop for coins, inner for amount) is crucial for counting combinations correctly and not permutations.
        *   The final answer is `dp[amount]`.

### 86. Isomorphic Strings

*   **Problem Description:** Given two strings `s` and `t`, determine if they are isomorphic. Two strings are isomorphic if the characters in `s` can be replaced to get `t`. All occurrences of a character must be replaced with another character while preserving the order. No two characters may map to the same character, but a character may map to itself.

*   **Key Signals & Analysis:** This is a mapping problem. We need to check for a consistent one-to-one mapping between characters from `s` to `t`. A character `s[i]` must always map to the same `t[i]`. Additionally, no other character `s[j]` can map to that same `t[i]`. This requires two-way validation.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Hash Map Traversal.
    2.  **Tool:** Two hash maps (or arrays of size 256 for ASCII) to store the mappings.
    3.  **Approach:**
        *   Create two maps: `s_to_t_map` and `t_to_s_map`.
        *   Iterate from `i = 0` to `s.length() - 1`.
        *   Let `char_s = s[i]` and `char_t = t[i]`.
        *   **Check Forward Mapping:**
            *   If `char_s` is in `s_to_t_map`, check if its mapping is equal to `char_t`. If not, return `false`.
        *   **Check Backward Mapping:**
            *   If `char_t` is in `t_to_s_map`, check if its mapping is equal to `char_s`. If not, return `false`.
        *   **Establish New Mapping:** If the checks pass and the characters are new, establish the mapping in both directions: `s_to_t_map[char_s] = char_t` and `t_to_s_map[char_t] = char_s`.
        *   If the loop completes, the strings are isomorphic. Return `true`.

### 87. Time Based Key-Value Store

*   **Problem Description:** Design a time-based key-value data structure that can store multiple values for the same key at different time stamps and retrieve the key's value at a certain timestamp. Implement the `TimeMap` class with `set(key, value, timestamp)` and `get(key, timestamp)`. The `get` function should return the value associated with the largest `timestamp_prev` <= `timestamp`.

*   **Key Signals & Analysis:** The `set` operation is straightforward. The challenge is the `get` operation. For each key, we have a list of values sorted by timestamp. We need to find the "floor" value for a given timestamp. Searching for a value in a sorted list efficiently is a classic use case for **Binary Search**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Hash Map combined with Binary Search.
    2.  **Tool:** A hash map where the key is the string `key`, and the value is a list/vector of pairs `(timestamp, value)`.
    3.  **Approach:**
        *   **`set(key, value, timestamp)`:**
            *   If the `key` is not in the map, create a new list for it.
            *   Append the `(timestamp, value)` pair to the list associated with the `key`. Since timestamps are strictly increasing, this list will remain sorted by timestamp.
        *   **`get(key, timestamp)`:**
            *   If the `key` is not in the map, return `""`.
            *   Get the list of `(ts, val)` pairs.
            *   Perform a **Binary Search** on this list to find the largest timestamp that is less than or equal to the given `timestamp`.
            *   The binary search will find an index. Return the value at that index. If the search finds no valid timestamp (e.g., all timestamps in the list are greater than the target), return `""`.

---

### 88. Kth Largest Element in an Array

*   **Problem Description:** Given an integer array `nums` and an integer `k`, return the `k`-th largest element in the array. Note that it is the k-th largest element in the sorted order, not the k-th distinct element.

*   **Key Signals & Analysis:** A naive solution is to sort the entire array (O(N log N)) and pick the element at index `n-k`. This is often too slow. The problem asks for the "k-th largest," which is a strong signal for a selection algorithm. Two main patterns apply:
    1.  **Top 'K' Elements:** Using a Min-Heap of size `k` is a very common and easy-to-implement solution.
    2.  **Quickselect:** A more advanced, in-place algorithm that is a modification of Quicksort and has an average-case time complexity of O(N).

*   **Strategic Pattern & Approach (Heap):**
    1.  **Pattern:** Top 'K' Elements.
    2.  **Tool:** A **Min-Heap** of size `k`.
    3.  **Approach:**
        *   Initialize a Min-Heap.
        *   Iterate through the `nums` array.
        *   For each `num`:
            *   Push `num` into the heap.
            *   If the heap's size is greater than `k`, pop the smallest element (the heap's root).
        *   After the loop, the heap contains the `k` largest elements from the array. The root of the Min-Heap is the `k`-th largest element. Return it. This is an O(N log K) solution.

### 89. Construct Binary Tree from Preorder and Inorder Traversal

*   **Problem Description:** Given two integer arrays, `preorder` and `inorder`, where `preorder` is the preorder traversal of a binary tree and `inorder` is the inorder traversal of the same tree, construct and return the binary tree.

*   **Key Signals & Analysis:** This is a classic tree construction problem that relies on the fundamental properties of traversals.
    *   The **first element of the `preorder` traversal is always the root** of the current (sub)tree.
    *   In the **`inorder` traversal, all elements to the left of the root form the left subtree**, and all elements to the right form the right subtree.
    This structure naturally lends itself to a **recursive (DFS)** solution.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Recursive Tree Building (DFS).
    2.  **Tool:** A recursive helper function. A hash map to store the indices of elements in the `inorder` array can significantly speed up the lookup process.
    3.  **Approach:**
        *   Create a hash map `inorder_map` from value to index for the `inorder` array.
        *   Define a recursive function `build(pre_start, pre_end, in_start, in_end)`.
        *   **Base Case:** If `pre_start > pre_end` or `in_start > in_end`, return `NULL`.
        *   **Find Root:** The root of the current subtree is `preorder[pre_start]`. Create a `TreeNode` for it.
        *   **Find Split Point:** Find the index of this root value in the `inorder` array using the `inorder_map`. Let this be `in_root_idx`.
        *   **Calculate Left Subtree Size:** The number of nodes in the left subtree is `num_left = in_root_idx - in_start`.
        *   **Recurse:**
            *   Recursively build the left subtree: `root.left = build(pre_start + 1, pre_start + num_left, in_start, in_root_idx - 1)`.
            *   Recursively build the right subtree: `root.right = build(pre_start + num_left + 1, pre_end, in_root_idx + 1, in_end)`.
        *   Return the created `root`.

### 90. Validate Sudoku

*   **Problem Description:** Determine if a `9 x 9` Sudoku board is valid. Only the filled cells need to be validated according to the following rules: each row must contain the digits 1-9 without repetition; each column must contain the digits 1-9 without repetition; each of the nine `3 x 3` sub-boxes must contain the digits 1-9 without repetition.

*   **Key Signals & Analysis:** This is a constraint validation problem. We need to check for duplicates across three different types of groups (rows, columns, and boxes). A straightforward simulation of the rules is the best approach. The key to an efficient solution is using a data structure that can detect duplicates in O(1) time.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Single Pass with Hashing.
    2.  **Tool:** Three sets of hash sets (or boolean arrays) to track seen numbers. For example, `rows[9][9]`, `cols[9][9]`, `boxes[9][9]`.
    3.  **Approach:**
        *   Initialize the three 2D boolean arrays (or arrays of hash sets).
        *   Iterate through the board from `row = 0` to `8` and `col = 0` to `8`.
        *   For each cell containing a number `num`:
            *   Calculate the index of the 3x3 box it belongs to: `box_index = (row / 3) * 3 + (col / 3)`.
            *   **Check for Duplicates:**
                *   Check if `rows[row][num-1]` is already `true`. If so, return `false`.
                *   Check if `cols[col][num-1]` is already `true`. If so, return `false`.
                *   Check if `boxes[box_index][num-1]` is already `true`. If so, return `false`.
            *   **Mark as Seen:** If no duplicates are found, set all three corresponding entries to `true`.
        *   If the entire loop completes, the board is valid. Return `true`.

### 91. Koko Eating Bananas

*   **Problem Description:** Koko loves to eat bananas. There are `n` piles of bananas, the `i`-th pile has `piles[i]` bananas. The guards have gone and will come back in `h` hours. Koko can decide her bananas-per-hour eating speed of `k`. Each hour, she chooses some pile and eats `k` bananas. If the pile has less than `k` bananas, she eats all of them and won't eat any more during that hour. Find the minimum integer `k` such that she can eat all the bananas within `h` hours.

*   **Key Signals & Analysis:** This is a search problem, but the search space is the *eating speed `k`*, not the array of piles. The problem asks for the **minimum** `k` that works. The range of possible speeds is from 1 to the maximum number of bananas in any single pile. Let's say we test a speed `k`. We can easily calculate the total hours required: `sum(ceil(piles[i] / k))` for all `i`.
    *   If the time taken is `> h`, our speed `k` is too slow. We need to try a larger `k`.
    *   If the time taken is `<= h`, our speed `k` is a possible answer, but maybe an even smaller speed could also work. We should try a smaller `k`.
    This monotonic property (if speed `k` works, any speed `> k` also works) is a perfect signal for **Binary Search on the Answer**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Binary Search on the Answer.
    2.  **Tool:** A standard binary search framework.
    3.  **Approach:**
        *   Define the search space for `k`: `left = 1`, `right = max(piles)`.
        *   Initialize `result = right`.
        *   While `left <= right`:
            *   Calculate `mid = left + (right - left) / 2`.
            *   Check if this speed `mid` is feasible: calculate `hours_needed` by summing `ceil(piles[i] / mid)` for all piles.
            *   If `hours_needed <= h`:
                *   `mid` is a possible answer. Store it: `result = mid`.
                *   Try for an even smaller speed: `right = mid - 1`.
            *   Else (`hours_needed > h`):
                *   `mid` is too slow. We need a larger speed: `left = mid + 1`.
        *   Return `result`.

### 92. Daily Temperatures

*   **Problem Description:** Given an array of integers `temperatures` representing the daily temperatures, return an array `answer` such that `answer[i]` is the number of days you have to wait after the `i`-th day to get a warmer temperature. If there is no future day for which this is possible, keep `answer[i] == 0`.

*   **Key Signals & Analysis:** For each day, we need to find the **next greater element**. This is the exact problem definition that the **Monotonic Stack** pattern is designed to solve with optimal O(N) time complexity.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Monotonic Stack.
    2.  **Tool:** A stack to store the *indices* of the days.
    3.  **Approach:**
        *   Initialize a `result` array with all 0s and an empty stack.
        *   The stack will store indices of days in decreasing order of temperature.
        *   Iterate through the `temperatures` array with index `i`.
        *   While the stack is not empty and the current temperature `temperatures[i]` is greater than the temperature at the index on top of the stack (`temperatures[stack.top()]`):
            *   The current day `i` is the answer for the day at the top of the stack.
            *   Pop the index `prev_day_idx` from the stack.
            *   Set `result[prev_day_idx] = i - prev_day_idx`.
        *   Push the current index `i` onto the stack.
        *   Return the `result` array.

Of course. Here is a final advanced set of competitive programming problems. These challenges test a deep understanding of core patterns and often require combining them or applying them to complex state representations.

---

### 93. Word Break II

*   **Problem Description:** Given a string `s` and a dictionary of strings `wordDict`, add spaces in `s` to construct a sentence where each word is a valid dictionary word. Return all such possible sentences.

*   **Key Signals & Analysis:** This is a step up from "Word Break I". While the original problem just asked *if* a break was possible (a boolean DP problem), this one asks for **all possible segmentations**. "All possible" is a strong signal for **Backtracking**. However, a naive backtracking approach will be too slow because it will repeatedly re-solve the same subproblems (e.g., trying to segment the suffix `"apple"` multiple times). The optimal solution combines the strengths of both patterns: use **Dynamic Programming (Memoization)** to remember which substrings can be broken down, and then use **Backtracking** to construct the actual sentences from this knowledge.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Backtracking with Memoization (a form of DP).
    2.  **Tool:** A recursive helper function and a hash map to memoize the results for suffixes.
    3.  **Approach:**
        *   Define a recursive function `find_breaks(substring)`. The function should return a list of all valid sentences that can be formed from `substring`.
        *   **Memoization:** Use a map `memo` to store the results for substrings that have already been computed. If `substring` is in `memo`, return the stored result immediately.
        *   **Recursive Step:**
            *   Iterate from `i = 1` to `len(substring)`.
            *   Consider the `prefix = substring[0...i-1]`.
            *   If `prefix` is in the `wordDict`:
                *   Get the `suffix = substring[i...]`.
                *   Recursively call `suffix_results = find_breaks(suffix)`.
                *   For each `sentence` in `suffix_results`, prepend the `prefix` to form a new valid sentence (`prefix + " " + sentence`) and add it to the current list of results.
        *   **Base Case (Implicit):** If the `substring` itself is a valid word, add it to the results list (this handles the end of a sentence).
        *   Store the computed results for the current `substring` in the `memo` map before returning.

### 94. Minimum Knight Moves

*   **Problem Description:** In an infinite chessboard, a knight starts at `[0, 0]`. Given a target coordinate `[x, y]`, return the minimum number of moves to reach the target.

*   **Key Signals & Analysis:** This is a classic **shortest path** problem on an infinite, unweighted grid. The "minimum number of moves" is the key phrase that signals **Breadth-First Search (BFS)**. Each square on the board is a node, and a valid knight move is an edge. BFS guarantees finding the shortest path in terms of the number of moves (edges).

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Breadth-First Search (BFS).
    2.  **Tool:** A queue to store `(x, y, distance)` tuples and a `visited` set to avoid cycles and redundant computations.
    3.  **Approach:**
        *   The problem has symmetry. The target `(x, y)` can be treated as `(|x|, |y|)` to reduce the search space to the first quadrant.
        *   Initialize a queue with `(0, 0, 0)`.
        *   Initialize a `visited` set and add the starting coordinate `(0,0)`.
        *   While the queue is not empty:
            *   Dequeue `(curr_x, curr_y, dist)`.
            *   If `(curr_x, curr_y)` is the target, return `dist`.
            *   Generate all 8 possible knight moves from the current position.
            *   For each `(next_x, next_y)`:
                *   If this new position has not been visited:
                    *   Add it to the `visited` set.
                    *   Enqueue `(next_x, next_y, dist + 1)`.

### 95. Number of Provinces

*   **Problem Description:** There are `n` cities. Some of them are connected, while some are not. If city `a` is connected directly with city `b`, and city `b` is connected directly with city `c`, then city `a` is connected indirectly with city `c`. A province is a group of directly or indirectly connected cities and no other cities outside of the group. You are given an `n x n` matrix `isConnected` where `isConnected[i][j] = 1` if the `i`-th city and the `j`-th city are directly connected. Return the total number of provinces.

*   **Key Signals & Analysis:** This is a more formal name for the "Number of Connected Components" or "Number of Islands" problem. The input is an adjacency matrix representation of the graph. The goal is to count the number of distinct, disconnected groups of cities. This can be solved with either traversal or Union-Find.

*   **Strategic Pattern & Approach (Union-Find):**
    1.  **Pattern:** Union-Find (DSU).
    2.  **Tool:** A DSU data structure.
    3.  **Approach:**
        *   Initialize a DSU with `n` cities, where each city is its own component. The initial province count is `n`.
        *   Iterate through the `isConnected` matrix.
        *   If `isConnected[i][j] == 1`:
            *   Perform a `union(i, j)` operation. The DSU will handle merging the components and will only decrement its internal count if `i` and `j` were previously in different components.
        *   After iterating through the entire matrix, the final count of components in the DSU is the number of provinces. Return this count.

### 96. Meeting Scheduler

*   **Problem Description:** You are given two schedules of `slots1` and `slots2`, where `slots1[i] = [start, end]` represents the available time slot for a person. You are also given an integer `duration`. Return the earliest time slot that works for both people and is of duration `duration`. If there is no such slot, return an empty array.

*   **Key Signals & Analysis:** This is an interval intersection problem. We need to find a common available block of time that is long enough. The "earliest" requirement suggests we should process the slots in chronological order. This can be efficiently solved by sorting both lists of slots and using a two-pointer approach to find the first valid intersection.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Two Pointers on Sorted Intervals.
    2.  **Tool:** Two pointers, one for each schedule.
    3.  **Approach:**
        *   Sort both `slots1` and `slots2` by their start times.
        *   Initialize two pointers, `p1 = 0` for `slots1` and `p2 = 0` for `slots2`.
        *   While `p1 < len(slots1)` and `p2 < len(slots2)`:
            *   Get the current intervals: `interval1 = slots1[p1]` and `interval2 = slots2[p2]`.
            *   Find the intersection of these two intervals:
                *   `intersection_start = max(interval1.start, interval2.start)`.
                *   `intersection_end = min(interval1.end, interval2.end)`.
            *   **Check for Validity:** If `intersection_end - intersection_start >= duration`, we have found the earliest valid slot. Return `[intersection_start, intersection_start + duration]`.
            *   **Advance Pointer:** We need to greedily advance one of the pointers. It's always optimal to advance the pointer corresponding to the interval that **ends earlier**, as that interval cannot possibly overlap with any future intervals of the other person.
            *   If `interval1.end < interval2.end`, increment `p1`.
            *   Else, increment `p2`.
        *   If the loop finishes, no common slot was found. Return an empty array.

### 97. Reorganize String

*   **Problem Description:** Given a string `s`, rearrange the characters of `s` so that any two adjacent characters are not the same. If possible, return any such string. If it's not possible, return `""`.

*   **Key Signals & Analysis:** This is a scheduling/arrangement problem with constraints. To ensure no two adjacent characters are the same, the best strategy is to always place the most frequent available character, followed by the second most frequent, and so on. This greedy approach prevents the most frequent character from being "cornered" with no other characters left to place next to it. "Most frequent" is a strong signal for using a heap.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Greedy with a Heap.
    2.  **Tool:** A **Max-Heap** to store the character frequencies.
    3.  **Approach:**
        *   Count the frequency of each character. A quick check: if any character's frequency is `> (n+1)/2`, a solution is impossible, so return `""`.
        *   Push `(frequency, character)` pairs into a Max-Heap.
        *   Initialize a result string builder.
        *   While the heap has more than one element:
            *   Pop the two most frequent characters, `(freq1, char1)` and `(freq2, char2)`.
            *   Append `char1` and then `char2` to the result.
            *   Decrement their frequencies. If a character's new frequency is still greater than 0, push it back into the heap.
        *   **Handle Last Character:** If the heap is not empty after the loop, it will contain one last character. Append it to the result.
        *   Return the final string.

---

### 98. Text Justification

*   **Problem Description:** Given an array of strings `words` and a width `maxWidth`, format the text such that each line has exactly `maxWidth` characters and is fully (left and right) justified. You should pack your words in a greedy approach; that is, pack as many words as you can in each line. Pad extra spaces `' '` when necessary so that each line has exactly `maxWidth` characters. For the last line of text, it should be left-justified and no extra space is inserted between words.

*   **Key Signals & Analysis:** This is a pure **simulation and implementation** problem. There is no clever algorithmic pattern like DP or graphs that will solve it directly. Its complexity comes from meticulous bookkeeping, handling numerous edge cases, and careful string manipulation. The logic must be broken down into distinct phases: identifying which words fit on a line, and then a separate, complex phase for calculating and applying the correct spacing for that line.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Simulation with careful Greedy Word Grouping.
    2.  **Tool:** Precise control over loops, indices, and string building.
    3.  **Approach:**
        *   The main loop should group words line by line. Use a pointer `i` to track the first word of the potential next line.
        *   **Grouping Phase:**
            *   From `i`, use an inner loop with pointer `j` to greedily find how many words (`j - i`) can fit on the current line. Keep track of the `current_line_length`, including single spaces between words. Stop when adding the next word `words[j]` would exceed `maxWidth`.
        *   **Justification Phase (for the words from `i` to `j-1`):**
            *   Calculate `num_words = j - i` and `total_char_length`.
            *   Calculate `total_spaces_needed = maxWidth - total_char_length`.
            *   **Handle Edge Cases:**
                *   **Last Line:** If `j` has reached the end of the `words` array, this is the last line. It must be left-justified. Append each word with a single space, and then pad the rest of the line with spaces at the end.
                *   **Single Word Line:** If `num_words == 1`, it must also be left-justified. Append the word and pad the rest with spaces.
            *   **Standard Justification:**
                *   Calculate the number of "gaps" between words: `num_gaps = num_words - 1`.
                *   Calculate the base number of spaces for each gap: `spaces_per_gap = total_spaces_needed / num_gaps`.
                *   Calculate the number of extra spaces that need to be distributed from the left: `extra_spaces = total_spaces_needed % num_gaps`.
                *   Build the line string. For the first `extra_spaces` gaps, apply `spaces_per_gap + 1` spaces. For the remaining gaps, apply `spaces_per_gap` spaces.
        *   After processing a line, update the main pointer `i = j` to start the next line's grouping phase.

### 99. Trapping Rain Water II

*   **Problem Description:** Given an `m x n` integer matrix `heightMap` representing the height of each unit cell in a 2D elevation map, compute the volume of water it can trap after raining.

*   **Key Signals & Analysis:** This is the 3D version of the "Trapping Rain Water" problem and is significantly more complex. The water level is no longer determined by simple left and right boundaries but by the "wall" of cells surrounding a basin. Water can be trapped inside if the height of the inner cells is lower than the surrounding wall. The key insight is that water is contained by the lowest point on the entire border of the map. This suggests a search algorithm that always processes the lowest boundary cell first. This is a perfect application of **Dijkstra's algorithm or a modified BFS using a Priority Queue**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dijkstra-like Search from the Borders.
    2.  **Tool:** A **Min-Priority Queue (Min-Heap)** and a `visited` 2D array.
    3.  **Approach:**
        *   The heap will store `(height, row, col)` tuples, ordered by height.
        *   **Initialization:** Add all cells from the border of the `heightMap` to the Min-Heap. Mark them as `visited`. These cells form the initial "wall" and cannot trap water themselves.
        *   Initialize `total_water = 0`.
        *   **Dijkstra-like Traversal:**
            *   While the heap is not empty:
                *   Pop the cell with the minimum height from the heap, let's call it `(h, r, c)`. This is the current lowest point on our "dam."
                *   For each of the four `neighbor` cells of `(r, c)`:
                    *   If the `neighbor` is within bounds and has not been visited:
                        *   Mark the `neighbor` as `visited`.
                        *   The water trapped at the `neighbor` is determined by the height of the current dam. Water trapped is `max(0, h - neighbor_height)`. Add this to `total_water`.
                        *   Push the `neighbor` onto the heap, but its effective height for future damming purposes is `max(h, neighbor_height)`. This is because the water level can't be lower than the neighbor itself.
        *   The `total_water` is the final answer.

### 100. Regular Expression Matching

*   **Problem Description:** Given an input string `s` and a pattern `p`, implement regular expression matching with support for `.` (matches any single character) and `*` (matches zero or more of the preceding element). The matching should cover the entire input string (not partial).

*   **Key Signals & Analysis:** This is a notoriously difficult problem that tests deep understanding of recursion and state transitions. The state of our match depends on our current position in both the string `s` and the pattern `p`. The `*` character is what makes this complex, as it can represent multiple possibilities (matching zero, one, or more characters). This branching of possibilities and reliance on solving subproblems (`does the rest of the string match the rest of the pattern?`) is a clear signal for **Dynamic Programming** or **Recursion with Memoization**.

*   **Strategic Pattern & Approach:**
    1.  **Pattern:** Dynamic Programming.
    2.  **Tool:** A 2D boolean DP table, `dp[i][j]`, which will be `true` if the first `i` characters of `s` can be matched by the first `j` characters of `p`.
    3.  **Approach:**
        *   Create the `dp` table of size `(len(s)+1) x (len(p)+1)`. `dp[0][0]` is `true` (empty string matches empty pattern).
        *   **Initialize First Row (for `*`):** Handle patterns like `a*` or `a*b*` which can match an empty string. `dp[0][j]` is `true` if `p[j-1]` is `*` and `dp[0][j-2]` is `true`.
        *   Iterate `i` from 1 to `len(s)` and `j` from 1 to `len(p)`.
        *   **Main Logic for `dp[i][j]`:**
            *   **Case 1: `p[j-1]` is a normal character or `.`**
                *   If `p[j-1]` matches `s[i-1]` (or `p[j-1]` is `.`), then `dp[i][j]` is `true` if and only if the substring before it also matched: `dp[i][j] = dp[i-1][j-1]`.
            *   **Case 2: `p[j-1]` is `*`**
                *   The `*` and its preceding character `p[j-2]` can be treated in two ways:
                *   **Subcase 2a: The `*` matches zero elements.** In this case, we effectively ignore the `p[j-2]*` part of the pattern. The result depends on whether the string `s` up to `i` can be matched by the pattern `p` up to `j-2`. So, `dp[i][j] = dp[i][j-2]`.
                *   **Subcase 2b: The `*` matches one or more elements.** This is only possible if the current character `s[i-1]` matches the character before the `*`, i.e., `p[j-2]`. If they match, then the result depends on whether the prefix of the string `s` (without its last character) could be matched by the current pattern `p` up to `j`. So, we check `dp[i-1][j]`.
                *   `dp[i][j]` is `true` if either of these subcases is `true`.
        *   The final answer is `dp[len(s)][len(p)]`.