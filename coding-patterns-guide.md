# Mastering Coding Patterns: A Systematic Approach
## Complete Decision Framework for Problem-Solving

**Author's Note**: This guide is designed to transform you from someone who solves problems one-by-one into a pattern-recognition expert who can identify and apply optimal solutions quickly.

---

## TABLE OF CONTENTS

### PART 1: FOUNDATION
1. Understanding Pattern Recognition
2. The Pattern Selection Framework
3. Input Analysis Methodology

### PART 2: DETAILED PATTERNS
4. Two Pointers Pattern
5. Sliding Window Pattern
6. Fast and Slow Pointers
7. Merge Intervals Pattern
8. Top K Elements Pattern
9. Breadth-First Search (BFS)
10. Depth-First Search (DFS)
11. Backtracking Pattern
12. Monotonic Stack Pattern
13. Union Find Pattern

### PART 3: PRACTICAL APPLICATION
14. Pattern Combination Strategies
15. Complete Decision Framework
16. Practice Roadmap

---

# PART 1: FOUNDATION

## Section 1: Understanding Pattern Recognition

### 1.1 What Are Coding Patterns?

Coding patterns are **reusable problem-solving templates** that represent common algorithmic approaches.

**Analogy**: Think of patterns like cooking techniques:
- **Two Pointers** = Folding ingredients from both ends
- **Sliding Window** = Rolling dough in one direction
- **BFS** = Layering a cake (level by level)
- **DFS** = Digging deep into flavors (explore fully before moving on)

### 1.2 The Pattern Recognition Skill Development

```
Stage 1: NOVICE (0-20 problems)
├─ Can't identify patterns without hints
├─ Implementation requires constant reference
└─ Struggles with variations

Stage 2: BEGINNER (20-50 problems)
├─ Recognizes 3-4 basic patterns
├─ Can implement with template
└─ Handles straightforward problems

Stage 3: INTERMEDIATE (50-150 problems)
├─ Quickly identifies primary pattern
├─ Adapts templates independently
└─ Combines multiple patterns

Stage 4: ADVANCED (150+ problems)
├─ Instant pattern recognition
├─ Creates hybrid approaches
└─ Optimizes beyond templates
```

### 1.3 The Core Problem-Solving Framework

```
STEP 1: READ & UNDERSTAND
├─ What is the input format?
├─ What is the expected output?
├─ What are the constraints?
└─ What are edge cases?

STEP 2: IDENTIFY CUES
├─ Data structure type?
├─ Sorted or unsorted?
├─ Finding optimal/all solutions?
├─ Constraints on time/space?
└─ Keywords in problem statement?

STEP 3: PATTERN MATCHING
├─ List 2-3 candidate patterns
├─ Evaluate complexity for each
├─ Choose best fit
└─ Consider hybrid approaches

STEP 4: IMPLEMENT
├─ Start with pattern template
├─ Adapt to specific problem
├─ Handle edge cases
└─ Test thoroughly
```

---

## Section 2: The Pattern Selection Framework

### 2.1 The Master Decision Tree

```
START: Analyze Problem
    |
    ├─ Is input SORTED?
    |   ├─ YES → Consider: Two Pointers (90%)
    |   └─ NO → Continue analysis
    |
    ├─ Looking for SUBARRAY/SUBSTRING?
    |   ├─ Contiguous? → Sliding Window
    |   └─ Non-contiguous? → Dynamic Programming
    |
    ├─ Data Structure Type?
    |   ├─ Array/String → Two Pointers, Sliding Window, Monotonic Stack
    |   ├─ Linked List → Fast & Slow Pointers
    |   ├─ Tree → BFS, DFS
    |   ├─ Graph → BFS, DFS, Union Find
    |   └─ Intervals → Merge Intervals
    |
    ├─ Finding K-th element or Top K?
    |   └─ YES → Top K Elements (Heap)
    |
    ├─ Need ALL solutions?
    |   └─ YES → Backtracking, DFS
    |
    ├─ Cycle detection needed?
    |   └─ YES → Fast & Slow Pointers, DFS with visited
    |
    └─ Connectivity/Grouping problem?
        └─ YES → Union Find, BFS, DFS
```

### 2.2 Pattern Recognition Cues

#### Quick Reference Table

| Cue | Pattern | Confidence |
|-----|---------|------------|
| **"sorted array" + "pair/triplet"** | Two Pointers | 95% |
| **"subarray with sum/length"** | Sliding Window | 90% |
| **"middle of linked list"** | Fast & Slow Pointers | 99% |
| **"cycle in linked list"** | Fast & Slow Pointers | 99% |
| **"merge intervals/ranges"** | Merge Intervals | 95% |
| **"k largest/smallest"** | Top K Elements | 90% |
| **"level order traversal"** | BFS | 99% |
| **"all paths/combinations"** | DFS/Backtracking | 85% |
| **"next greater element"** | Monotonic Stack | 90% |
| **"connected components"** | Union Find/DFS | 80% |

### 2.3 Keyword Analysis

**Keywords that signal specific patterns:**

**Two Pointers Keywords:**
- "sorted array"
- "pair with sum"
- "remove duplicates"
- "reverse"
- "partition"

**Sliding Window Keywords:**
- "subarray"
- "substring"
- "consecutive elements"
- "window size"
- "maximum/minimum sum of size k"

**Fast & Slow Pointers Keywords:**
- "middle element"
- "cycle detection"
- "nth node from end"
- "linked list"

**Intervals Keywords:**
- "overlapping"
- "merge"
- "intervals"
- "meeting rooms"
- "schedule"

**Top K Keywords:**
- "k largest"
- "k smallest"
- "k most frequent"
- "k closest"
- "top k"

**BFS Keywords:**
- "level order"
- "shortest path"
- "minimum steps"
- "layer by layer"

**DFS Keywords:**
- "all paths"
- "all combinations"
- "tree traversal"
- "explore fully"

**Backtracking Keywords:**
- "all possible"
- "generate all"
- "combinations"
- "permutations"
- "subsets"

**Monotonic Stack Keywords:**
- "next greater"
- "next smaller"
- "previous greater"
- "histogram"
- "temperature"

**Union Find Keywords:**
- "connected components"
- "redundant connection"
- "number of islands"
- "network"
- "clusters"

---

## Section 3: Input Analysis Methodology

### 3.1 Analyze Input Type

**Step 1: Identify Primary Data Structure**

```c
// Ask yourself:
int inputAnalysis() {
    // What am I given?
    
    // Single Array?
    int arr[] = {1, 2, 3, 4, 5};
    // → Consider: Two Pointers, Sliding Window, Monotonic Stack
    
    // Linked List?
    struct ListNode* head;
    // → Consider: Fast & Slow Pointers, Two Pointers
    
    // Tree?
    struct TreeNode* root;
    // → Consider: BFS, DFS
    
    // Graph (adjacency list/matrix)?
    int** graph;
    // → Consider: BFS, DFS, Union Find
    
    // Array of Intervals?
    int intervals[][2];
    // → Consider: Merge Intervals
    
    // Multiple Arrays?
    // → Consider: Two Pointers (merge), Multiple patterns
    
    return 0;
}
```

### 3.2 Analyze Input Properties

**Critical Questions Checklist:**

```
□ Is the input SORTED?
  ├─ YES → Two Pointers highly likely
  └─ NO → Other patterns possible

□ Can the input be MODIFIED?
  ├─ YES → In-place algorithms possible
  └─ NO → Need auxiliary space

□ What is the SIZE CONSTRAINT?
  ├─ Small (n ≤ 20) → Backtracking, Brute Force okay
  ├─ Medium (n ≤ 10^5) → O(n log n) or O(n) required
  └─ Large (n > 10^6) → Must be O(n) or O(log n)

□ Are there DUPLICATES?
  ├─ YES → Need to handle carefully
  └─ NO → Simpler logic

□ What's the VALUE RANGE?
  ├─ Small range → Array counting possible
  └─ Large range → Need other approaches
```

### 3.3 Analyze Output Requirements

**What does the problem ask for?**

```
OUTPUT TYPE                  → PATTERN HINT
─────────────────────────────────────────────
Single value (min/max/count) → Optimization pattern
All solutions                → Backtracking, DFS
k elements                   → Top K Elements
Boolean (exists/valid)       → Search pattern
Modified input               → In-place algorithms
Grouped elements             → Union Find
```

### 3.4 Constraint Analysis for Complexity

**Time Complexity Hints from Constraints:**

```c
// Given n = input size, what complexity can we afford?

// n ≤ 10
// → O(n!) is acceptable → Backtracking all permutations

// n ≤ 20
// → O(2^n) is acceptable → Backtracking subsets/combinations

// n ≤ 100
// → O(n^3) is acceptable → Triple nested loops

// n ≤ 1,000
// → O(n^2) is acceptable → Double nested loops

// n ≤ 10,000
// → O(n^2) might work → Optimized double loops

// n ≤ 100,000
// → O(n log n) required → Sorting, Heap, Divide & Conquer

// n ≤ 1,000,000
// → O(n) or O(n log n) required → Linear scan, efficient patterns

// n ≤ 10,000,000
// → O(n) required → Must be single/double pass
```

**Example Analysis:**

```
Problem: "Find pair in array with sum equal to target"
Input: nums = [2, 7, 11, 15], target = 9

ANALYSIS:
├─ Input Type: Array of integers
├─ Input Property: NOT sorted initially
├─ Size: n can be up to 10^4 (medium)
├─ Output: Pair of indices or values
└─ Constraint: O(n^2) might be acceptable, but O(n) is better

PATTERN DECISION:
├─ If sorted: Two Pointers (O(n))
├─ If unsorted: Hash Table (O(n)) OR Sort + Two Pointers (O(n log n))
└─ Chosen: Hash Table for O(n) single pass
```

---

# PART 2: DETAILED PATTERNS

## Section 4: Two Pointers Pattern

### 4.1 Pattern Overview

**Core Concept**: Use two pointers to iterate through data structure, typically to reduce time complexity from O(n²) to O(n).

**When to Use - STRONG Signals:**
- ✅ Input is **SORTED** (95% confidence)
- ✅ Looking for **PAIR/TRIPLET** with certain sum
- ✅ Need to **REMOVE/MOVE** elements in-place
- ✅ **COMPARING** elements from both ends
- ✅ **PARTITIONING** array based on condition

**When to Use - WEAK Signals:**
- ⚠️ Unsorted array (might need to sort first)
- ⚠️ Need to maintain original order
- ⚠️ Multiple passes required

**Visual Representation:**
```
Opposite Direction (Classic):
[1, 2, 3, 4, 5, 6, 7, 8]
 ↑                     ↑
left                 right
 
Same Direction (Fast-Slow):
[1, 2, 3, 4, 5, 6, 7, 8]
 ↑  ↑
slow fast
```

### 4.2 Pattern Variants

#### Variant 1: Opposite Direction Pointers

**Use Case**: Sorted array, finding pairs, reversing

```c
/**
 * Template: Opposite Direction Two Pointers
 * Time: O(n), Space: O(1)
 */
void oppositeTwoPointers(int* arr, int size) {
    int left = 0;
    int right = size - 1;
    
    while (left < right) {
        // Process current pair
        int sum = arr[left] + arr[right];
        
        // Decision logic
        if (sum == target) {
            // Found solution
            // Process and move both
            left++;
            right--;
        } else if (sum < target) {
            // Need larger sum
            left++;
        } else {
            // Need smaller sum
            right--;
        }
    }
}
```

#### Variant 2: Same Direction Pointers (Slow-Fast)

**Use Case**: Remove duplicates, partition array, in-place modifications

```c
/**
 * Template: Same Direction Two Pointers
 * Time: O(n), Space: O(1)
 */
int sameDirectionTwoPointers(int* arr, int size) {
    int slow = 0;  // Write position
    int fast = 0;  // Read position
    
    while (fast < size) {
        // Check condition
        if (shouldKeep(arr[fast])) {
            // Keep element
            arr[slow] = arr[fast];
            slow++;
        }
        fast++;
    }
    
    return slow;  // New size
}
```

### 4.3 Implementation Examples

#### Example 1: Two Sum in Sorted Array

**Problem**: Given sorted array, find two numbers that add up to target.

```c
#include <stdio.h>
#include <stdlib.h>

/**
 * Two Sum in Sorted Array
 * 
 * Input: nums = [1, 2, 3, 4, 6], target = 6
 * Output: [1, 3] (indices where nums[1]=2, nums[3]=4, and 2+4=6)
 * 
 * Time: O(n), Space: O(1)
 */
int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    int* result = (int*)malloc(2 * sizeof(int));
    *returnSize = 2;
    
    int left = 0;
    int right = numbersSize - 1;
    
    while (left < right) {
        int sum = numbers[left] + numbers[right];
        
        if (sum == target) {
            result[0] = left;
            result[1] = right;
            return result;
        } else if (sum < target) {
            left++;   // Need larger sum
        } else {
            right--;  // Need smaller sum
        }
    }
    
    // No solution found
    *returnSize = 0;
    return result;
}

// Test function
void testTwoSum() {
    int nums[] = {1, 2, 3, 4, 6};
    int target = 6;
    int returnSize;
    
    int* result = twoSum(nums, 5, target, &returnSize);
    
    if (returnSize == 2) {
        printf("Found pair at indices: %d and %d\n", result[0], result[1]);
        printf("Values: %d + %d = %d\n", 
               nums[result[0]], nums[result[1]], target);
    } else {
        printf("No pair found\n");
    }
    
    free(result);
}
```

**Why Two Pointers?**
- Input is **SORTED** → Strong signal
- Looking for a **PAIR** → Two Pointers classic use case
- Need O(n) solution → Two Pointers achieves this

#### Example 2: Remove Duplicates from Sorted Array

**Problem**: Remove duplicates in-place, return new length.

```c
/**
 * Remove Duplicates from Sorted Array
 * 
 * Input: nums = [1, 1, 2, 2, 3, 4, 4]
 * Output: 4 (modified array: [1, 2, 3, 4, _, _, _])
 * 
 * Time: O(n), Space: O(1)
 */
int removeDuplicates(int* nums, int numsSize) {
    if (numsSize == 0) return 0;
    
    int slow = 1;  // Position for next unique element
    
    for (int fast = 1; fast < numsSize; fast++) {
        // Found new unique element
        if (nums[fast] != nums[fast - 1]) {
            nums[slow] = nums[fast];
            slow++;
        }
    }
    
    return slow;  // New length
}

// Test function
void testRemoveDuplicates() {
    int nums[] = {1, 1, 2, 2, 3, 4, 4};
    int size = 7;
    
    printf("Original: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    
    int newSize = removeDuplicates(nums, size);
    
    printf("After removing duplicates: ");
    for (int i = 0; i < newSize; i++) {
        printf("%d ", nums[i]);
    }
    printf("\nNew length: %d\n", newSize);
}
```

**Why Two Pointers?**
- Input is **SORTED** → Duplicates are adjacent
- Need **IN-PLACE** modification → Slow-Fast variant
- Single pass required → Two Pointers perfect fit

#### Example 3: Squaring a Sorted Array

**Problem**: Square each element and return in sorted order.

```c
/**
 * Squares of a Sorted Array
 * 
 * Input: nums = [-4, -1, 0, 3, 10]
 * Output: [0, 1, 9, 16, 100]
 * 
 * Time: O(n), Space: O(n)
 * 
 * Key Insight: Largest squares are at the ends (largest absolute values)
 */
int* sortedSquares(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;
    int* result = (int*)malloc(numsSize * sizeof(int));
    
    int left = 0;
    int right = numsSize - 1;
    int pos = numsSize - 1;  // Fill from end (largest first)
    
    while (left <= right) {
        int leftSquare = nums[left] * nums[left];
        int rightSquare = nums[right] * nums[right];
        
        if (leftSquare > rightSquare) {
            result[pos] = leftSquare;
            left++;
        } else {
            result[pos] = rightSquare;
            right--;
        }
        pos--;
    }
    
    return result;
}

// Test function
void testSortedSquares() {
    int nums[] = {-4, -1, 0, 3, 10};
    int size = 5;
    int returnSize;
    
    printf("Input: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    
    int* result = sortedSquares(nums, size, &returnSize);
    
    printf("Squared and sorted: ");
    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    free(result);
}
```

**Why Two Pointers?**
- Input is **SORTED** → But squares may not be
- Largest absolute values at **ENDS** → Compare from both ends
- Need O(n) without sorting → Two Pointers from ends

#### Example 4: Three Sum Problem

**Problem**: Find all unique triplets that sum to zero.

```c
/**
 * Three Sum
 * 
 * Input: nums = [-1, 0, 1, 2, -1, -4]
 * Output: [[-1, -1, 2], [-1, 0, 1]]
 * 
 * Time: O(n²), Space: O(1) excluding output
 */
#include <stdlib.h>

// Comparison function for qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    // Sort array first
    qsort(nums, numsSize, sizeof(int), compare);
    
    // Allocate space for results
    int capacity = 100;
    int** result = (int**)malloc(capacity * sizeof(int*));
    *returnSize = 0;
    *returnColumnSizes = (int*)malloc(capacity * sizeof(int));
    
    // Fix first element, use two pointers for remaining two
    for (int i = 0; i < numsSize - 2; i++) {
        // Skip duplicates for first element
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        
        int target = -nums[i];
        int left = i + 1;
        int right = numsSize - 1;
        
        while (left < right) {
            int sum = nums[left] + nums[right];
            
            if (sum == target) {
                // Found triplet
                result[*returnSize] = (int*)malloc(3 * sizeof(int));
                result[*returnSize][0] = nums[i];
                result[*returnSize][1] = nums[left];
                result[*returnSize][2] = nums[right];
                (*returnColumnSizes)[*returnSize] = 3;
                (*returnSize)++;
                
                // Skip duplicates
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                
                left++;
                right--;
            } else if (sum < target) {
                left++;
            } else {
                right--;
            }
        }
    }
    
    return result;
}
```

**Why Two Pointers?**
- Can **SORT** input → Makes Two Pointers applicable
- Finding **TRIPLET** → Fix one, Two Pointers for two
- Need O(n²) solution → Outer loop + Two Pointers

### 4.4 Decision Checklist for Two Pointers

```
Use Two Pointers IF:
☑ Input is sorted (or can be sorted)
☑ Looking for pair/triplet/group
☑ Need in-place modification
☑ Comparing elements from different positions
☑ Can solve in O(n) or O(n²) with two pointers

DON'T Use Two Pointers IF:
☒ Need to maintain original order (and sorting breaks it)
☒ Problem requires more complex state tracking
☒ Random access needed repeatedly
☒ Better pattern available (e.g., hash table for unsorted)
```

### 4.5 Practice Problems

#### Beginner Level
1. **Two Sum II** - Sorted array, find pair with target sum
   - Input: `[2,7,11,15], target = 9`
   - Output: `[0,1]`
   - Hint: Use opposite direction pointers

2. **Valid Palindrome** - Check if string is palindrome
   - Input: `"A man, a plan, a canal: Panama"`
   - Output: `true`
   - Hint: Two pointers from ends, skip non-alphanumeric

3. **Remove Element** - Remove all occurrences of value
   - Input: `nums = [3,2,2,3], val = 3`
   - Output: `2` (nums = `[2,2,_,_]`)
   - Hint: Slow-fast pointers

4. **Reverse String** - Reverse character array in-place
   - Input: `["h","e","l","l","o"]`
   - Output: `["o","l","l","e","h"]`
   - Hint: Swap from ends

5. **Move Zeroes** - Move all zeros to end
   - Input: `[0,1,0,3,12]`
   - Output: `[1,3,12,0,0]`
   - Hint: Slow-fast, slow tracks non-zero position

#### Intermediate Level
6. **Container With Most Water** - Find two lines that form largest container
   - Input: `height = [1,8,6,2,5,4,8,3,7]`
   - Output: `49`
   - Hint: Start from ends, move pointer with smaller height

7. **Sort Colors** - Sort array of 0s, 1s, 2s (Dutch National Flag)
   - Input: `[2,0,2,1,1,0]`
   - Output: `[0,0,1,1,2,2]`
   - Hint: Three pointers (low, mid, high)

8. **Trapping Rain Water** - Calculate trapped water
   - Input: `height = [0,1,0,2,1,0,1,3,2,1,2,1]`
   - Output: `6`
   - Hint: Two pointers, track max heights

9. **3Sum Closest** - Find triplet closest to target
   - Input: `nums = [-1,2,1,-4], target = 1`
   - Output: `2` (sum = -1+2+1)
   - Hint: Sort + fix one + two pointers

10. **Remove Duplicates from Sorted Array II** - Allow at most 2 duplicates
    - Input: `[1,1,1,2,2,3]`
    - Output: `5` (nums = `[1,1,2,2,3,_]`)
    - Hint: Slow-fast, check against slow-2

#### Advanced Level
11. **4Sum** - Find all unique quadruplets that sum to target
    - Input: `nums = [1,0,-1,0,-2,2], target = 0`
    - Output: `[[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]`
    - Hint: Two loops + two pointers

12. **Subarray Product Less Than K** - Count subarrays with product < k
    - Input: `nums = [10,5,2,6], k = 100`
    - Output: `8`
    - Hint: Sliding window with two pointers

13. **Boats to Save People** - Minimum boats needed (max 2 people, weight limit)
    - Input: `people = [3,2,2,1], limit = 3`
    - Output: `3`
    - Hint: Sort + greedy two pointers

14. **Merge Sorted Array** - Merge nums2 into nums1 in-place
    - Input: `nums1 = [1,2,3,0,0,0], nums2 = [2,5,6]`
    - Output: `[1,2,2,3,5,6]`
    - Hint: Merge from end to avoid overwriting

15. **Partition Labels** - Partition string into max parts
    - Input: `s = "ababcbacadefegdehijhklij"`
    - Output: `[9,7,8]`
    - Hint: Track last occurrence + greedy partitioning

---

## Section 5: Sliding Window Pattern

### 5.1 Pattern Overview

**Core Concept**: Maintain a window that slides through the array/string to find optimal subarray/substring.

**When to Use - STRONG Signals:**
- ✅ Looking for **CONTIGUOUS** subarray/substring
- ✅ Keywords: "subarray", "substring", "consecutive"
- ✅ Need to find **MAXIMUM/MINIMUM** of size/sum/length
- ✅ **OPTIMIZE** over all possible subarrays
- ✅ Constraint on window (sum, length, distinct characters)

**When NOT to Use:**
- ❌ Need **NON-CONTIGUOUS** elements
- ❌ Elements can be picked from anywhere
- ❌ Need to consider all subsets (use backtracking)

**Visual Representation:**
```
Fixed Window:
[1, 2, 3, 4, 5, 6, 7, 8]
[-----]          window size = 3
   [-----]       slide right
      [-----]    slide right

Variable Window:
[1, 2, 3, 4, 5, 6, 7, 8]
[---]            expand when condition not met
[-------]        expand
   [-------]     shrink when condition met
```

### 5.2 Pattern Variants

#### Variant 1: Fixed Size Window

**Use Case**: Maximum/minimum sum of subarray of size k

```c
/**
 * Template: Fixed Size Sliding Window
 * Time: O(n), Space: O(1)
 */
int fixedWindowOptimum(int* arr, int size, int k) {
    if (size < k) return -1;
    
    // Step 1: Calculate first window
    int windowSum = 0;
    for (int i = 0; i < k; i++) {
        windowSum += arr[i];
    }
    
    int result = windowSum;  // Initialize result
    
    // Step 2: Slide window
    for (int i = k; i < size; i++) {
        // Add new element, remove old element
        windowSum = windowSum + arr[i] - arr[i - k];
        
        // Update result
        result = max(result, windowSum);  // or min, depending on problem
    }
    
    return result;
}
```

#### Variant 2: Variable Size Window (Expand/Shrink)

**Use Case**: Find smallest/largest window satisfying condition

```c
/**
 * Template: Variable Size Sliding Window
 * Time: O(n), Space: O(1) or O(k) for state
 */
int variableWindow(int* arr, int size, int target) {
    int windowStart = 0;
    int windowState = 0;  // Could be sum, count, etc.
    int result = INT_MAX;  // or INT_MIN, depending on problem
    
    for (int windowEnd = 0; windowEnd < size; windowEnd++) {
        // Step 1: EXPAND - Add current element to window
        windowState += arr[windowEnd];
        
        // Step 2: SHRINK - While condition is met/violated
        while (windowState >= target && windowStart <= windowEnd) {
            // Update result before shrinking
            int windowSize = windowEnd - windowStart + 1;
            result = min(result, windowSize);
            
            // Shrink window from left
            windowState -= arr[windowStart];
            windowStart++;
        }
    }
    
    return (result == INT_MAX) ? 0 : result;
}
```

#### Variant 3: Window with HashMap/Frequency Counter

**Use Case**: Substring with distinct characters, character frequency constraints

```c
/**
 * Template: Sliding Window with Character Frequency
 * Time: O(n), Space: O(k) where k is character set size
 */
int windowWithFrequency(char* s, int condition) {
    int freq[256] = {0};  // Frequency map
    int windowStart = 0;
    int distinctCount = 0;  // Or any other state
    int result = 0;
    
    for (int windowEnd = 0; s[windowEnd] != '\0'; windowEnd++) {
        // Expand window
        char rightChar = s[windowEnd];
        freq[rightChar]++;
        
        if (freq[rightChar] == 1) {
            distinctCount++;
        }
        
        // Shrink window if needed
        while (distinctCount > condition) {
            char leftChar = s[windowStart];
            freq[leftChar]--;
            
            if (freq[leftChar] == 0) {
                distinctCount--;
            }
            windowStart++;
        }
        
        // Update result
        int windowSize = windowEnd - windowStart + 1;
        result = max(result, windowSize);
    }
    
    return result;
}
```

### 5.3 Implementation Examples

#### Example 1: Maximum Sum Subarray of Size K

**Problem**: Find maximum sum of any contiguous subarray of size k.

```c
#include <stdio.h>
#include <limits.h>

/**
 * Maximum Sum Subarray of Size K
 * 
 * Input: arr = [2, 1, 5, 1, 3, 2], k = 3
 * Output: 9 (subarray [5, 1, 3])
 * 
 * Time: O(n), Space: O(1)
 */
int maxSumSubarray(int* arr, int size, int k) {
    if (size < k) {
        printf("Error: Array size less than k\n");
        return -1;
    }
    
    // Calculate sum of first window
    int windowSum = 0;
    for (int i = 0; i < k; i++) {
        windowSum += arr[i];
    }
    
    int maxSum = windowSum;
    
    // Slide the window
    for (int i = k; i < size; i++) {
        // Add new element, remove leftmost element
        windowSum = windowSum + arr[i] - arr[i - k];
        
        if (windowSum > maxSum) {
            maxSum = windowSum;
        }
    }
    
    return maxSum;
}

// Test function
void testMaxSumSubarray() {
    int arr[] = {2, 1, 5, 1, 3, 2};
    int k = 3;
    int size = sizeof(arr) / sizeof(arr[0]);
    
    printf("Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nWindow size: %d\n", k);
    
    int result = maxSumSubarray(arr, size, k);
    printf("Maximum sum of subarray of size %d: %d\n", k, result);
}
```

**Why Sliding Window?**
- Looking for **SUBARRAY** (contiguous elements)
- **FIXED SIZE** k
- Need to check all possible windows → Sliding Window O(n) vs Brute Force O(n*k)

#### Example 2: Smallest Subarray with Sum Greater Than Target

**Problem**: Find smallest contiguous subarray with sum ≥ target.

```c
/**
 * Smallest Subarray with Given Sum
 * 
 * Input: arr = [2, 1, 5, 2, 3, 2], target = 7
 * Output: 2 (subarray [5, 2])
 * 
 * Time: O(n), Space: O(1)
 */
int minSubarrayLen(int target, int* nums, int numsSize) {
    int windowStart = 0;
    int windowSum = 0;
    int minLength = INT_MAX;
    
    for (int windowEnd = 0; windowEnd < numsSize; windowEnd++) {
        // Expand window
        windowSum += nums[windowEnd];
        
        // Shrink window while condition is met
        while (windowSum >= target) {
            // Update result
            int currentLength = windowEnd - windowStart + 1;
            if (currentLength < minLength) {
                minLength = currentLength;
            }
            
            // Try to shrink
            windowSum -= nums[windowStart];
            windowStart++;
        }
    }
    
    return (minLength == INT_MAX) ? 0 : minLength;
}

// Test function
void testMinSubarrayLen() {
    int arr[] = {2, 1, 5, 2, 3, 2};
    int target = 7;
    int size = sizeof(arr) / sizeof(arr[0]);
    
    printf("Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nTarget sum: %d\n", target);
    
    int result = minSubarrayLen(target, arr, size);
    if (result > 0) {
        printf("Minimum length of subarray with sum >= %d: %d\n", target, result);
    } else {
        printf("No subarray found with sum >= %d\n", target);
    }
}
```

**Why Sliding Window?**
- Looking for **SUBARRAY** with condition
- **VARIABLE SIZE** → Expand and shrink
- Greedy approach works → Shrink when possible

#### Example 3: Longest Substring Without Repeating Characters

**Problem**: Find length of longest substring without repeating characters.

```c
#include <string.h>

/**
 * Longest Substring Without Repeating Characters
 * 
 * Input: s = "abcabcbb"
 * Output: 3 (substring "abc")
 * 
 * Time: O(n), Space: O(min(n, m)) where m is character set size
 */
int lengthOfLongestSubstring(char* s) {
    int charIndex[256];  // Last seen index of each character
    for (int i = 0; i < 256; i++) {
        charIndex[i] = -1;
    }
    
    int maxLength = 0;
    int windowStart = 0;
    
    for (int windowEnd = 0; s[windowEnd] != '\0'; windowEnd++) {
        char currentChar = s[windowEnd];
        
        // If character already in window, move start
        if (charIndex[currentChar] >= windowStart) {
            windowStart = charIndex[currentChar] + 1;
        }
        
        // Update last seen index
        charIndex[currentChar] = windowEnd;
        
        // Update max length
        int currentLength = windowEnd - windowStart + 1;
        if (currentLength > maxLength) {
            maxLength = currentLength;
        }
    }
    
    return maxLength;
}

// Alternative implementation with frequency map
int lengthOfLongestSubstringAlt(char* s) {
    int freq[256] = {0};
    int windowStart = 0;
    int maxLength = 0;
    
    for (int windowEnd = 0; s[windowEnd] != '\0'; windowEnd++) {
        char rightChar = s[windowEnd];
        freq[rightChar]++;
        
        // Shrink window until no duplicates
        while (freq[rightChar] > 1) {
            char leftChar = s[windowStart];
            freq[leftChar]--;
            windowStart++;
        }
        
        // Update max length
        int currentLength = windowEnd - windowStart + 1;
        maxLength = (currentLength > maxLength) ? currentLength : maxLength;
    }
    
    return maxLength;
}

// Test function
void testLongestSubstring() {
    char* testCases[] = {
        "abcabcbb",
        "bbbbb",
        "pwwkew",
        "abcdefgh",
        ""
    };
    
    int numTests = sizeof(testCases) / sizeof(testCases[0]);
    
    for (int i = 0; i < numTests; i++) {
        int result = lengthOfLongestSubstring(testCases[i]);
        printf("Input: \"%s\"\n", testCases[i]);
        printf("Longest substring without repeating: %d\n\n", result);
    }
}
```

**Why Sliding Window?**
- Looking for **SUBSTRING** (contiguous)
- Need to track **CHARACTERS** in current window
- **VARIABLE SIZE** → Expand and shrink based on duplicates

#### Example 4: Longest Substring with K Distinct Characters

**Problem**: Find longest substring with at most k distinct characters.

```c
/**
 * Longest Substring with At Most K Distinct Characters
 * 
 * Input: s = "eceba", k = 2
 * Output: 3 (substring "ece")
 * 
 * Time: O(n), Space: O(k)
 */
int lengthOfLongestSubstringKDistinct(char* s, int k) {
    if (k == 0) return 0;
    
    int freq[256] = {0};
    int distinctCount = 0;
    int windowStart = 0;
    int maxLength = 0;
    
    for (int windowEnd = 0; s[windowEnd] != '\0'; windowEnd++) {
        char rightChar = s[windowEnd];
        
        // Add to window
        if (freq[rightChar] == 0) {
            distinctCount++;
        }
        freq[rightChar]++;
        
        // Shrink window if too many distinct characters
        while (distinctCount > k) {
            char leftChar = s[windowStart];
            freq[leftChar]--;
            
            if (freq[leftChar] == 0) {
                distinctCount--;
            }
            windowStart++;
        }
        
        // Update max length
        int currentLength = windowEnd - windowStart + 1;
        maxLength = (currentLength > maxLength) ? currentLength : maxLength;
    }
    
    return maxLength;
}

// Test function
void testKDistinct() {
    char* s = "eceba";
    int k = 2;
    
    int result = lengthOfLongestSubstringKDistinct(s, k);
    printf("Input: \"%s\", k = %d\n", s, k);
    printf("Longest substring with at most %d distinct characters: %d\n", k, result);
}
```

**Why Sliding Window?**
- **SUBSTRING** with constraint
- Track **DISTINCT CHARACTERS** count
- **VARIABLE SIZE** → Shrink when exceeds k

#### Example 5: Fruits into Baskets (Two Distinct Types)

**Problem**: Pick maximum fruits from trees with at most 2 types.

```c
/**
 * Fruits into Baskets
 * (Longest subarray with at most 2 distinct values)
 * 
 * Input: fruits = [1, 2, 1, 2, 3, 2, 2]
 * Output: 5 (subarray [1, 2, 1, 2, 3] → types 2 and 3 → wait, that's wrong)
 * Correct: [2, 1, 2, 3] with types... Actually [2, 3, 2, 2] = 4
 * Let me recalculate: [2, 1, 2] = 3 types 1,2
 *                     [1, 2, 1, 2] = 4 types 1,2
 *                     [2, 3, 2, 2] = 4 types 2,3
 * Answer: 4
 * 
 * Time: O(n), Space: O(1)
 */
int totalFruit(int* fruits, int fruitsSize) {
    int freq[100001] = {0};  // Assuming fruit types < 100001
    int distinctTypes = 0;
    int windowStart = 0;
    int maxFruits = 0;
    
    for (int windowEnd = 0; windowEnd < fruitsSize; windowEnd++) {
        int rightFruit = fruits[windowEnd];
        
        // Add fruit to basket
        if (freq[rightFruit] == 0) {
            distinctTypes++;
        }
        freq[rightFruit]++;
        
        // If more than 2 types, remove from left
        while (distinctTypes > 2) {
            int leftFruit = fruits[windowStart];
            freq[leftFruit]--;
            
            if (freq[leftFruit] == 0) {
                distinctTypes--;
            }
            windowStart++;
        }
        
        // Update max fruits
        int currentFruits = windowEnd - windowStart + 1;
        maxFruits = (currentFruits > maxFruits) ? currentFruits : maxFruits;
    }
    
    return maxFruits;
}

// Test function
void testTotalFruit() {
    int fruits[] = {1, 2, 1, 2, 3, 2, 2};
    int size = sizeof(fruits) / sizeof(fruits[0]);
    
    printf("Fruits: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", fruits[i]);
    }
    printf("\n");
    
    int result = totalFruit(fruits, size);
    printf("Maximum fruits with at most 2 types: %d\n", result);
}
```

**Why Sliding Window?**
- **CONTIGUOUS** fruits (subarray)
- Constraint on **DISTINCT TYPES** (at most 2)
- This is actually "Longest substring with K distinct" where K=2

### 5.4 Decision Checklist for Sliding Window

```
Use Sliding Window IF:
☑ Looking for CONTIGUOUS subarray/substring
☑ Problem mentions "subarray", "substring", "consecutive"
☑ Need to optimize (max/min) over all subarrays
☑ Constraint on window (sum, length, distinct elements, frequency)
☑ Can expand/shrink window based on condition

DON'T Use Sliding Window IF:
☒ Elements don't need to be contiguous
☒ Need to pick elements from anywhere in array
☒ Problem requires all subsets (use backtracking)
☒ Need to sort or rearrange elements
```

### 5.5 Practice Problems

#### Beginner Level

1. **Average of Subarrays of Size K**
   - Input: `arr = [1, 3, 2, 6, -1, 4, 1, 8, 2], k = 5`
   - Output: `[2.2, 2.8, 2.4, 3.6, 2.8]`
   - Hint: Fixed window, calculate average

2. **Maximum Sum Subarray of Size K**
   - Input: `arr = [2, 1, 5, 1, 3, 2], k = 3`
   - Output: `9`
   - Hint: Fixed window, track maximum

3. **Find All Anagrams in a String**
   - Input: `s = "cbaebabacd", p = "abc"`
   - Output: `[0, 6]` (starting indices)
   - Hint: Fixed window of size len(p), frequency match

4. **Minimum Size Subarray Sum**
   - Input: `target = 7, nums = [2,3,1,2,4,3]`
   - Output: `2` (subarray [4,3])
   - Hint: Variable window, shrink when sum >= target

5. **Maximum Average Subarray I**
   - Input: `nums = [1,12,-5,-6,50,3], k = 4`
   - Output: `12.75` (subarray [12,-5,-6,50])
   - Hint: Fixed window, track max average

#### Intermediate Level

6. **Longest Substring with At Most K Distinct Characters**
   - Input: `s = "eceba", k = 2`
   - Output: `3` (substring "ece")
   - Hint: Variable window with frequency map

7. **Permutation in String**
   - Input: `s1 = "ab", s2 = "eidbaooo"`
   - Output: `true` (s2 contains "ba")
   - Hint: Fixed window, check frequency match

8. **Longest Repeating Character Replacement**
   - Input: `s = "AABABBA", k = 1`
   - Output: `4` (replace one B to get "AAAA")
   - Hint: Variable window, track max frequency

9. **Max Consecutive Ones III**
   - Input: `nums = [1,1,1,0,0,0,1,1,1,1,0], k = 2`
   - Output: `6` (flip two 0s)
   - Hint: Variable window, count zeros

10. **Subarray Product Less Than K**
    - Input: `nums = [10,5,2,6], k = 100`
    - Output: `8` (all subarrays with product < 100)
    - Hint: Variable window, count subarrays

#### Advanced Level

11. **Minimum Window Substring**
    - Input: `s = "ADOBECODEBANC", t = "ABC"`
    - Output: `"BANC"`
    - Hint: Variable window, track required characters

12. **Substring with Concatenation of All Words**
    - Input: `s = "barfoothefoobarman", words = ["foo","bar"]`
    - Output: `[0, 9]`
    - Hint: Fixed window of total word length

13. **Longest Substring with At Least K Repeating Characters**
    - Input: `s = "aaabb", k = 3`
    - Output: `3` (substring "aaa")
    - Hint: Divide and conquer or sliding window with recursion

14. **Subarrays with K Different Integers**
    - Input: `nums = [1,2,1,2,3], k = 2`
    - Output: `7`
    - Hint: Count with at most K - count with at most K-1

15. **Sliding Window Maximum**
    - Input: `nums = [1,3,-1,-3,5,3,6,7], k = 3`
    - Output: `[3,3,5,5,6,7]`
    - Hint: Use deque to maintain maximum in window

---

## Section 6: Fast and Slow Pointers

### 6.1 Pattern Overview

**Core Concept**: Two pointers moving at different speeds through a linked list or sequence. Also known as **Floyd's Cycle Detection** or **Tortoise and Hare**.

**When to Use - STRONG Signals:**
- ✅ **LINKED LIST** problem (99% confidence)
- ✅ Keywords: "middle", "cycle", "loop"
- ✅ Need to detect **CYCLE** in sequence
- ✅ Find element at **SPECIFIC POSITION** (middle, k-th from end)
- ✅ **SPACE CONSTRAINT** O(1) for linked list problems

**When NOT to Use:**
- ❌ Array problems (usually Two Pointers or Sliding Window better)
- ❌ Need to access previous nodes frequently
- ❌ Tree problems (use BFS/DFS instead)

**Visual Representation:**
```
Finding Middle:
1 → 2 → 3 → 4 → 5 → NULL
↑       ↑
slow   fast

After 1 step:
1 → 2 → 3 → 4 → 5 → NULL
    ↑           ↑
   slow        fast

After 2 steps:
1 → 2 → 3 → 4 → 5 → NULL
        ↑               ↑
       slow           fast (NULL)
Middle is at slow (node 3)

Cycle Detection:
1 → 2 → 3 → 4 → 5
        ↑_________|

They will meet inside the cycle!
```

### 6.2 Core Technique

```c
/**
 * Basic Fast and Slow Pointer Structure
 */
struct ListNode {
    int val;
    struct ListNode* next;
};

// Template: Fast moves 2x speed of slow
struct ListNode* fastSlowTemplate(struct ListNode* head) {
    if (!head || !head->next) return head;
    
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    
    // Standard pattern: fast moves 2 steps, slow moves 1 step
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;           // 1 step
        fast = fast->next->next;     // 2 steps
        
        // Check condition (varies by problem)
        if (slow == fast) {
            // Found something (cycle, meeting point, etc.)
            return slow;
        }
    }
    
    // No cycle or reached end
    return slow;  // Often the middle node
}
```

### 6.3 Implementation Examples

#### Example 1: Middle of Linked List

**Problem**: Find the middle node of a linked list.

```c
/**
 * Middle of the Linked List
 * 
 * Input: 1 → 2 → 3 → 4 → 5
 * Output: Node 3
 * 
 * Input: 1 → 2 → 3 → 4 → 5 → 6
 * Output: Node 4 (second middle when even)
 * 
 * Time: O(n), Space: O(1)
 */
struct ListNode* middleNode(struct ListNode* head) {
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    
    // When fast reaches end, slow is at middle
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

// Helper function to create linked list
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

// Helper function to print linked list
void printList(struct ListNode* head) {
    while (head != NULL) {
        printf("%d", head->val);
        if (head->next) printf(" → ");
        head = head->next;
    }
    printf("\n");
}

// Test function
void testMiddleNode() {
    // Create list: 1 → 2 → 3 → 4 → 5
    struct ListNode* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);
    
    printf("List: ");
    printList(head);
    
    struct ListNode* middle = middleNode(head);
    printf("Middle node value: %d\n", middle->val);
}
```

**Why Fast & Slow Pointers?**
- **LINKED LIST** problem → Primary signal
- Find **MIDDLE** without knowing length
- O(1) space required → Can't use array/counting

**Key Insight**: When fast pointer reaches end (moving 2x speed), slow pointer is exactly at middle.

#### Example 2: Linked List Cycle Detection

**Problem**: Detect if linked list has a cycle.

```c
#include <stdbool.h>

/**
 * Linked List Cycle
 * 
 * Input: 3 → 2 → 0 → -4
 *            ↑________|
 * Output: true
 * 
 * Time: O(n), Space: O(1)
 */
bool hasCycle(struct ListNode* head) {
    if (!head || !head->next) return false;
    
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        // If they meet, there's a cycle
        if (slow == fast) {
            return true;
        }
    }
    
    // Fast reached end, no cycle
    return false;
}

// Test function
void testHasCycle() {
    // Create list with cycle: 1 → 2 → 3 → 4
    //                              ↑______|
    struct ListNode* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = head->next;  // Create cycle
    
    bool result = hasCycle(head);
    printf("Has cycle: %s\n", result ? "true" : "false");
}
```

**Why Fast & Slow Pointers?**
- **CYCLE DETECTION** in linked list
- Can't use extra space (hash set would be O(n) space)
- Floyd's algorithm guarantees meeting in cycle

**Mathematical Proof**: If there's a cycle, fast pointer will eventually catch slow pointer because it gains one node per iteration.

#### Example 3: Find Cycle Start

**Problem**: Find where the cycle begins in a linked list.

```c
/**
 * Linked List Cycle II - Find Start of Cycle
 * 
 * Input: 3 → 2 → 0 → -4
 *            ↑________|
 * Output: Node with value 2
 * 
 * Time: O(n), Space: O(1)
 * 
 * Algorithm:
 * 1. Detect cycle using fast/slow
 * 2. Reset one pointer to head
 * 3. Move both at same speed - they meet at cycle start
 */
struct ListNode* detectCycle(struct ListNode* head) {
    if (!head || !head->next) return NULL;
    
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    
    // Phase 1: Detect if cycle exists
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            // Cycle detected
            // Phase 2: Find cycle start
            struct ListNode* ptr1 = head;
            struct ListNode* ptr2 = slow;
            
            while (ptr1 != ptr2) {
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
            }
            
            return ptr1;  // This is the start of cycle
        }
    }
    
    return NULL;  // No cycle
}
```

**Why this works?**

Let's say:
- Distance from head to cycle start = `a`
- Distance from cycle start to meeting point = `b`
- Remaining cycle length = `c`

When they meet:
- Slow traveled: `a + b`
- Fast traveled: `a + b + c + b` (one extra cycle)
- Since fast is 2x slow: `2(a + b) = a + 2b + c`
- Simplifying: `a = c`

This means distance from head to cycle start equals distance from meeting point to cycle start!

#### Example 4: Happy Number

**Problem**: Determine if a number is happy (sum of squares of digits eventually reaches 1).

```c
/**
 * Happy Number
 * 
 * Input: n = 19
 * Process: 1² + 9² = 82
 *          8² + 2² = 68
 *          6² + 8² = 100
 *          1² + 0² + 0² = 1  ← Happy!
 * Output: true
 * 
 * Input: n = 2
 * Process: Eventually cycles without reaching 1
 * Output: false
 * 
 * Time: O(log n), Space: O(1)
 */

// Helper function to get sum of squares of digits
int getNext(int n) {
    int sum = 0;
    while (n > 0) {
        int digit = n % 10;
        sum += digit * digit;
        n /= 10;
    }
    return sum;
}

bool isHappy(int n) {
    int slow = n;
    int fast = n;
    
    do {
        slow = getNext(slow);           // Move 1 step
        fast = getNext(getNext(fast));  // Move 2 steps
        
        if (fast == 1) return true;     // Reached 1, happy number
        
    } while (slow != fast);  // Continue until they meet (cycle detected)
    
    return false;  // Cycle detected without reaching 1
}

// Test function
void testIsHappy() {
    int testCases[] = {19, 2, 7, 10, 13};
    int numTests = sizeof(testCases) / sizeof(testCases[0]);
    
    for (int i = 0; i < numTests; i++) {
        bool result = isHappy(testCases[i]);
        printf("n = %d: %s\n", testCases[i], result ? "Happy" : "Not Happy");
    }
}
```

**Why Fast & Slow Pointers?**
- Detecting **CYCLE** in sequence (not linked list, but same principle)
- Either reaches 1 or cycles forever
- O(1) space vs hash set O(log n) space

#### Example 5: Palindrome Linked List

**Problem**: Check if linked list is a palindrome.

```c
/**
 * Palindrome Linked List
 * 
 * Input: 1 → 2 → 2 → 1
 * Output: true
 * 
 * Input: 1 → 2 → 3 → 2 → 1
 * Output: true
 * 
 * Approach:
 * 1. Find middle using fast/slow
 * 2. Reverse second half
 * 3. Compare first half with reversed second half
 * 
 * Time: O(n), Space: O(1)
 */

// Helper function to reverse linked list
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode* prev = NULL;
    struct ListNode* curr = head;
    
    while (curr != NULL) {
        struct ListNode* nextTemp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextTemp;
    }
    
    return prev;
}

bool isPalindrome(struct ListNode* head) {
    if (!head || !head->next) return true;
    
    // Step 1: Find middle using fast/slow pointers
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // Step 2: Reverse second half
    struct ListNode* secondHalf = reverseList(slow);
    struct ListNode* firstHalf = head;
    
    // Step 3: Compare both halves
    struct ListNode* secondHalfCopy = secondHalf;  // Save for restoration
    bool isPalin = true;
    
    while (secondHalf != NULL) {
        if (firstHalf->val != secondHalf->val) {
            isPalin = false;
            break;
        }
        firstHalf = firstHalf->next;
        secondHalf = secondHalf->next;
    }
    
    // Step 4: Restore list (optional, good practice)
    reverseList(secondHalfCopy);
    
    return isPalin;
}

// Test function
void testIsPalindrome() {
    // Test case 1: 1 → 2 → 2 → 1
    struct ListNode* head1 = createNode(1);
    head1->next = createNode(2);
    head1->next->next = createNode(2);
    head1->next->next->next = createNode(1);
    
    printf("List 1: ");
    printList(head1);
    printf("Is palindrome: %s\n\n", isPalindrome(head1) ? "true" : "false");
    
    // Test case 2: 1 → 2 → 3 → 2 → 1
    struct ListNode* head2 = createNode(1);
    head2->next = createNode(2);
    head2->next->next = createNode(3);
    head2->next->next->next = createNode(2);
    head2->next->next->next->next = createNode(1);
    
    printf("List 2: ");
    printList(head2);
    printf("Is palindrome: %s\n", isPalindrome(head2) ? "true" : "false");
}
```

**Why Fast & Slow Pointers?**
- Need to find **MIDDLE** of linked list
- Then work with both halves
- O(1) space requirement

### 6.4 Decision Checklist for Fast & Slow Pointers

```
Use Fast & Slow Pointers IF:
☑ Problem involves LINKED LIST
☑ Need to find MIDDLE element
☑ Detect CYCLE in linked list or sequence
☑ Find K-th element from end
☑ O(1) space required
☑ Two pointers moving at different speeds makes sense

DON'T Use Fast & Slow Pointers IF:
☒ Working with arrays (use Two Pointers instead)
☒ Need to access multiple elements simultaneously
☒ Tree or graph problem (use BFS/DFS)
☒ Can use extra space (hash table might be simpler)
```

### 6.5 Practice Problems

#### Beginner Level

1. **Middle of the Linked List**
   - Input: `1 → 2 → 3 → 4 → 5`
   - Output: Node `3`
   - Hint: Fast moves 2x, slow 1x

2. **Linked List Cycle**
   - Input: `3 → 2 → 0 → -4 (cycle back to 2)`
   - Output: `true`
   - Hint: If they meet, there's a cycle

3. **Remove Nth Node From End of List**
   - Input: `1 → 2 → 3 → 4 → 5`, n = 2
   - Output: `1 → 2 → 3 → 5`
   - Hint: Fast pointer moves n steps ahead first

4. **Happy Number**
   - Input: `19`
   - Output: `true`
   - Hint: Cycle detection in number sequence

5. **Palindrome Linked List**
   - Input: `1 → 2 → 2 → 1`
   - Output: `true`
   - Hint: Find middle, reverse second half, compare

#### Intermediate Level

6. **Linked List Cycle II** (Find cycle start)
   - Input: `3 → 2 → 0 → -4 (cycle at 2)`
   - Output: Node `2`
   - Hint: After detecting cycle, reset one pointer to head

7. **Reorder List**
   - Input: `1 → 2 → 3 → 4 → 5`
   - Output: `1 → 5 → 2 → 4 → 3`
   - Hint: Find middle, reverse second half, merge alternately

8. **Delete the Middle Node**
   - Input: `1 → 3 → 4 → 7 → 1 → 2 → 6`
   - Output: `1 → 3 → 4 → 1 → 2 → 6`
   - Hint: Fast/slow to find middle, keep prev pointer

9. **Circular Array Loop**
   - Input: `[2, -1, 1, 2, 2]`
   - Output: `true` (there's a positive direction cycle)
   - Hint: Fast/slow pointers with direction checking

10. **Find Duplicate Number**
    - Input: `[1, 3, 4, 2, 2]` (n+1 numbers in range [1, n])
    - Output: `2`
    - Hint: Treat as linked list, use Floyd's algorithm

#### Advanced Level

11. **Intersection of Two Linked Lists**
    - Input: Two lists intersecting at a common node
    - Output: The intersection node
    - Hint: Modified fast/slow or two pointers

12. **Split Linked List in Parts**
    - Input: `[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]`, k = 3
    - Output: `[[1,2,3,4], [5,6,7], [8,9,10]]`
    - Hint: Calculate part sizes, use fast/slow for counting

13. **Copy List with Random Pointer**
    - Input: Linked list with random pointers
    - Output: Deep copy
    - Hint: Can use fast/slow variations or interleaving

14. **Linked List Random Node**
    - Input: Linked list
    - Output: Random node with equal probability
    - Hint: Reservoir sampling with slow pointer

15. **Rotate List**
    - Input: `1 → 2 → 3 → 4 → 5`, k = 2
    - Output: `4 → 5 → 1 → 2 → 3`
    - Hint: Find length and k-th from end using fast/slow

---

## Section 7: Merge Intervals Pattern

### 7.1 Pattern Overview

**Core Concept**: Deal with overlapping intervals, merge them, or insert new intervals into existing sorted intervals.

**When to Use - STRONG Signals:**
- ✅ Problem mentions **"INTERVALS"** or **"RANGES"**
- ✅ Keywords: "merge", "overlap", "intersect"
- ✅ Input is array of **[start, end]** pairs
- ✅ Scheduling problems (meetings, tasks)
- ✅ Timeline problems

**When NOT to Use:**
- ❌ Single points, not ranges
- ❌ No overlap concept involved
- ❌ Tree interval problems (use interval trees)

**Visual Representation:**
```
Overlapping Intervals:
[1, 3] [2, 6] [8, 10] [15, 18]
  |--|
    |------|
Merge to: [1, 6]

[1, 3] overlaps with [2, 6] because 2 ≤ 3
Result: [1, max(3, 6)] = [1, 6]
```

### 7.2 Core Technique

```c
/**
 * Interval Structure
 */
struct Interval {
    int start;
    int end;
};

/**
 * Step 1: ALWAYS sort intervals by start time
 * Step 2: Iterate and merge if overlap exists
 * Step 3: Add merged interval to result
 */

// Comparison function for sorting
int compareIntervals(const void* a, const void* b) {
    struct Interval* i1 = (struct Interval*)a;
    struct Interval* i2 = (struct Interval*)b;
    return i1->start - i2->start;
}

// Check if two intervals overlap
bool isOverlap(struct Interval* i1, struct Interval* i2) {
    return i1->end >= i2->start;  // i1 ends at or after i2 starts
}
```

### 7.3 Implementation Examples

#### Example 1: Merge Intervals

**Problem**: Merge all overlapping intervals.

```c
#include <stdio.h>
#include <stdlib.h>

/**
 * Merge Intervals
 * 
 * Input: [[1,3], [2,6], [8,10], [15,18]]
 * Output: [[1,6], [8,10], [15,18]]
 * 
 * Time: O(n log n) for sorting, Space: O(n)
 */

// For LeetCode style with 2D array
int** merge(int** intervals, int intervalsSize, int* intervalsColSize, 
            int* returnSize, int** returnColumnSizes) {
    
    if (intervalsSize == 0) {
        *returnSize = 0;
        return NULL;
    }
    
    // Step 1: Sort by start time
    // Use qsort with custom comparator
    qsort(intervals, intervalsSize, sizeof(int*), 
          (int (*)(const void*, const void*))compareIntervals);
    
    // Allocate result array
    int** result = (int**)malloc(intervalsSize * sizeof(int*));
    *returnColumnSizes = (int*)malloc(intervalsSize * sizeof(int));
    *returnSize = 0;
    
    // Step 2: Start with first interval
    result[0] = (int*)malloc(2 * sizeof(int));
    result[0][0] = intervals[0][0];  // start
    result[0][1] = intervals[0][1];  // end
    (*returnColumnSizes)[0] = 2;
    *returnSize = 1;
    
    // Step 3: Merge overlapping intervals
    for (int i = 1; i < intervalsSize; i++) {
        int currentStart = intervals[i][0];
        int currentEnd = intervals[i][1];
        int lastEnd = result[*returnSize - 1][1];
        
        // Check if current overlaps with last merged interval
        if (currentStart <= lastEnd) {
            // Overlap exists, merge by extending end
            if (currentEnd > lastEnd) {
                result[*returnSize - 1][1] = currentEnd;
            }
        } else {
            // No overlap, add as new interval
            result[*returnSize] = (int*)malloc(2 * sizeof(int));
            result[*returnSize][0] = currentStart;
            result[*returnSize][1] = currentEnd;
            (*returnColumnSizes)[*returnSize] = 2;
            (*returnSize)++;
        }
    }
    
    return result;
}

// Cleaner version with struct
struct Interval* mergeIntervals(struct Interval* intervals, int size, int* returnSize) {
    if (size == 0) {
        *returnSize = 0;
        return NULL;
    }
    
    // Step 1: Sort
    qsort(intervals, size, sizeof(struct Interval), compareIntervals);
    
    // Result array
    struct Interval* result = (struct Interval*)malloc(size * sizeof(struct Interval));
    result[0] = intervals[0];
    *returnSize = 1;
    
    // Step 2: Merge
    for (int i = 1; i < size; i++) {
        struct Interval* lastMerged = &result[*returnSize - 1];
        struct Interval* current = &intervals[i];
        
        if (current->start <= lastMerged->end) {
            // Merge
            if (current->end > lastMerged->end) {
                lastMerged->end = current->end;
            }
        } else {
            // Add new interval
            result[*returnSize] = *current;
            (*returnSize)++;
        }
    }
    
    return result;
}

// Test function
void testMergeIntervals() {
    struct Interval intervals[] = {
        {1, 3}, {2, 6}, {8, 10}, {15, 18}
    };
    int size = 4;
    int returnSize;
    
    printf("Input intervals:\n");
    for (int i = 0; i < size; i++) {
        printf("[%d, %d] ", intervals[i].start, intervals[i].end);
    }
    printf("\n");
    
    struct Interval* result = mergeIntervals(intervals, size, &returnSize);
    
    printf("Merged intervals:\n");
    for (int i = 0; i < returnSize; i++) {
        printf("[%d, %d] ", result[i].start, result[i].end);
    }
    printf("\n");
    
    free(result);
}
```

**Why Merge Intervals Pattern?**
- Problem explicitly about **INTERVALS**
- Need to handle **OVERLAPS**
- Sort + Merge is classic approach

**Key Steps:**
1. **Sort** by start time
2. **Compare** current interval with last merged
3. **Merge** if overlap, otherwise add new

#### Example 2: Insert Interval

**Problem**: Insert a new interval and merge if necessary.

```c
/**
 * Insert Interval
 * 
 * Input: intervals = [[1,3],[6,9]], newInterval = [2,5]
 * Output: [[1,5],[6,9]]
 * 
 * Input: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
 * Output: [[1,2],[3,10],[12,16]]
 * 
 * Time: O(n), Space: O(n)
 */
int** insert(int** intervals, int intervalsSize, int* intervalsColSize,
             int* newInterval, int newIntervalSize,
             int* returnSize, int** returnColumnSizes) {
    
    int** result = (int**)malloc((intervalsSize + 1) * sizeof(int*));
    *returnColumnSizes = (int*)malloc((intervalsSize + 1) * sizeof(int));
    *returnSize = 0;
    int i = 0;
    
    // Step 1: Add all intervals that end before newInterval starts
    while (i < intervalsSize && intervals[i][1] < newInterval[0]) {
        result[*returnSize] = (int*)malloc(2 * sizeof(int));
        result[*returnSize][0] = intervals[i][0];
        result[*returnSize][1] = intervals[i][1];
        (*returnColumnSizes)[*returnSize] = 2;
        (*returnSize)++;
        i++;
    }
    
    // Step 2: Merge all overlapping intervals with newInterval
    int mergedStart = newInterval[0];
    int mergedEnd = newInterval[1];
    
    while (i < intervalsSize && intervals[i][0] <= newInterval[1]) {
        mergedStart = (intervals[i][0] < mergedStart) ? intervals[i][0] : mergedStart;
        mergedEnd = (intervals[i][1] > mergedEnd) ? intervals[i][1] : mergedEnd;
        i++;
    }
    
    // Add merged interval
    result[*returnSize] = (int*)malloc(2 * sizeof(int));
    result[*returnSize][0] = mergedStart;
    result[*returnSize][1] = mergedEnd;
    (*returnColumnSizes)[*returnSize] = 2;
    (*returnSize)++;
    
    // Step 3: Add remaining intervals
    while (i < intervalsSize) {
        result[*returnSize] = (int*)malloc(2 * sizeof(int));
        result[*returnSize][0] = intervals[i][0];
        result[*returnSize][1] = intervals[i][1];
        (*returnColumnSizes)[*returnSize] = 2;
        (*returnSize)++;
        i++;
    }
    
    return result;
}

// Test function
void testInsertInterval() {
    int** intervals = (int**)malloc(2 * sizeof(int*));
    intervals[0] = (int*)malloc(2 * sizeof(int));
    intervals[0][0] = 1; intervals[0][1] = 3;
    intervals[1] = (int*)malloc(2 * sizeof(int));
    intervals[1][0] = 6; intervals[1][1] = 9;
    
    int newInterval[] = {2, 5};
    int returnSize;
    int* returnColumnSizes;
    int colSize = 2;
    
    printf("Original intervals: [1,3] [6,9]\n");
    printf("Insert: [2,5]\n");
    
    int** result = insert(intervals, 2, &colSize, newInterval, 2, 
                         &returnSize, &returnColumnSizes);
    
    printf("Result: ");
    for (int i = 0; i < returnSize; i++) {
        printf("[%d,%d] ", result[i][0], result[i][1]);
    }
    printf("\n");
}
```

**Why Insert Interval Pattern?**
- Working with sorted **INTERVALS**
- Need to handle **MERGING** with new interval
- Three-phase approach: before, overlap, after

#### Example 3: Meeting Rooms

**Problem**: Can a person attend all meetings?

```c
/**
 * Meeting Rooms
 * 
 * Input: [[0,30], [5,10], [15,20]]
 * Output: false (overlap between [0,30] and others)
 * 
 * Input: [[7,10], [2,4]]
 * Output: true
 * 
 * Time: O(n log n), Space: O(1)
 */
bool canAttendMeetings(int** intervals, int intervalsSize, int* intervalsColSize) {
    if (intervalsSize <= 1) return true;
    
    // Sort by start time
    qsort(intervals, intervalsSize, sizeof(int*), compareIntervals);
    
    // Check for overlaps
    for (int i = 1; i < intervalsSize; i++) {
        // If current starts before previous ends, overlap exists
        if (intervals[i][0] < intervals[i-1][1]) {
            return false;
        }
    }
    
    return true;
}

// Test function
void testCanAttendMeetings() {
    int** meetings = (int**)malloc(3 * sizeof(int*));
    meetings[0] = (int*)malloc(2 * sizeof(int));
    meetings[0][0] = 0; meetings[0][1] = 30;
    meetings[1] = (int*)malloc(2 * sizeof(int));
    meetings[1][0] = 5; meetings[1][1] = 10;
    meetings[2] = (int*)malloc(2 * sizeof(int));
    meetings[2][0] = 15; meetings[2][1] = 20;
    
    int colSize = 2;
    bool result = canAttendMeetings(meetings, 3, &colSize);
    
    printf("Meetings: [0,30] [5,10] [15,20]\n");
    printf("Can attend all: %s\n", result ? "true" : "false");
}
```

**Why Meeting Rooms Pattern?**
- **INTERVALS** representing meetings
- Check for **OVERLAPS**
- Sort + check adjacent pairs

#### Example 4: Meeting Rooms II

**Problem**: Minimum number of conference rooms required.

```c
/**
 * Meeting Rooms II
 * 
 * Input: [[0,30], [5,10], [15,20]]
 * Output: 2 (need 2 rooms: one for [0,30], another for [5,10] and [15,20])
 * 
 * Approach: Track all start and end times separately
 * 
 * Time: O(n log n), Space: O(n)
 */
int minMeetingRooms(int** intervals, int intervalsSize, int* intervalsColSize) {
    if (intervalsSize == 0) return 0;
    
    // Separate start and end times
    int* starts = (int*)malloc(intervalsSize * sizeof(int));
    int* ends = (int*)malloc(intervalsSize * sizeof(int));
    
    for (int i = 0; i < intervalsSize; i++) {
        starts[i] = intervals[i][0];
        ends[i] = intervals[i][1];
    }
    
    // Sort both arrays
    qsort(starts, intervalsSize, sizeof(int), 
          (int (*)(const void*, const void*))compareInts);
    qsort(ends, intervalsSize, sizeof(int), 
          (int (*)(const void*, const void*))compareInts);
    
    int rooms = 0;
    int maxRooms = 0;
    int startPtr = 0;
    int endPtr = 0;
    
    // Sweep through timeline
    while (startPtr < intervalsSize) {
        if (starts[startPtr] < ends[endPtr]) {
            // Meeting starts, need a room
            rooms++;
            startPtr++;
            maxRooms = (rooms > maxRooms) ? rooms : maxRooms;
        } else {
            // Meeting ends, free a room
            rooms--;
            endPtr++;
        }
    }
    
    free(starts);
    free(ends);
    return maxRooms;
}

// Helper comparison function
int compareInts(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Test function
void testMinMeetingRooms() {
    int** meetings = (int**)malloc(3 * sizeof(int*));
    meetings[0] = (int*)malloc(2 * sizeof(int));
    meetings[0][0] = 0; meetings[0][1] = 30;
    meetings[1] = (int*)malloc(2 * sizeof(int));
    meetings[1][0] = 5; meetings[1][1] = 10;
    meetings[2] = (int*)malloc(2 * sizeof(int));
    meetings[2][0] = 15; meetings[2][1] = 20;
    
    int colSize = 2;
    int result = minMeetingRooms(meetings, 3, &colSize);
    
    printf("Meetings: [0,30] [5,10] [15,20]\n");
    printf("Minimum rooms required: %d\n", result);
}
```

**Why Meeting Rooms II Pattern?**
- **INTERVALS** with overlap
- Need to count **CONCURRENT** intervals
- Sweep line algorithm (separate start/end events)

#### Example 5: Non-overlapping Intervals

**Problem**: Minimum intervals to remove to make rest non-overlapping.

```c
/**
 * Non-overlapping Intervals
 * 
 * Input: [[1,2], [2,3], [3,4], [1,3]]
 * Output: 1 (remove [1,3])
 * 
 * Approach: Greedy - always keep interval with earliest end time
 * 
 * Time: O(n log n), Space: O(1)
 */

// Sort by end time
int compareByEnd(const void* a, const void* b) {
    int** i1 = (int**)a;
    int** i2 = (int**)b;
    return (*i1)[1] - (*i2)[1];
}

int eraseOverlapIntervals(int** intervals, int intervalsSize, int* intervalsColSize) {
    if (intervalsSize <= 1) return 0;
    
    // Sort by end time
    qsort(intervals, intervalsSize, sizeof(int*), compareByEnd);
    
    int removed = 0;
    int prevEnd = intervals[0][1];
    
    for (int i = 1; i < intervalsSize; i++) {
        // If current starts before previous ends, overlap
        if (intervals[i][0] < prevEnd) {
            removed++;  // Remove current (keep one with earlier end)
        } else {
            // No overlap, update end
            prevEnd = intervals[i][1];
        }
    }
    
    return removed;
}

// Test function
void testEraseOverlapIntervals() {
    int** intervals = (int**)malloc(4 * sizeof(int*));
    intervals[0] = (int*)malloc(2 * sizeof(int));
    intervals[0][0] = 1; intervals[0][1] = 2;
    intervals[1] = (int*)malloc(2 * sizeof(int));
    intervals[1][0] = 2; intervals[1][1] = 3;
    intervals[2] = (int*)malloc(2 * sizeof(int));
    intervals[2][0] = 3; intervals[2][1] = 4;
    intervals[3] = (int*)malloc(2 * sizeof(int));
    intervals[3][0] = 1; intervals[3][1] = 3;
    
    int colSize = 2;
    int result = eraseOverlapIntervals(intervals, 4, &colSize);
    
    printf("Intervals: [1,2] [2,3] [3,4] [1,3]\n");
    printf("Minimum intervals to remove: %d\n", result);
}
```

**Why Non-overlapping Pattern?**
- **INTERVALS** with overlaps
- Need to **REMOVE** minimum to make non-overlapping
- Greedy algorithm with sorting by end time

### 7.4 Decision Checklist for Merge Intervals

```
Use Merge Intervals IF:
☑ Problem mentions "intervals", "ranges", "time slots"
☑ Input is array of [start, end] pairs
☑ Need to merge overlapping intervals
☑ Scheduling/calendar problems
☑ Timeline-based problems
☑ Check for overlaps or conflicts

DON'T Use Merge Intervals IF:
☒ Not dealing with ranges/intervals
☒ Points, not ranges
☒ Tree-based interval queries (use interval trees)
☒ Need fast query on intervals (use segment tree)
```

### 7.5 Practice Problems

#### Beginner Level

1. **Merge Intervals**
   - Input: `[[1,3], [2,6], [8,10], [15,18]]`
   - Output: `[[1,6], [8,10], [15,18]]`
   - Hint: Sort by start, merge overlapping

2. **Meeting Rooms**
   - Input: `[[0,30], [5,10], [15,20]]`
   - Output: `false`
   - Hint: Sort and check adjacent pairs

3. **Insert Interval**
   - Input: `intervals = [[1,3],[6,9]]`, `newInterval = [2,5]`
   - Output: `[[1,5], [6,9]]`
   - Hint: Three phases: before, merge, after

4. **Summary Ranges**
   - Input: `[0,1,2,4,5,7]`
   - Output: `["0->2", "4->5", "7"]`
   - Hint: Find consecutive sequences

5. **Missing Ranges**
   - Input: `nums = [0,1,3,50,75]`, `lower = 0`, `upper = 99`
   - Output: `["2", "4->49", "51->74", "76->99"]`
   - Hint: Find gaps between intervals

#### Intermediate Level

6. **Meeting Rooms II**
   - Input: `[[0,30], [5,10], [15,20]]`
   - Output: `2`
   - Hint: Separate start/end times, sweep line

7. **Non-overlapping Intervals**
   - Input: `[[1,2], [2,3], [3,4], [1,3]]`
   - Output: `1`
   - Hint: Sort by end time, greedy

8. **Interval List Intersections**
   - Input: `A = [[0,2],[5,10]]`, `B = [[1,5],[8,12]]`
   - Output: `[[1,2], [5,5], [8,10]]`
   - Hint: Two pointers on two sorted lists

9. **Employee Free Time**
   - Input: Schedule of employees (list of intervals)
   - Output: Common free time intervals
   - Hint: Merge all intervals, find gaps

10. **Partition Labels**
    - Input: `S = "ababcbacadefegdehijhklij"`
    - Output: `[9,7,8]`
    - Hint: Track last occurrence, create intervals

#### Advanced Level

11. **My Calendar I** (Can book event)
    - Input: Sequence of book(start, end) calls
    - Output: `true` if can book, `false` if overlaps
    - Hint: Maintain sorted intervals, check overlaps

12. **My Calendar II** (Can book with at most 1 overlap)
    - Input: Sequence of book(start, end) calls
    - Output: `true` if at most 1 overlap
    - Hint: Track single and double bookings

13. **My Calendar III** (Return max bookings at any time)
    - Input: Sequence of book(start, end) calls
    - Output: Maximum k-bookings
    - Hint: Sweep line with start/end events

14. **Count Intervals**
    - Input: Add intervals, query count of covered integers
    - Output: Total integers covered
    - Hint: Maintain merged intervals

15. **Minimum Interval to Include Each Query**
    - Input: `intervals`, `queries` (points)
    - Output: For each query, smallest interval containing it
    - Hint: Sort both, use heap

---

## Section 8: Top K Elements Pattern

### 8.1 Pattern Overview

**Core Concept**: Find K largest, smallest, or most frequent elements using a heap (priority queue) without sorting the entire dataset.

**When to Use - STRONG Signals:**
- ✅ Keywords: **"top K"**, **"K largest"**, **"K smallest"**, **"K most frequent"**
- ✅ Need to find K elements from large dataset
- ✅ Don't need full sort, just K elements
- ✅ Streaming data where you maintain top K
- ✅ K-th largest/smallest element queries

**When NOT to Use:**
- ❌ Need all elements sorted
- ❌ K equals N (just sort entire array)
- ❌ Need elements in specific order beyond K
- ❌ Simple problems where sorting is fine (small N)

**Visual Representation:**
```
Finding K=3 Largest Elements from [3, 2, 1, 5, 6, 4]:

Using Min Heap of size K:
Step 1: [3]
Step 2: [2, 3]
Step 3: [1, 2, 3]
Step 4: [5] replaces 1 → [2, 3, 5]
Step 5: [6] replaces 2 → [3, 5, 6]
Step 6: 4 < 3, skip

Result: [3, 5, 6]
```

**Key Insight:**
- For K largest → Use **MIN heap** of size K (remove smallest)
- For K smallest → Use **MAX heap** of size K (remove largest)

### 8.2 Heap Implementation in C

C doesn't have built-in heap, so we implement a simple one:

```c
#include <stdio.h>
#include <stdlib.h>

/**
 * MIN HEAP IMPLEMENTATION
 * Used for finding K largest elements
 */
typedef struct {
    int* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (int*)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int parent(int i) { return (i - 1) / 2; }
int leftChild(int i) { return 2 * i + 1; }
int rightChild(int i) { return 2 * i + 2; }

// Heapify up (used after insertion)
void heapifyUp(MinHeap* heap, int i) {
    while (i > 0 && heap->data[parent(i)] > heap->data[i]) {
        swap(&heap->data[i], &heap->data[parent(i)]);
        i = parent(i);
    }
}

// Heapify down (used after removal)
void heapifyDown(MinHeap* heap, int i) {
    int minIdx = i;
    int left = leftChild(i);
    int right = rightChild(i);
    
    if (left < heap->size && heap->data[left] < heap->data[minIdx]) {
        minIdx = left;
    }
    if (right < heap->size && heap->data[right] < heap->data[minIdx]) {
        minIdx = right;
    }
    
    if (minIdx != i) {
        swap(&heap->data[i], &heap->data[minIdx]);
        heapifyDown(heap, minIdx);
    }
}

// Insert element
void heapPush(MinHeap* heap, int val) {
    if (heap->size == heap->capacity) return;
    
    heap->data[heap->size] = val;
    heapifyUp(heap, heap->size);
    heap->size++;
}

// Remove and return minimum element
int heapPop(MinHeap* heap) {
    if (heap->size == 0) return -1;
    
    int min = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    
    return min;
}

// Get minimum without removing
int heapPeek(MinHeap* heap) {
    return (heap->size > 0) ? heap->data[0] : -1;
}

// Check if heap is full
bool heapIsFull(MinHeap* heap) {
    return heap->size == heap->capacity;
}

void freeHeap(MinHeap* heap) {
    free(heap->data);
    free(heap);
}

/**
 * MAX HEAP IMPLEMENTATION
 * Used for finding K smallest elements
 */
typedef struct {
    int* data;
    int size;
    int capacity;
} MaxHeap;

MaxHeap* createMaxHeap(int capacity) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    heap->data = (int*)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void maxHeapifyUp(MaxHeap* heap, int i) {
    while (i > 0 && heap->data[parent(i)] < heap->data[i]) {
        swap(&heap->data[i], &heap->data[parent(i)]);
        i = parent(i);
    }
}

void maxHeapifyDown(MaxHeap* heap, int i) {
    int maxIdx = i;
    int left = leftChild(i);
    int right = rightChild(i);
    
    if (left < heap->size && heap->data[left] > heap->data[maxIdx]) {
        maxIdx = left;
    }
    if (right < heap->size && heap->data[right] > heap->data[maxIdx]) {
        maxIdx = right;
    }
    
    if (maxIdx != i) {
        swap(&heap->data[i], &heap->data[maxIdx]);
        maxHeapifyDown(heap, maxIdx);
    }
}

void maxHeapPush(MaxHeap* heap, int val) {
    if (heap->size == heap->capacity) return;
    
    heap->data[heap->size] = val;
    maxHeapifyUp(heap, heap->size);
    heap->size++;
}

int maxHeapPop(MaxHeap* heap) {
    if (heap->size == 0) return -1;
    
    int max = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    maxHeapifyDown(heap, 0);
    
    return max;
}

int maxHeapPeek(MaxHeap* heap) {
    return (heap->size > 0) ? heap->data[0] : -1;
}
```

### 8.3 Implementation Examples

#### Example 1: K-th Largest Element in Array

**Problem**: Find the k-th largest element in unsorted array.

```c
/**
 * Kth Largest Element in an Array
 * 
 * Input: nums = [3, 2, 1, 5, 6, 4], k = 2
 * Output: 5 (2nd largest)
 * 
 * Approach: Use min heap of size K
 * Time: O(n log k), Space: O(k)
 */
int findKthLargest(int* nums, int numsSize, int k) {
    // Create min heap of size k
    MinHeap* heap = createMinHeap(k);
    
    // Add first k elements
    for (int i = 0; i < k && i < numsSize; i++) {
        heapPush(heap, nums[i]);
    }
    
    // For remaining elements
    for (int i = k; i < numsSize; i++) {
        // If current element larger than heap min, replace it
        if (nums[i] > heapPeek(heap)) {
            heapPop(heap);
            heapPush(heap, nums[i]);
        }
    }
    
    // The minimum in heap is k-th largest overall
    int result = heapPeek(heap);
    freeHeap(heap);
    
    return result;
}

// Test function
void testFindKthLargest() {
    int nums[] = {3, 2, 1, 5, 6, 4};
    int k = 2;
    int size = sizeof(nums) / sizeof(nums[0]);
    
    printf("Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }
    printf("\nk = %d\n", k);
    
    int result = findKthLargest(nums, size, k);
    printf("The %d-th largest element is: %d\n", k, result);
}
```

**Why Top K Elements Pattern?**
- Need **K-th largest** element
- Don't need full sort
- Heap maintains K largest efficiently

**Algorithm Explanation:**
1. Maintain min heap of size K
2. Heap always contains K largest elements seen so far
3. Minimum element in heap is K-th largest

#### Example 2: Top K Frequent Elements

**Problem**: Find k most frequent elements.

```c
/**
 * Top K Frequent Elements
 * 
 * Input: nums = [1,1,1,2,2,3], k = 2
 * Output: [1, 2]
 * 
 * Time: O(n log k), Space: O(n)
 */

// Structure for frequency counting
typedef struct {
    int num;
    int freq;
} FreqPair;

// Max heap for frequencies
typedef struct {
    FreqPair* data;
    int size;
    int capacity;
} FreqMaxHeap;

FreqMaxHeap* createFreqMaxHeap(int capacity) {
    FreqMaxHeap* heap = (FreqMaxHeap*)malloc(sizeof(FreqMaxHeap));
    heap->data = (FreqPair*)malloc(capacity * sizeof(FreqPair));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void freqSwap(FreqPair* a, FreqPair* b) {
    FreqPair temp = *a;
    *a = *b;
    *b = temp;
}

void freqHeapifyUp(FreqMaxHeap* heap, int i) {
    while (i > 0 && heap->data[parent(i)].freq < heap->data[i].freq) {
        freqSwap(&heap->data[i], &heap->data[parent(i)]);
        i = parent(i);
    }
}

void freqHeapifyDown(FreqMaxHeap* heap, int i) {
    int maxIdx = i;
    int left = leftChild(i);
    int right = rightChild(i);
    
    if (left < heap->size && heap->data[left].freq > heap->data[maxIdx].freq) {
        maxIdx = left;
    }
    if (right < heap->size && heap->data[right].freq > heap->data[maxIdx].freq) {
        maxIdx = right;
    }
    
    if (maxIdx != i) {
        freqSwap(&heap->data[i], &heap->data[maxIdx]);
        freqHeapifyDown(heap, maxIdx);
    }
}

void freqHeapPush(FreqMaxHeap* heap, int num, int freq) {
    if (heap->size == heap->capacity) return;
    
    heap->data[heap->size].num = num;
    heap->data[heap->size].freq = freq;
    freqHeapifyUp(heap, heap->size);
    heap->size++;
}

FreqPair freqHeapPop(FreqMaxHeap* heap) {
    FreqPair max = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    freqHeapifyDown(heap, 0);
    return max;
}

int* topKFrequent(int* nums, int numsSize, int k, int* returnSize) {
    // Step 1: Count frequencies using hash map
    // For simplicity, assume values in range [-1000, 1000]
    int freqMap[2001] = {0};  // Offset by 1000
    
    for (int i = 0; i < numsSize; i++) {
        freqMap[nums[i] + 1000]++;
    }
    
    // Step 2: Build max heap with all unique elements
    FreqMaxHeap* heap = createFreqMaxHeap(numsSize);
    
    for (int i = 0; i < 2001; i++) {
        if (freqMap[i] > 0) {
            freqHeapPush(heap, i - 1000, freqMap[i]);
        }
    }
    
    // Step 3: Extract top k elements
    int* result = (int*)malloc(k * sizeof(int));
    *returnSize = k;
    
    for (int i = 0; i < k && heap->size > 0; i++) {
        FreqPair pair = freqHeapPop(heap);
        result[i] = pair.num;
    }
    
    free(heap->data);
    free(heap);
    
    return result;
}

// Test function
void testTopKFrequent() {
    int nums[] = {1, 1, 1, 2, 2, 3};
    int k = 2;
    int size = sizeof(nums) / sizeof(nums[0]);
    int returnSize;
    
    printf("Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }
    printf("\nk = %d\n", k);
    
    int* result = topKFrequent(nums, size, k, &returnSize);
    
    printf("Top %d frequent elements: ", k);
    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    free(result);
}
```

**Why Top K Pattern?**
- Need **K most frequent** elements
- Heap naturally maintains ordering
- More efficient than full sort

#### Example 3: K Closest Points to Origin

**Problem**: Find K closest points to origin (0, 0).

```c
#include <math.h>

/**
 * K Closest Points to Origin
 * 
 * Input: points = [[1,3], [-2,2]], k = 1
 * Output: [[-2,2]]
 * 
 * Distance = sqrt(x^2 + y^2)
 * 
 * Time: O(n log k), Space: O(k)
 */

typedef struct {
    int x;
    int y;
    int distSquared;  // Store squared distance to avoid sqrt
} Point;

typedef struct {
    Point* data;
    int size;
    int capacity;
} MaxDistHeap;

MaxDistHeap* createMaxDistHeap(int capacity) {
    MaxDistHeap* heap = (MaxDistHeap*)malloc(sizeof(MaxDistHeap));
    heap->data = (Point*)malloc(capacity * sizeof(Point));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void pointSwap(Point* a, Point* b) {
    Point temp = *a;
    *a = *b;
    *b = temp;
}

void maxDistHeapifyUp(MaxDistHeap* heap, int i) {
    while (i > 0 && heap->data[parent(i)].distSquared < heap->data[i].distSquared) {
        pointSwap(&heap->data[i], &heap->data[parent(i)]);
        i = parent(i);
    }
}

void maxDistHeapifyDown(MaxDistHeap* heap, int i) {
    int maxIdx = i;
    int left = leftChild(i);
    int right = rightChild(i);
    
    if (left < heap->size && 
        heap->data[left].distSquared > heap->data[maxIdx].distSquared) {
        maxIdx = left;
    }
    if (right < heap->size && 
        heap->data[right].distSquared > heap->data[maxIdx].distSquared) {
        maxIdx = right;
    }
    
    if (maxIdx != i) {
        pointSwap(&heap->data[i], &heap->data[maxIdx]);
        maxDistHeapifyDown(heap, maxIdx);
    }
}

void maxDistHeapPush(MaxDistHeap* heap, int x, int y) {
    if (heap->size == heap->capacity) return;
    
    heap->data[heap->size].x = x;
    heap->data[heap->size].y = y;
    heap->data[heap->size].distSquared = x * x + y * y;
    maxDistHeapifyUp(heap, heap->size);
    heap->size++;
}

Point maxDistHeapPop(MaxDistHeap* heap) {
    Point max = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    maxDistHeapifyDown(heap, 0);
    return max;
}

Point maxDistHeapPeek(MaxDistHeap* heap) {
    return heap->data[0];
}

int** kClosest(int** points, int pointsSize, int* pointsColSize, 
               int k, int* returnSize, int** returnColumnSizes) {
    
    // Use max heap of size k (for k closest, we remove farthest)
    MaxDistHeap* heap = createMaxDistHeap(k);
    
    // Process each point
    for (int i = 0; i < pointsSize; i++) {
        int x = points[i][0];
        int y = points[i][1];
        int distSq = x * x + y * y;
        
        if (heap->size < k) {
            // Heap not full, add point
            maxDistHeapPush(heap, x, y);
        } else {
            // If current point closer than farthest in heap
            if (distSq < maxDistHeapPeek(heap).distSquared) {
                maxDistHeapPop(heap);
                maxDistHeapPush(heap, x, y);
            }
        }
    }
    
    // Extract result
    int** result = (int**)malloc(k * sizeof(int*));
    *returnColumnSizes = (int*)malloc(k * sizeof(int));
    *returnSize = k;
    
    for (int i = 0; i < k; i++) {
        result[i] = (int*)malloc(2 * sizeof(int));
        (*returnColumnSizes)[i] = 2;
    }
    
    // Pop all points from heap
    for (int i = k - 1; i >= 0; i--) {
        Point p = maxDistHeapPop(heap);
        result[i][0] = p.x;
        result[i][1] = p.y;
    }
    
    free(heap->data);
    free(heap);
    
    return result;
}

// Test function
void testKClosest() {
    int** points = (int**)malloc(3 * sizeof(int*));
    points[0] = (int*)malloc(2 * sizeof(int));
    points[0][0] = 1; points[0][1] = 3;
    points[1] = (int*)malloc(2 * sizeof(int));
    points[1][0] = -2; points[1][1] = 2;
    points[2] = (int*)malloc(2 * sizeof(int));
    points[2][0] = 5; points[2][1] = 8;
    
    int k = 2;
    int returnSize;
    int* returnColumnSizes;
    int colSize = 2;
    
    printf("Points: [1,3] [-2,2] [5,8]\n");
    printf("k = %d\n", k);
    
    int** result = kClosest(points, 3, &colSize, k, 
                           &returnSize, &returnColumnSizes);
    
    printf("K closest points: ");
    for (int i = 0; i < returnSize; i++) {
        printf("[%d,%d] ", result[i][0], result[i][1]);
    }
    printf("\n");
}
```

**Why Top K Pattern?**
- Finding **K closest** points
- Max heap of size K maintains K closest
- More efficient than sorting all points

#### Example 4: Kth Smallest Element in Sorted Matrix

**Problem**: Find k-th smallest element in n x n matrix where each row and column is sorted.

```c
/**
 * Kth Smallest Element in a Sorted Matrix
 * 
 * Input: matrix = [[1,5,9],
 *                  [10,11,13],
 *                  [12,13,15]], k = 8
 * Output: 13
 * 
 * Time: O(k log n), Space: O(n)
 */

typedef struct {
    int val;
    int row;
    int col;
} MatrixElement;

typedef struct {
    MatrixElement* data;
    int size;
    int capacity;
} MinMatrixHeap;

MinMatrixHeap* createMinMatrixHeap(int capacity) {
    MinMatrixHeap* heap = (MinMatrixHeap*)malloc(sizeof(MinMatrixHeap));
    heap->data = (MatrixElement*)malloc(capacity * sizeof(MatrixElement));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void matrixSwap(MatrixElement* a, MatrixElement* b) {
    MatrixElement temp = *a;
    *a = *b;
    *b = temp;
}

void minMatrixHeapifyUp(MinMatrixHeap* heap, int i) {
    while (i > 0 && heap->data[parent(i)].val > heap->data[i].val) {
        matrixSwap(&heap->data[i], &heap->data[parent(i)]);
        i = parent(i);
    }
}

void minMatrixHeapifyDown(MinMatrixHeap* heap, int i) {
    int minIdx = i;
    int left = leftChild(i);
    int right = rightChild(i);
    
    if (left < heap->size && heap->data[left].val < heap->data[minIdx].val) {
        minIdx = left;
    }
    if (right < heap->size && heap->data[right].val < heap->data[minIdx].val) {
        minIdx = right;
    }
    
    if (minIdx != i) {
        matrixSwap(&heap->data[i], &heap->data[minIdx]);
        minMatrixHeapifyDown(heap, minIdx);
    }
}

void minMatrixHeapPush(MinMatrixHeap* heap, int val, int row, int col) {
    if (heap->size == heap->capacity) return;
    
    heap->data[heap->size].val = val;
    heap->data[heap->size].row = row;
    heap->data[heap->size].col = col;
    minMatrixHeapifyUp(heap, heap->size);
    heap->size++;
}

MatrixElement minMatrixHeapPop(MinMatrixHeap* heap) {
    MatrixElement min = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    minMatrixHeapifyDown(heap, 0);
    return min;
}

int kthSmallest(int** matrix, int matrixSize, int* matrixColSize, int k) {
    int n = matrixSize;
    
    // Create min heap and add first element from each row
    MinMatrixHeap* heap = createMinMatrixHeap(n);
    
    for (int i = 0; i < n; i++) {
        minMatrixHeapPush(heap, matrix[i][0], i, 0);
    }
    
    // Extract min k times
    MatrixElement elem;
    for (int i = 0; i < k; i++) {
        elem = minMatrixHeapPop(heap);
        
        // If there's a next element in the same row, add it
        if (elem.col + 1 < n) {
            minMatrixHeapPush(heap, 
                            matrix[elem.row][elem.col + 1],
                            elem.row,
                            elem.col + 1);
        }
    }
    
    free(heap->data);
    free(heap);
    
    return elem.val;
}

// Test function
void testKthSmallestMatrix() {
    int** matrix = (int**)malloc(3 * sizeof(int*));
    matrix[0] = (int*)malloc(3 * sizeof(int));
    matrix[0][0] = 1; matrix[0][1] = 5; matrix[0][2] = 9;
    matrix[1] = (int*)malloc(3 * sizeof(int));
    matrix[1][0] = 10; matrix[1][1] = 11; matrix[1][2] = 13;
    matrix[2] = (int*)malloc(3 * sizeof(int));
    matrix[2][0] = 12; matrix[2][1] = 13; matrix[2][2] = 15;
    
    int k = 8;
    int colSize = 3;
    
    printf("Matrix:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("k = %d\n", k);
    
    int result = kthSmallest(matrix, 3, &colSize, k);
    printf("The %d-th smallest element is: %d\n", k, result);
}
```

**Why Top K Pattern?**
- Finding **K-th smallest** in matrix
- Min heap processes elements in sorted order
- Efficient: O(k log n) vs O(n² log n²) for full sort

### 8.4 Decision Checklist for Top K Elements

```
Use Top K Elements IF:
☑ Keywords: "top k", "k largest", "k smallest", "k most/least frequent"
☑ Don't need full sort, just K elements
☑ K is significantly smaller than N
☑ Streaming data or online algorithm
☑ Need to maintain K elements efficiently

DON'T Use Top K Elements IF:
☒ Need all elements sorted
☒ K equals N (just sort)
☒ Simple problem where sorting is acceptable
☒ Need multiple queries on different K values
```

### 8.5 Practice Problems

#### Beginner Level

1. **Kth Largest Element in an Array**
   - Input: `nums = [3,2,1,5,6,4]`, k = 2
   - Output: `5`
   - Hint: Min heap of size k

2. **Kth Smallest Element in a Sorted Matrix**
   - Input: Matrix with sorted rows/cols, k
   - Output: k-th smallest value
   - Hint: Min heap with matrix elements

3. **Last Stone Weight**
   - Input: `stones = [2,7,4,1,8,1]`
   - Output: `1` (result after smashing)
   - Hint: Max heap, extract two largest

4. **Reorganize String**
   - Input: `s = "aab"`
   - Output: `"aba"` (no adjacent same chars)
   - Hint: Max heap by frequency

5. **Sort Characters By Frequency**
   - Input: `s = "tree"`
   - Output: `"eert"` or `"eetr"`
   - Hint: Frequency count + max heap

#### Intermediate Level

6. **Top K Frequent Elements**
   - Input: `nums = [1,1,1,2,2,3]`, k = 2
   - Output: `[1,2]`
   - Hint: Frequency map + heap

7. **K Closest Points to Origin**
   - Input: `points = [[1,3],[-2,2]]`, k = 1
   - Output: `[[-2,2]]`
   - Hint: Max heap by distance

8. **Find K Pairs with Smallest Sums**
   - Input: Two sorted arrays, k
   - Output: k pairs with smallest sums
   - Hint: Min heap with pairs

9. **Kth Largest Element in a Stream**
   - Input: Stream of numbers, find k-th largest after each
   - Output: k-th largest at each step
   - Hint: Min heap of size k maintained

10. **Ugly Number II**
    - Input: n
    - Output: n-th ugly number (factors only 2, 3, 5)
    - Hint: Min heap generating ugly numbers

#### Advanced Level

11. **Find Median from Data Stream**
    - Input: Stream of numbers
    - Output: Median after each number
    - Hint: Two heaps (max for left half, min for right half)

12. **Sliding Window Median**
    - Input: `nums`, k
    - Output: Median of each window of size k
    - Hint: Two heaps + removal logic

13. **Meeting Rooms III**
    - Input: Meetings, number of rooms
    - Output: Room that hosted most meetings
    - Hint: Min heap for available rooms

14. **Maximum Performance of a Team**
    - Input: Engineers with speed/efficiency, k team size
    - Output: Maximum performance
    - Hint: Sort by efficiency + min heap for speeds

15. **IPO** (Capital)
    - Input: Projects with capital/profit, k projects to select
    - Output: Maximum capital after k projects
    - Hint: Two heaps (available projects + best profit)

---

## Section 9: Breadth-First Search (BFS)

### 9.1 Pattern Overview

**Core Concept**: Explore nodes level by level, visiting all neighbors before going deeper. Uses a queue (FIFO).

**When to Use - STRONG Signals:**
- ✅ Keywords: **"level order"**, **"shortest path"**, **"minimum steps"**
- ✅ Tree: level-by-level traversal
- ✅ Graph: shortest path in unweighted graph
- ✅ **Layer-by-layer** processing needed
- ✅ Find **nearest/closest** nodes

**When NOT to Use:**
- ❌ Need to explore all paths (use DFS)
- ❌ Weighted graph shortest path (use Dijkstra)
- ❌ Simple tree traversal where order doesn't matter

**Visual Representation:**
```
Tree BFS:
        1
       / \
      2   3
     / \   \
    4   5   6

Level 0: [1]
Level 1: [2, 3]
Level 2: [4, 5, 6]

Output: [1, 2, 3, 4, 5, 6]

Graph BFS (shortest path):
Start → A → B → Goal
        ↓   ↓
        C → D

BFS explores all distance-1 nodes before distance-2 nodes
```

### 9.2 Core Template

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * TREE NODE STRUCTURE
 */
struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

/**
 * QUEUE IMPLEMENTATION FOR BFS
 */
typedef struct QueueNode {
    struct TreeNode* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

bool isQueueEmpty(Queue* q) {
    return q->size == 0;
}

void enqueue(Queue* q, struct TreeNode* node) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->treeNode = node;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

struct TreeNode* dequeue(Queue* q) {
    if (isQueueEmpty(q)) return NULL;
    
    QueueNode* temp = q->front;
    struct TreeNode* node = temp->treeNode;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return node;
}

void freeQueue(Queue* q) {
    while (!isQueueEmpty(q)) {
        dequeue(q);
    }
    free(q);
}

/**
 * BASIC BFS TEMPLATE
 */
void bfsTraversal(struct TreeNode* root) {
    if (root == NULL) return;
    
    Queue* q = createQueue();
    enqueue(q, root);
    
    while (!isQueueEmpty(q)) {
        struct TreeNode* current = dequeue(q);
        
        // Process current node
        printf("%d ", current->val);
        
        // Add children to queue
        if (current->left) {
            enqueue(q, current->left);
        }
        if (current->right) {
            enqueue(q, current->right);
        }
    }
    
    freeQueue(q);
}

/**
 * LEVEL-BY-LEVEL BFS TEMPLATE
 */
void bfsLevelOrder(struct TreeNode* root) {
    if (root == NULL) return;
    
    Queue* q = createQueue();
    enqueue(q, root);
    
    while (!isQueueEmpty(q)) {
        int levelSize = q->size;  // Number of nodes at current level
        
        // Process all nodes at current level
        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* current = dequeue(q);
            printf("%d ", current->val);
            
            // Add children for next level
            if (current->left) {
                enqueue(q, current->left);
            }
            if (current->right) {
                enqueue(q, current->right);
            }
        }
        printf("\n");  // New line after each level
    }
    
    freeQueue(q);
}
```

### 9.3 Implementation Examples

#### Example 1: Binary Tree Level Order Traversal

**Problem**: Return level order traversal of tree as list of lists.

```c
/**
 * Binary Tree Level Order Traversal
 * 
 * Input:     3
 *           / \
 *          9  20
 *            /  \
 *           15   7
 * 
 * Output: [[3], [9,20], [15,7]]
 * 
 * Time: O(n), Space: O(w) where w is max width
 */
int** levelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
    if (root == NULL) {
        *returnSize = 0;
        return NULL;
    }
    
    // Allocate result array (max levels = tree height, worst case n levels)
    int** result = (int**)malloc(1000 * sizeof(int*));
    *returnColumnSizes = (int*)malloc(1000 * sizeof(int));
    *returnSize = 0;
    
    Queue* q = createQueue();
    enqueue(q, root);
    
    while (!isQueueEmpty(q)) {
        int levelSize = q->size;
        
        // Allocate array for current level
        result[*returnSize] = (int*)malloc(levelSize * sizeof(int));
        (*returnColumnSizes)[*returnSize] = levelSize;
        
        // Process all nodes at current level
        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* current = dequeue(q);
            result[*returnSize][i] = current->val;
            
            if (current->left) enqueue(q, current->left);
            if (current->right) enqueue(q, current->right);
        }
        
        (*returnSize)++;
    }
    
    freeQueue(q);
    return result;
}
```

**Why BFS?**
- Need **LEVEL ORDER** traversal → Perfect BFS use case
- Process nodes **layer by layer**
- Queue naturally maintains level structure

#### Example 2: Zigzag Level Order Traversal

**Problem**: Level order but alternating left-to-right and right-to-left.

```c
/**
 * Binary Tree Zigzag Level Order Traversal
 * 
 * Input:     3
 *           / \
 *          9  20
 *            /  \
 *           15   7
 * 
 * Output: [[3], [20,9], [15,7]]
 * 
 * Time: O(n), Space: O(w)
 */
int** zigzagLevelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
    if (root == NULL) {
        *returnSize = 0;
        return NULL;
    }
    
    int** result = (int**)malloc(1000 * sizeof(int*));
    *returnColumnSizes = (int*)malloc(1000 * sizeof(int));
    *returnSize = 0;
    
    Queue* q = createQueue();
    enqueue(q, root);
    bool leftToRight = true;
    
    while (!isQueueEmpty(q)) {
        int levelSize = q->size;
        result[*returnSize] = (int*)malloc(levelSize * sizeof(int));
        (*returnColumnSizes)[*returnSize] = levelSize;
        
        // Store level nodes
        int* levelNodes = (int*)malloc(levelSize * sizeof(int));
        
        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* current = dequeue(q);
            levelNodes[i] = current->val;
            
            if (current->left) enqueue(q, current->left);
            if (current->right) enqueue(q, current->right);
        }
        
        // Copy to result based on direction
        if (leftToRight) {
            for (int i = 0; i < levelSize; i++) {
                result[*returnSize][i] = levelNodes[i];
            }
        } else {
            for (int i = 0; i < levelSize; i++) {
                result[*returnSize][i] = levelNodes[levelSize - 1 - i];
            }
        }
        
        free(levelNodes);
        leftToRight = !leftToRight;
        (*returnSize)++;
    }
    
    freeQueue(q);
    return result;
}
```

**Why BFS with modification?**
- Still **LEVEL ORDER** traversal
- Additional logic for zigzag direction
- BFS structure maintained

#### Example 3: Minimum Depth of Binary Tree

**Problem**: Find shortest path from root to leaf.

```c
/**
 * Minimum Depth of Binary Tree
 * 
 * Input:     3
 *           / \
 *          9  20
 *            /  \
 *           15   7
 * 
 * Output: 2 (path 3 → 9)
 * 
 * Time: O(n), Space: O(w)
 */
int minDepth(struct TreeNode* root) {
    if (root == NULL) return 0;
    
    Queue* q = createQueue();
    enqueue(q, root);
    int depth = 1;
    
    while (!isQueueEmpty(q)) {
        int levelSize = q->size;
        
        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* current = dequeue(q);
            
            // Check if leaf node
            if (current->left == NULL && current->right == NULL) {
                freeQueue(q);
                return depth;  // First leaf found is minimum depth
            }
            
            if (current->left) enqueue(q, current->left);
            if (current->right) enqueue(q, current->right);
        }
        
        depth++;
    }
    
    freeQueue(q);
    return depth;
}
```

**Why BFS?**
- Need **SHORTEST PATH** to leaf
- BFS guarantees first leaf found is closest
- DFS would need to explore all paths

#### Example 4: Binary Tree Right Side View

**Problem**: Return values visible from right side of tree.

```c
/**
 * Binary Tree Right Side View
 * 
 * Input:     1
 *           / \
 *          2   3
 *           \   \
 *            5   4
 * 
 * Output: [1, 3, 4] (rightmost node at each level)
 * 
 * Time: O(n), Space: O(w)
 */
int* rightSideView(struct TreeNode* root, int* returnSize) {
    if (root == NULL) {
        *returnSize = 0;
        return NULL;
    }
    
    int* result = (int*)malloc(1000 * sizeof(int));
    *returnSize = 0;
    
    Queue* q = createQueue();
    enqueue(q, root);
    
    while (!isQueueEmpty(q)) {
        int levelSize = q->size;
        
        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* current = dequeue(q);
            
            // Last node of the level is the rightmost
            if (i == levelSize - 1) {
                result[*returnSize] = current->val;
                (*returnSize)++;
            }
            
            if (current->left) enqueue(q, current->left);
            if (current->right) enqueue(q, current->right);
        }
    }
    
    freeQueue(q);
    return result;
}
```

**Why BFS?**
- Need to identify **RIGHTMOST** node per level
- Level-by-level processing required
- Last node in each level is the answer

#### Example 5: Word Ladder (Graph BFS)

**Problem**: Shortest transformation sequence from beginWord to endWord.

```c
/**
 * Word Ladder
 * 
 * Input: beginWord = "hit", endWord = "cog"
 *        wordList = ["hot","dot","dog","lot","log","cog"]
 * Output: 5 ("hit" → "hot" → "dot" → "dog" → "cog")
 * 
 * Time: O(M^2 * N) where M = word length, N = wordList size
 * Space: O(M * N)
 */

// Check if two words differ by exactly one character
bool isOneCharDiff(char* word1, char* word2) {
    int diff = 0;
    int len = strlen(word1);
    
    for (int i = 0; i < len; i++) {
        if (word1[i] != word2[i]) {
            diff++;
            if (diff > 1) return false;
        }
    }
    
    return diff == 1;
}

typedef struct WordQueueNode {
    char* word;
    int level;
    struct WordQueueNode* next;
} WordQueueNode;

typedef struct {
    WordQueueNode* front;
    WordQueueNode* rear;
} WordQueue;

WordQueue* createWordQueue() {
    WordQueue* q = (WordQueue*)malloc(sizeof(WordQueue));
    q->front = q->rear = NULL;
    return q;
}

void enqueueWord(WordQueue* q, char* word, int level) {
    WordQueueNode* newNode = (WordQueueNode*)malloc(sizeof(WordQueueNode));
    newNode->word = word;
    newNode->level = level;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

WordQueueNode* dequeueWord(WordQueue* q) {
    if (q->front == NULL) return NULL;
    
    WordQueueNode* temp = q->front;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    return temp;
}

bool isWordQueueEmpty(WordQueue* q) {
    return q->front == NULL;
}

int ladderLength(char* beginWord, char* endWord, char** wordList, int wordListSize) {
    // Check if endWord exists in wordList
    bool endWordExists = false;
    for (int i = 0; i < wordListSize; i++) {
        if (strcmp(wordList[i], endWord) == 0) {
            endWordExists = true;
            break;
        }
    }
    if (!endWordExists) return 0;
    
    // Track visited words
    bool* visited = (bool*)calloc(wordListSize, sizeof(bool));
    
    // BFS
    WordQueue* q = createWordQueue();
    enqueueWord(q, beginWord, 1);
    
    while (!isWordQueueEmpty(q)) {
        WordQueueNode* node = dequeueWord(q);
        char* currentWord = node->word;
        int currentLevel = node->level;
        
        // Check if we reached endWord
        if (strcmp(currentWord, endWord) == 0) {
            free(node);
            free(visited);
            // Free queue
            while (!isWordQueueEmpty(q)) {
                free(dequeueWord(q));
            }
            free(q);
            return currentLevel;
        }
        
        // Try all words in wordList
        for (int i = 0; i < wordListSize; i++) {
            if (!visited[i] && isOneCharDiff(currentWord, wordList[i])) {
                visited[i] = true;
                enqueueWord(q, wordList[i], currentLevel + 1);
            }
        }
        
        free(node);
    }
    
    free(visited);
    free(q);
    return 0;  // No path found
}
```

**Why BFS for Graph?**
- Need **SHORTEST PATH** in unweighted graph
- BFS guarantees shortest path found first
- Each word is a node, edges connect words differing by 1 char

### 9.4 Decision Checklist for BFS

```
Use BFS IF:
☑ Need level-by-level/layer-by-layer processing
☑ Shortest path in unweighted graph
☑ Minimum steps/moves required
☑ Tree level order traversal
☑ Finding nearest/closest nodes
☑ Keywords: "level", "shortest", "minimum", "nearest"

DON'T Use BFS IF:
☒ Weighted graph (use Dijkstra/Bellman-Ford)
☒ Need all paths (use DFS)
☒ Memory constrained and tree is very wide (use DFS)
☒ Just need any path, not shortest (DFS might be simpler)
```

### 9.5 Practice Problems

#### Beginner Level

1. **Binary Tree Level Order Traversal**
   - Input: Tree root
   - Output: List of lists (each level)
   - Hint: Queue, process level by level

2. **Minimum Depth of Binary Tree**
   - Input: Tree root
   - Output: Shortest path to leaf
   - Hint: Return depth when first leaf found

3. **Binary Tree Right Side View**
   - Input: Tree root
   - Output: Rightmost values at each level
   - Hint: Last node in each level

4. **Average of Levels in Binary Tree**
   - Input: Tree root
   - Output: Average value at each level
   - Hint: Sum and count per level

5. **N-ary Tree Level Order Traversal**
   - Input: N-ary tree root
   - Output: Level order traversal
   - Hint: Same as binary, but multiple children

#### Intermediate Level

6. **Zigzag Level Order Traversal**
   - Input: Tree root
   - Output: Alternating left-right level order
   - Hint: Toggle direction each level

7. **Binary Tree Level Order Traversal II** (Bottom-up)
   - Input: Tree root
   - Output: Level order from bottom to top
   - Hint: BFS then reverse result

8. **Populating Next Right Pointers**
   - Input: Perfect binary tree
   - Output: Connect nodes at same level
   - Hint: Level order, connect within each level

9. **Symmetric Tree**
   - Input: Tree root
   - Output: Check if mirror of itself
   - Hint: BFS with two queues (left subtree, right subtree)

10. **Rotting Oranges**
    - Input: Grid with fresh/rotten oranges
    - Output: Minutes until all oranges rotten
    - Hint: Multi-source BFS from all rotten oranges

#### Advanced Level

11. **Word Ladder**
    - Input: Begin word, end word, word list
    - Output: Shortest transformation sequence
    - Hint: BFS where each word is a node

12. **Walls and Gates**
    - Input: Grid with gates and obstacles
    - Output: Distance to nearest gate for each cell
    - Hint: Multi-source BFS from all gates

13. **Shortest Path in Binary Matrix**
    - Input: Binary matrix (0 = passable, 1 = blocked)
    - Output: Shortest clear path from top-left to bottom-right
    - Hint: BFS with 8-directional movement

14. **Clone Graph**
    - Input: Graph node
    - Output: Deep copy of graph
    - Hint: BFS with hash map to track cloned nodes

15. **Pacific Atlantic Water Flow**
    - Input: Matrix of heights
    - Output: Cells where water flows to both oceans
    - Hint: Two BFS from each ocean boundary

---

## Section 10: Depth-First Search (DFS)

### 10.1 Pattern Overview

**Core Concept**: Explore as deep as possible before backtracking. Uses recursion or stack (LIFO).

**When to Use - STRONG Signals:**
- ✅ Keywords: **"all paths"**, **"all combinations"**, **"explore fully"**
- ✅ Tree traversal (preorder, inorder, postorder)
- ✅ Graph: find **any path** or **all paths**
- ✅ **Exhaustive search** needed
- ✅ Detect cycles in directed graphs
- ✅ Topological sort

**When NOT to Use:**
- ❌ Need shortest path (use BFS)
- ❌ Level-by-level processing (use BFS)
- ❌ Very deep recursion might cause stack overflow

**Visual Representation:**
```
Tree DFS:
        1
       / \
      2   3
     / \
    4   5

Preorder (Root, Left, Right):  1, 2, 4, 5, 3
Inorder (Left, Root, Right):   4, 2, 5, 1, 3
Postorder (Left, Right, Root): 4, 5, 2, 3, 1

Graph DFS:
A → B → D
↓   ↓
C → E

DFS from A: A → B → D (backtrack) → E (backtrack) → C
```

### 10.2 Core Templates

```c
/**
 * DFS RECURSIVE TEMPLATE
 */
void dfsRecursive(struct TreeNode* node) {
    if (node == NULL) return;
    
    // Preorder: Process node before children
    printf("%d ", node->val);
    
    // Recurse on children
    dfsRecursive(node->left);
    dfsRecursive(node->right);
    
    // Postorder: Process node after children
    // printf("%d ", node->val);
}

/**
 * DFS ITERATIVE TEMPLATE (Using Stack)
 */
void dfsIterative(struct TreeNode* root) {
    if (root == NULL) return;
    
    // Stack implementation
    struct TreeNode** stack = (struct TreeNode**)malloc(1000 * sizeof(struct TreeNode*));
    int top = -1;
    
    // Push root
    stack[++top] = root;
    
    while (top >= 0) {
        // Pop node
        struct TreeNode* current = stack[top--];
        printf("%d ", current->val);
        
        // Push right first (so left is processed first)
        if (current->right) {
            stack[++top] = current->right;
        }
        if (current->left) {
            stack[++top] = current->left;
        }
    }
    
    free(stack);
}

/**
 * DFS WITH PATH TRACKING
 */
void dfsWithPath(struct TreeNode* node, int* path, int pathLen) {
    if (node == NULL) return;
    
    // Add current node to path
    path[pathLen] = node->val;
    pathLen++;
    
    // If leaf node, process path
    if (node->left == NULL && node->right == NULL) {
        printf("Path: ");
        for (int i = 0; i < pathLen; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    }
    
    // Recurse
    dfsWithPath(node->left, path, pathLen);
    dfsWithPath(node->right, path, pathLen);
    
    // Backtrack (path automatically backtracks due to local pathLen)
}

/**
 * GRAPH DFS TEMPLATE
 */
void graphDFS(int node, int** graph, int* graphColSize, bool* visited, int n) {
    visited[node] = true;
    printf("%d ", node);
    
    // Visit all neighbors
    for (int i = 0; i < graphColSize[node]; i++) {
        int neighbor = graph[node][i];
        if (!visited[neighbor]) {
            graphDFS(neighbor, graph, graphColSize, visited, n);
        }
    }
}
```

### 10.3 Implementation Examples

#### Example 1: Binary Tree Paths

**Problem**: Find all root-to-leaf paths.

```c
/**
 * Binary Tree Paths
 * 
 * Input:     1
 *           / \
 *          2   3
 *           \
 *            5
 * 
 * Output: ["1->2->5", "1->3"]
 * 
 * Time: O(n), Space: O(h) where h is height
 */

void dfsPathsHelper(struct TreeNode* node, char** result, int* returnSize,
                    char* currentPath, int pathLen) {
    if (node == NULL) return;
    
    // Add current node to path
    int len = pathLen;
    if (len > 0) {
        len += sprintf(currentPath + len, "->");
    }
    len += sprintf(currentPath + len, "%d", node->val);
    
    // If leaf, add path to result
    if (node->left == NULL && node->right == NULL) {
        result[*returnSize] = (char*)malloc((len + 1) * sizeof(char));
        strcpy(result[*returnSize], currentPath);
        (*returnSize)++;
    } else {
        // Recurse on children
        dfsPathsHelper(node->left, result, returnSize, currentPath, len);
        dfsPathsHelper(node->right, result, returnSize, currentPath, len);
    }
}

char** binaryTreePaths(struct TreeNode* root, int* returnSize) {
    *returnSize = 0;
    if (root == NULL) return NULL;
    
    char** result = (char**)malloc(1000 * sizeof(char*));
    char* currentPath = (char*)malloc(1000 * sizeof(char));
    
    dfsPathsHelper(root, result, returnSize, currentPath, 0);
    
    free(currentPath);
    return result;
}
```

**Why DFS?**
- Need **ALL PATHS** from root to leaves
- DFS explores each path completely
- Natural recursion matches tree structure

#### Example 2: Path Sum II

**Problem**: Find all root-to-leaf paths with target sum.

```c
/**
 * Path Sum II
 * 
 * Input: root = [5,4,8,11,null,13,4,7,2,null,null,5,1], targetSum = 22
 * Output: [[5,4,11,2], [5,8,4,5]]
 * 
 * Time: O(n), Space: O(h)
 */

void pathSumHelper(struct TreeNode* node, int targetSum, int* path, int pathLen,
                   int** result, int* returnSize, int** returnColumnSizes) {
    if (node == NULL) return;
    
    // Add current node to path
    path[pathLen] = node->val;
    pathLen++;
    targetSum -= node->val;
    
    // If leaf and sum matches
    if (node->left == NULL && node->right == NULL && targetSum == 0) {
        result[*returnSize] = (int*)malloc(pathLen * sizeof(int));
        for (int i = 0; i < pathLen; i++) {
            result[*returnSize][i] = path[i];
        }
        (*returnColumnSizes)[*returnSize] = pathLen;
        (*returnSize)++;
    }
    
    // Recurse
    pathSumHelper(node->left, targetSum, path, pathLen, result, returnSize, returnColumnSizes);
    pathSumHelper(node->right, targetSum, path, pathLen, result, returnSize, returnColumnSizes);
    
    // Backtrack happens automatically (pathLen is local)
}

int** pathSum(struct TreeNode* root, int targetSum, int* returnSize, int** returnColumnSizes) {
    *returnSize = 0;
    int** result = (int**)malloc(1000 * sizeof(int*));
    *returnColumnSizes = (int*)malloc(1000 * sizeof(int));
    int* path = (int*)malloc(1000 * sizeof(int));
    
    pathSumHelper(root, targetSum, path, 0, result, returnSize, returnColumnSizes);
    
    free(path);
    return result;
}
```

**Why DFS?**
- Need **ALL PATHS** with specific property
- DFS naturally tracks cumulative sum
- Backtracking built into recursion

#### Example 3: Number of Islands

**Problem**: Count number of islands in 2D grid.

```c
/**
 * Number of Islands
 * 
 * Input: grid = [
 *   ["1","1","0","0","0"],
 *   ["1","1","0","0","0"],
 *   ["0","0","1","0","0"],
 *   ["0","0","0","1","1"]
 * ]
 * Output: 3
 * 
 * Time: O(m*n), Space: O(m*n) for recursion
 */

void dfsIsland(char** grid, int gridSize, int gridColSize, int i, int j) {
    // Boundary check and water check
    if (i < 0 || i >= gridSize || j < 0 || j >= gridColSize || grid[i][j] == '0') {
        return;
    }
    
    // Mark as visited by changing to '0'
    grid[i][j] = '0';
    
    // Explore all 4 directions
    dfsIsland(grid, gridSize, gridColSize, i + 1, j);  // Down
    dfsIsland(grid, gridSize, gridColSize, i - 1, j);  // Up
    dfsIsland(grid, gridSize, gridColSize, i, j + 1);  // Right
    dfsIsland(grid, gridSize, gridColSize, i, j - 1);  // Left
}

int numIslands(char** grid, int gridSize, int* gridColSize) {
    if (gridSize == 0) return 0;
    
    int count = 0;
    int cols = gridColSize[0];
    
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == '1') {
                count++;
                dfsIsland(grid, gridSize, cols, i, j);  // Sink the island
            }
        }
    }
    
    return count;
}
```

**Why DFS?**
- Need to explore **ALL CONNECTED** land cells
- DFS naturally explores connected components
- Simpler than BFS for this problem

#### Example 4: Course Schedule (Cycle Detection)

**Problem**: Detect if courses can be finished (no cycle in dependency graph).

```c
/**
 * Course Schedule (Cycle Detection in Directed Graph)
 * 
 * Input: numCourses = 2, prerequisites = [[1,0]]
 * Output: true (can finish: take 0 then 1)
 * 
 * Input: numCourses = 2, prerequisites = [[1,0],[0,1]]
 * Output: false (circular dependency)
 * 
 * Time: O(V + E), Space: O(V + E)
 */

// States: 0 = unvisited, 1 = visiting, 2 = visited
bool hasCycleDFS(int course, int** graph, int* graphColSize, int* state) {
    if (state[course] == 1) return true;   // Visiting again = cycle
    if (state[course] == 2) return false;  // Already processed
    
    state[course] = 1;  // Mark as visiting
    
    // Check all prerequisites
    for (int i = 0; i < graphColSize[course]; i++) {
        int prereq = graph[course][i];
        if (hasCycleDFS(prereq, graph, graphColSize, state)) {
            return true;
        }
    }
    
    state[course] = 2;  // Mark as visited
    return false;
}

bool canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {
    // Build adjacency list
    int** graph = (int**)calloc(numCourses, sizeof(int*));
    int* graphColSize = (int*)calloc(numCourses, sizeof(int));
    int* graphCapacity = (int*)calloc(numCourses, sizeof(int));
    
    for (int i = 0; i < prerequisitesSize; i++) {
        int course = prerequisites[i][0];
        int prereq = prerequisites[i][1];
        
        if (graphColSize[course] == graphCapacity[course]) {
            graphCapacity[course] = graphCapacity[course] == 0 ? 4 : graphCapacity[course] * 2;
            graph[course] = (int*)realloc(graph[course], graphCapacity[course] * sizeof(int));
        }
        
        graph[course][graphColSize[course]++] = prereq;
    }
    
    // DFS to detect cycle
    int* state = (int*)calloc(numCourses, sizeof(int));
    
    for (int i = 0; i < numCourses; i++) {
        if (state[i] == 0) {  // Unvisited
            if (hasCycleDFS(i, graph, graphColSize, state)) {
                // Cleanup
                for (int j = 0; j < numCourses; j++) {
                    free(graph[j]);
                }
                free(graph);
                free(graphColSize);
                free(graphCapacity);
                free(state);
                return false;
            }
        }
    }
    
    // Cleanup
    for (int i = 0; i < numCourses; i++) {
        free(graph[i]);
    }
    free(graph);
    free(graphColSize);
    free(graphCapacity);
    free(state);
    
    return true;
}
```

**Why DFS for Cycle Detection?**
- Need to detect **CYCLE** in directed graph
- DFS with 3 states (unvisited, visiting, visited)
- If we visit a "visiting" node, cycle exists

#### Example 5: Validate Binary Search Tree

**Problem**: Check if tree is valid BST.

```c
/**
 * Validate Binary Search Tree
 * 
 * Input:     2
 *           / \
 *          1   3
 * Output: true
 * 
 * Input:     5
 *           / \
 *          1   4
 *             / \
 *            3   6
 * Output: false (4 is not > 5)
 * 
 * Time: O(n), Space: O(h)
 */

bool isValidBSTHelper(struct TreeNode* node, long min, long max) {
    if (node == NULL) return true;
    
    // Check if current node violates BST property
    if (node->val <= min || node->val >= max) {
        return false;
    }
    
    // Check left subtree (all values must be < node->val)
    // Check right subtree (all values must be > node->val)
    return isValidBSTHelper(node->left, min, node->val) &&
           isValidBSTHelper(node->right, node->val, max);
}

bool isValidBST(struct TreeNode* root) {
    return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
}
```

**Why DFS?**
- Need to check **ALL NODES** with constraints
- DFS naturally passes constraints down the tree
- Each subtree has valid range constraints

### 10.4 Decision Checklist for DFS

```
Use DFS IF:
☑ Need to explore ALL paths or combinations
☑ Tree traversal (preorder, inorder, postorder)
☑ Graph cycle detection
☑ Topological sort
☑ Finding connected components
☑ Exhaustive search problems
☑ Backtracking problems

DON'T Use DFS IF:
☒ Need shortest path (use BFS)
☒ Level-by-level processing (use BFS)
☒ Very deep recursion (stack overflow risk)
☒ Need to visit nearest nodes first
```

### 10.5 Practice Problems

#### Beginner Level

1. **Maximum Depth of Binary Tree**
   - Input: Tree root
   - Output: Maximum depth
   - Hint: DFS, return 1 + max(left, right)

2. **Sum of Left Leaves**
   - Input: Tree root
   - Output: Sum of all left leaf values
   - Hint: DFS with flag to track left children

3. **Path Sum** (exists or not)
   - Input: Tree root, target sum
   - Output: true if path exists
   - Hint: DFS with running sum

4. **Invert Binary Tree**
   - Input: Tree root
   - Output: Mirrored tree
   - Hint: DFS, swap left and right children

5. **Same Tree**
   - Input: Two tree roots
   - Output: true if identical
   - Hint: DFS comparing both trees

#### Intermediate Level

6. **Binary Tree Paths**
   - Input: Tree root
   - Output: All root-to-leaf paths
   - Hint: DFS with path tracking

7. **Path Sum II** (all paths)
   - Input: Tree root, target
   - Output: All paths with sum = target
   - Hint: DFS with path array

8. **Lowest Common Ancestor**
   - Input: Tree root, two nodes
   - Output: Their LCA
   - Hint: DFS, check if nodes in left/right subtree

9. **Number of Islands**
   - Input: 2D grid
   - Output: Count islands
   - Hint: DFS to mark connected cells

10. **Clone Graph**
    - Input: Graph node
    - Output: Deep copy
    - Hint: DFS with hash map

#### Advanced Level

11. **Course Schedule** (cycle detection)
    - Input: Courses and prerequisites
    - Output: Can finish all courses?
    - Hint: DFS with 3-state cycle detection

12. **Serialize and Deserialize Binary Tree**
    - Input: Tree
    - Output: String representation and reverse
    - Hint: Preorder DFS

13. **Binary Tree Maximum Path Sum**
    - Input: Tree root
    - Output: Maximum path sum (any node to any node)
    - Hint: DFS with global maximum

14. **Word Search**
    - Input: 2D board, word
    - Output: Word exists in board?
    - Hint: DFS with backtracking

15. **All Paths From Source to Target**
    - Input: Directed acyclic graph
    - Output: All paths from 0 to n-1
    - Hint: DFS with path tracking

---

## Section 11: Backtracking Pattern

### 11.1 Pattern Overview

**Core Concept**: Build solutions incrementally, abandoning candidates ("backtracking") when they cannot lead to a valid solution. It's DFS with explicit decision reversal.

**When to Use - STRONG Signals:**
- ✅ Keywords: **"all combinations"**, **"all permutations"**, **"all subsets"**
- ✅ **"Generate all"** possible solutions
- ✅ Constraint satisfaction problems
- ✅ Puzzles (Sudoku, N-Queens)
- ✅ Need to explore **decision tree** completely

**When NOT to Use:**
- ❌ Just need one solution (DFS might suffice)
- ❌ Problem has greedy solution
- ❌ Dynamic programming applicable

**Visual Representation:**
```
Generating Subsets of [1,2,3]:

                    []
         /          |          \
       [1]         [2]         [3]
      /  \          |
   [1,2] [1,3]    [2,3]
    /
[1,2,3]

At each node: CHOOSE → EXPLORE → UNCHOOSE (backtrack)
```

**Backtracking Template Structure:**
```
1. CHOOSE: Add element to current solution
2. EXPLORE: Recurse with new state
3. UNCHOOSE: Remove element (backtrack) to try other options
```

### 11.2 Core Template

```c
/**
 * BACKTRACKING TEMPLATE
 */

void backtrack(int* candidates, int candidatesSize,
               int* current, int currentSize,
               int** result, int* returnSize,
               int start) {
    
    // BASE CASE: Solution found
    if (isValidSolution(current, currentSize)) {
        // Add to result
        addToResult(result, returnSize, current, currentSize);
        return;  // Or continue if multiple solutions needed
    }
    
    // RECURSIVE CASE: Try all possibilities
    for (int i = start; i < candidatesSize; i++) {
        // PRUNING: Skip invalid choices
        if (!isValid(candidates[i], current, currentSize)) {
            continue;
        }
        
        // CHOOSE: Add candidate to solution
        current[currentSize] = candidates[i];
        
        // EXPLORE: Recurse with new state
        backtrack(candidates, candidatesSize,
                 current, currentSize + 1,
                 result, returnSize,
                 i + 1);  // i+1 for combinations, i for permutations
        
        // UNCHOOSE: Backtrack (implicit in recursion for arrays)
        // For more complex structures, explicitly remove
    }
}

/**
 * BACKTRACKING WITH EXPLICIT UNDO
 */
void backtrackWithUndo(char** board, int row, int col) {
    // Base case
    if (isSolved(board)) {
        return;
    }
    
    // Try each possible value
    for (char val = '1'; val <= '9'; val++) {
        if (isValidPlacement(board, row, col, val)) {
            // CHOOSE
            board[row][col] = val;
            
            // EXPLORE
            backtrackWithUndo(board, nextRow, nextCol);
            
            // UNCHOOSE (explicit backtrack)
            board[row][col] = '.';
        }
    }
}
```

### 11.3 Implementation Examples

#### Example 1: Subsets

**Problem**: Generate all possible subsets.

```c
/**
 * Subsets (Power Set)
 * 
 * Input: nums = [1,2,3]
 * Output: [[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
 * 
 * Time: O(2^n), Space: O(n) for recursion
 */

void subsetsHelper(int* nums, int numsSize, int* current, int currentSize,
                   int** result, int* returnSize, int** returnColumnSizes,
                   int start) {
    
    // Add current subset to result
    result[*returnSize] = (int*)malloc(currentSize * sizeof(int));
    for (int i = 0; i < currentSize; i++) {
        result[*returnSize][i] = current[i];
    }
    (*returnColumnSizes)[*returnSize] = currentSize;
    (*returnSize)++;
    
    // Try adding each remaining element
    for (int i = start; i < numsSize; i++) {
        // CHOOSE
        current[currentSize] = nums[i];
        
        // EXPLORE
        subsetsHelper(nums, numsSize, current, currentSize + 1,
                     result, returnSize, returnColumnSizes, i + 1);
        
        // UNCHOOSE (automatic via currentSize)
    }
}

int** subsets(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    *returnSize = 0;
    int maxSubsets = 1 << numsSize;  // 2^n
    
    int** result = (int**)malloc(maxSubsets * sizeof(int*));
    *returnColumnSizes = (int*)malloc(maxSubsets * sizeof(int));
    int* current = (int*)malloc(numsSize * sizeof(int));
    
    subsetsHelper(nums, numsSize, current, 0, result, returnSize, returnColumnSizes, 0);
    
    free(current);
    return result;
}
```

**Why Backtracking?**
- Need **ALL SUBSETS** (all combinations)
- Decision tree: include or exclude each element
- Backtracking explores all branches

#### Example 2: Permutations

**Problem**: Generate all permutations of an array.

```c
/**
 * Permutations
 * 
 * Input: nums = [1,2,3]
 * Output: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
 * 
 * Time: O(n! * n), Space: O(n)
 */

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void permuteHelper(int* nums, int numsSize, int start,
                   int** result, int* returnSize, int** returnColumnSizes) {
    
    // Base case: complete permutation
    if (start == numsSize) {
        result[*returnSize] = (int*)malloc(numsSize * sizeof(int));
        for (int i = 0; i < numsSize; i++) {
            result[*returnSize][i] = nums[i];
        }
        (*returnColumnSizes)[*returnSize] = numsSize;
        (*returnSize)++;
        return;
    }
    
    // Try each element in remaining positions
    for (int i = start; i < numsSize; i++) {
        // CHOOSE: Swap current with start
        swap(&nums[start], &nums[i]);
        
        // EXPLORE: Fix this position, permute rest
        permuteHelper(nums, numsSize, start + 1, result, returnSize, returnColumnSizes);
        
        // UNCHOOSE: Swap back (backtrack)
        swap(&nums[start], &nums[i]);
    }
}

int** permute(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    *returnSize = 0;
    
    // Calculate n!
    int factorial = 1;
    for (int i = 2; i <= numsSize; i++) {
        factorial *= i;
    }
    
    int** result = (int**)malloc(factorial * sizeof(int*));
    *returnColumnSizes = (int*)malloc(factorial * sizeof(int));
    
    permuteHelper(nums, numsSize, 0, result, returnSize, returnColumnSizes);
    
    return result;
}
```

**Why Backtracking?**
- Need **ALL PERMUTATIONS** (all orderings)
- Swap-based approach to generate all arrangements
- Explicit backtrack by swapping back

#### Example 3: Combination Sum

**Problem**: Find all unique combinations that sum to target.

```c
/**
 * Combination Sum
 * 
 * Input: candidates = [2,3,6,7], target = 7
 * Output: [[2,2,3], [7]]
 * Note: Same number can be used unlimited times
 * 
 * Time: O(2^t) where t = target/min(candidates)
 * Space: O(t)
 */

void combinationSumHelper(int* candidates, int candidatesSize, int target,
                          int* current, int currentSize,
                          int** result, int* returnSize, int** returnColumnSizes,
                          int start) {
    
    // Base case: found solution
    if (target == 0) {
        result[*returnSize] = (int*)malloc(currentSize * sizeof(int));
        for (int i = 0; i < currentSize; i++) {
            result[*returnSize][i] = current[i];
        }
        (*returnColumnSizes)[*returnSize] = currentSize;
        (*returnSize)++;
        return;
    }
    
    // Base case: exceeded target
    if (target < 0) {
        return;
    }
    
    // Try each candidate from start onwards
    for (int i = start; i < candidatesSize; i++) {
        // CHOOSE
        current[currentSize] = candidates[i];
        
        // EXPLORE (can reuse same element, so pass i not i+1)
        combinationSumHelper(candidates, candidatesSize, target - candidates[i],
                            current, currentSize + 1,
                            result, returnSize, returnColumnSizes, i);
        
        // UNCHOOSE (automatic)
    }
}

int** combinationSum(int* candidates, int candidatesSize, int target,
                     int* returnSize, int** returnColumnSizes) {
    *returnSize = 0;
    
    int** result = (int**)malloc(1000 * sizeof(int*));
    *returnColumnSizes = (int*)malloc(1000 * sizeof(int));
    int* current = (int*)malloc(target * sizeof(int));
    
    combinationSumHelper(candidates, candidatesSize, target,
                        current, 0, result, returnSize, returnColumnSizes, 0);
    
    free(current);
    return result;
}
```

**Why Backtracking?**
- Need **ALL COMBINATIONS** with constraint
- Can reuse elements (pass same index)
- Prune when target exceeded

#### Example 4: N-Queens

**Problem**: Place N queens on N×N board so no two queens attack each other.

```c
/**
 * N-Queens
 * 
 * Input: n = 4
 * Output: [[".Q..","...Q","Q...","..Q."],["..Q.","Q...","...Q",".Q.."]]
 * 
 * Time: O(n!), Space: O(n^2)
 */

bool isSafe(int** board, int n, int row, int col) {
    // Check column
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1) {
            return false;
        }
    }
    
    // Check upper-left diagonal
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) {
            return false;
        }
    }
    
    // Check upper-right diagonal
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 1) {
            return false;
        }
    }
    
    return true;
}

void boardToStrings(int** board, int n, char*** result, int* returnSize, int** returnColumnSizes) {
    result[*returnSize] = (char**)malloc(n * sizeof(char*));
    (*returnColumnSizes)[*returnSize] = n;
    
    for (int i = 0; i < n; i++) {
        result[*returnSize][i] = (char*)malloc((n + 1) * sizeof(char));
        for (int j = 0; j < n; j++) {
            result[*returnSize][i][j] = (board[i][j] == 1) ? 'Q' : '.';
        }
        result[*returnSize][i][n] = '\0';
    }
    
    (*returnSize)++;
}

void solveNQueensHelper(int** board, int n, int row,
                        char*** result, int* returnSize, int** returnColumnSizes) {
    
    // Base case: all queens placed
    if (row == n) {
        boardToStrings(board, n, result, returnSize, returnColumnSizes);
        return;
    }
    
    // Try placing queen in each column of current row
    for (int col = 0; col < n; col++) {
        if (isSafe(board, n, row, col)) {
            // CHOOSE
            board[row][col] = 1;
            
            // EXPLORE
            solveNQueensHelper(board, n, row + 1, result, returnSize, returnColumnSizes);
            
            // UNCHOOSE
            board[row][col] = 0;
        }
    }
}

char*** solveNQueens(int n, int* returnSize, int** returnColumnSizes) {
    *returnSize = 0;
    
    char*** result = (char***)malloc(1000 * sizeof(char**));
    *returnColumnSizes = (int*)malloc(1000 * sizeof(int));
    
    // Create board
    int** board = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        board[i] = (int*)calloc(n, sizeof(int));
    }
    
    solveNQueensHelper(board, n, 0, result, returnSize, returnColumnSizes);
    
    // Free board
    for (int i = 0; i < n; i++) {
        free(board[i]);
    }
    free(board);
    
    return result;
}
```

**Why Backtracking?**
- Classic **CONSTRAINT SATISFACTION** problem
- Try placing queen, if invalid backtrack
- Explicit undo (board[row][col] = 0)

#### Example 5: Sudoku Solver

**Problem**: Solve 9×9 Sudoku puzzle.

```c
/**
 * Sudoku Solver
 * 
 * Input: Partially filled 9x9 board
 * Output: Completed valid board
 * 
 * Time: O(9^(n*n)) worst case, Space: O(n*n)
 */

bool isValidSudoku(char** board, int row, int col, char num) {
    // Check row
    for (int j = 0; j < 9; j++) {
        if (board[row][j] == num) {
            return false;
        }
    }
    
    // Check column
    for (int i = 0; i < 9; i++) {
        if (board[i][col] == num) {
            return false;
        }
    }
    
    // Check 3x3 box
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int i = boxRow; i < boxRow + 3; i++) {
        for (int j = boxCol; j < boxCol + 3; j++) {
            if (board[i][j] == num) {
                return false;
            }
        }
    }
    
    return true;
}

bool solveSudokuHelper(char** board) {
    // Find next empty cell
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') {
                // Try each digit
                for (char num = '1'; num <= '9'; num++) {
                    if (isValidSudoku(board, i, j, num)) {
                        // CHOOSE
                        board[i][j] = num;
                        
                        // EXPLORE
                        if (solveSudokuHelper(board)) {
                            return true;  // Solution found
                        }
                        
                        // UNCHOOSE
                        board[i][j] = '.';
                    }
                }
                
                return false;  // No valid digit for this cell
            }
        }
    }
    
    return true;  // All cells filled
}

void solveSudoku(char** board, int boardSize, int* boardColSize) {
    solveSudokuHelper(board);
}
```

**Why Backtracking?**
- **CONSTRAINT SATISFACTION** puzzle
- Try each digit, backtrack if invalid
- Returns early when solution found

### 11.4 Decision Checklist for Backtracking

```
Use Backtracking IF:
☑ Need ALL solutions (combinations, permutations, subsets)
☑ Constraint satisfaction problems
☑ Keywords: "all possible", "generate all"
☑ Decision tree exploration needed
☑ Can prune invalid branches early
☑ Puzzles (Sudoku, N-Queens, etc.)

DON'T Use Backtracking IF:
☒ Just need one solution (DFS might suffice)
☒ Greedy algorithm works
☒ Dynamic programming applicable
☒ Problem size too large (exponential complexity)
```

### 11.5 Practice Problems

#### Beginner Level

1. **Subsets**
   - Input: `[1,2,3]`
   - Output: All subsets
   - Hint: Include/exclude each element

2. **Permutations**
   - Input: `[1,2,3]`
   - Output: All permutations
   - Hint: Swap-based backtracking

3. **Letter Combinations of Phone Number**
   - Input: `"23"`
   - Output: `["ad","ae","af","bd","be","bf","cd","ce","cf"]`
   - Hint: Backtrack through digit mappings

4. **Generate Parentheses**
   - Input: `n = 3`
   - Output: All valid combinations of n pairs of parentheses
   - Hint: Track open/close count

5. **Binary Watch**
   - Input: `turnedOn = 1`
   - Output: All possible times
   - Hint: Backtrack through LED combinations

#### Intermediate Level

6. **Combination Sum**
   - Input: `candidates = [2,3,6,7]`, `target = 7`
   - Output: `[[2,2,3], [7]]`
   - Hint: Can reuse elements

7. **Combination Sum II** (no duplicates)
   - Input: candidates with duplicates, target
   - Output: Unique combinations
   - Hint: Sort and skip duplicates

8. **Palindrome Partitioning**
   - Input: `s = "aab"`
   - Output: `[["a","a","b"], ["aa","b"]]`
   - Hint: Backtrack, check palindrome at each step

9. **Word Search**
   - Input: 2D board, word
   - Output: Word exists?
   - Hint: DFS with backtracking, mark visited

10. **Restore IP Addresses**
    - Input: `"25525511135"`
    - Output: Valid IP addresses
    - Hint: Backtrack with 4 segments

#### Advanced Level

11. **N-Queens**
    - Input: `n = 4`
    - Output: All solutions
    - Hint: Place queen row by row, check attacks

12. **Sudoku Solver**
    - Input: Partially filled board
    - Output: Completed valid board
    - Hint: Fill empty cells, validate constraints

13. **Expression Add Operators**
    - Input: `num = "123"`, `target = 6`
    - Output: `["1+2+3", "1*2*3"]`
    - Hint: Backtrack with operators, track multiplication

14. **Remove Invalid Parentheses**
    - Input: `"()())()"`
    - Output: Minimum removed for valid
    - Hint: BFS or backtracking with validation

15. **Regular Expression Matching** (with backtracking)
    - Input: string, pattern with `*` and `.`
    - Output: Pattern matches?
    - Hint: Backtrack through pattern options

---

## Section 12: Monotonic Stack Pattern

### 12.1 Pattern Overview

**Core Concept**: Maintain a stack where elements are in monotonic order (increasing or decreasing). Used to find next greater/smaller element efficiently.

**When to Use - STRONG Signals:**
- ✅ Keywords: **"next greater"**, **"next smaller"**, **"previous greater/smaller"**
- ✅ Need to find nearest larger/smaller element
- ✅ **Histogram** problems
- ✅ **Temperature** or **stock** span problems
- ✅ Rectangle area in matrix

**When NOT to Use:**
- ❌ Need all greater/smaller elements (not just next)
- ❌ Random access needed
- ❌ No ordering relationship

**Visual Representation:**
```
Next Greater Element:
Input: [2, 1, 2, 4, 3]

Process with monotonic decreasing stack:
i=0: stack=[2], result[-1]
i=1: stack=[2,1], result[-1,-1]
i=2: 2>1, pop 1, result[-1,-1,2]; 2==2, pop 2, result[-1,-1,2]; stack=[2]
i=3: 4>2, pop 2, result[-1,-1,2,4]; stack=[4]
i=4: stack=[4,3], result[-1,-1,2,4,-1]

Result: [-1, 2, 4, 4, -1]
```

### 12.2 Core Template

```c
/**
 * MONOTONIC STACK TEMPLATES
 */

// Template 1: Next Greater Element
int* nextGreaterElement(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;
    int* result = (int*)malloc(numsSize * sizeof(int));
    int* stack = (int*)malloc(numsSize * sizeof(int));
    int top = -1;
    
    // Traverse from right to left
    for (int i = numsSize - 1; i >= 0; i--) {
        // Pop smaller elements
        while (top >= 0 && stack[top] <= nums[i]) {
            top--;
        }
        
        // Top of stack is next greater (or -1 if empty)
        result[i] = (top >= 0) ? stack[top] : -1;
        
        // Push current element
        stack[++top] = nums[i];
    }
    
    free(stack);
    return result;
}

// Template 2: Next Smaller Element
int* nextSmallerElement(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;
    int* result = (int*)malloc(numsSize * sizeof(int));
    int* stack = (int*)malloc(numsSize * sizeof(int));
    int top = -1;
    
    for (int i = numsSize - 1; i >= 0; i--) {
        // Pop larger elements
        while (top >= 0 && stack[top] >= nums[i]) {
            top--;
        }
        
        result[i] = (top >= 0) ? stack[top] : -1;
        stack[++top] = nums[i];
    }
    
    free(stack);
    return result;
}

// Template 3: Previous Greater Element
int* previousGreaterElement(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;
    int* result = (int*)malloc(numsSize * sizeof(int));
    int* stack = (int*)malloc(numsSize * sizeof(int));
    int top = -1;

    // Traverse from left to right
    for (int i = 0; i < numsSize; i++) {
        // Pop smaller elements
        while (top >= 0 && stack[top] <= nums[i]) {
            top--;
        }
        
        result[i] = (top >= 0) ? stack[top] : -1;
        stack[++top] = nums[i];
    }
    
    free(stack);
    return result;
}
```

### 12.3 Implementation Examples

#### Example 1: Daily Temperatures

**Problem**: For each day, find how many days until warmer temperature.

```c
/**
 * Daily Temperatures
 * 
 * Input: temperatures = [73,74,75,71,69,72,76,73]
 * Output: [1,1,4,2,1,1,0,0]
 * 
 * Time: O(n), Space: O(n)
 */
int* dailyTemperatures(int* temperatures, int temperaturesSize, int* returnSize) {
    *returnSize = temperaturesSize;
    int* result = (int*)calloc(temperaturesSize, sizeof(int));
    int* stack = (int*)malloc(temperaturesSize * sizeof(int));  // Store indices
    int top = -1;
    
    for (int i = 0; i < temperaturesSize; i++) {
        // While current temp is warmer than stack top
        while (top >= 0 && temperatures[i] > temperatures[stack[top]]) {
            int prevIndex = stack[top--];
            result[prevIndex] = i - prevIndex;  // Days until warmer
        }
        
        // Push current index
        stack[++top] = i;
    }
    
    free(stack);
    return result;
}

// Test function
void testDailyTemperatures() {
    int temps[] = {73, 74, 75, 71, 69, 72, 76, 73};
    int size = 8;
    int returnSize;
    
    printf("Temperatures: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", temps[i]);
    }
    printf("\n");
    
    int* result = dailyTemperatures(temps, size, &returnSize);
    
    printf("Days until warmer: ");
    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    free(result);
}
```

**Why Monotonic Stack?**
- Need **NEXT GREATER** element (warmer temperature)
- Stack maintains decreasing temperatures
- O(n) solution vs O(n²) brute force

**Key Insight**: Each element pushed and popped at most once → O(n) time.

#### Example 2: Next Greater Element I

**Problem**: Find next greater element for elements in nums1 within nums2.

```c
/**
 * Next Greater Element I
 * 
 * Input: nums1 = [4,1,2], nums2 = [1,3,4,2]
 * Output: [-1,3,-1]
 * 
 * Time: O(m + n), Space: O(n)
 */
int* nextGreaterElement(int* nums1, int nums1Size, int* nums2, int nums2Size, int* returnSize) {
    *returnSize = nums1Size;
    
    // Build next greater map for nums2
    // Using hash map (simplified with array for values up to 10000)
    int* nextGreater = (int*)malloc(10001 * sizeof(int));
    for (int i = 0; i < 10001; i++) {
        nextGreater[i] = -1;
    }
    
    int* stack = (int*)malloc(nums2Size * sizeof(int));
    int top = -1;
    
    // Build next greater map
    for (int i = 0; i < nums2Size; i++) {
        while (top >= 0 && nums2[i] > stack[top]) {
            int val = stack[top--];
            nextGreater[val] = nums2[i];
        }
        stack[++top] = nums2[i];
    }
    
    // Query for nums1
    int* result = (int*)malloc(nums1Size * sizeof(int));
    for (int i = 0; i < nums1Size; i++) {
        result[i] = nextGreater[nums1[i]];
    }
    
    free(nextGreater);
    free(stack);
    return result;
}
```

**Why Monotonic Stack?**
- Need **NEXT GREATER** for subset of elements
- Precompute with stack, then query
- Hash map stores results

#### Example 3: Largest Rectangle in Histogram

**Problem**: Find largest rectangle area in histogram.

```c
/**
 * Largest Rectangle in Histogram
 * 
 * Input: heights = [2,1,5,6,2,3]
 * Output: 10 (rectangle with height 5, width 2)
 * 
 * Time: O(n), Space: O(n)
 */
int largestRectangleArea(int* heights, int heightsSize) {
    int* stack = (int*)malloc((heightsSize + 1) * sizeof(int));
    int top = -1;
    int maxArea = 0;
    
    for (int i = 0; i <= heightsSize; i++) {
        int currentHeight = (i == heightsSize) ? 0 : heights[i];
        
        // While current height is less than stack top
        while (top >= 0 && currentHeight < heights[stack[top]]) {
            int h = heights[stack[top--]];
            
            // Width = current index - left boundary - 1
            int width = (top >= 0) ? (i - stack[top] - 1) : i;
            
            int area = h * width;
            if (area > maxArea) {
                maxArea = area;
            }
        }
        
        stack[++top] = i;
    }
    
    free(stack);
    return maxArea;
}

// Test function
void testLargestRectangle() {
    int heights[] = {2, 1, 5, 6, 2, 3};
    int size = 6;
    
    printf("Heights: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", heights[i]);
    }
    printf("\n");
    
    int result = largestRectangleArea(heights, size);
    printf("Largest rectangle area: %d\n", result);
}
```

**Why Monotonic Stack?**
- For each bar, need to find **LEFT and RIGHT boundaries**
- Left boundary = previous smaller element
- Right boundary = next smaller element
- Stack maintains increasing heights

**Algorithm Explanation:**
1. Stack stores indices of bars in increasing height order
2. When shorter bar found, calculate area for all taller bars
3. Width = current position - left boundary

#### Example 4: Remove K Digits

**Problem**: Remove k digits to form smallest possible number.

```c
/**
 * Remove K Digits
 * 
 * Input: num = "1432219", k = 3
 * Output: "1219"
 * 
 * Time: O(n), Space: O(n)
 */
char* removeKdigits(char* num, int k) {
    int len = strlen(num);
    if (k >= len) {
        char* result = (char*)malloc(2 * sizeof(char));
        result[0] = '0';
        result[1] = '\0';
        return result;
    }
    
    char* stack = (char*)malloc((len + 1) * sizeof(char));
    int top = -1;
    
    for (int i = 0; i < len; i++) {
        // Remove larger digits from stack
        while (k > 0 && top >= 0 && stack[top] > num[i]) {
            top--;
            k--;
        }
        stack[++top] = num[i];
    }
    
    // Remove remaining k digits from end
    top -= k;
    
    // Remove leading zeros
    int start = 0;
    while (start <= top && stack[start] == '0') {
        start++;
    }
    
    // Build result
    int resultLen = top - start + 1;
    if (resultLen <= 0) {
        char* result = (char*)malloc(2 * sizeof(char));
        result[0] = '0';
        result[1] = '\0';
        free(stack);
        return result;
    }
    
    char* result = (char*)malloc((resultLen + 1) * sizeof(char));
    for (int i = 0; i < resultLen; i++) {
        result[i] = stack[start + i];
    }
    result[resultLen] = '\0';
    
    free(stack);
    return result;
}
```

**Why Monotonic Stack?**
- Want **SMALLEST** number → maintain increasing digits
- Remove larger digits when smaller one appears
- Greedy with stack ensures optimal result

#### Example 5: Remove All Adjacent Duplicates In String II

**Problem**: Remove k adjacent duplicates repeatedly.

```c
/**
 * Remove All Adjacent Duplicates in String II
 * 
 * Input: s = "deeedbbcccbdaa", k = 3
 * Output: "aa"
 * 
 * Time: O(n), Space: O(n)
 */

typedef struct {
    char ch;
    int count;
} StackElement;

char* removeDuplicates(char* s, int k) {
    int len = strlen(s);
    StackElement* stack = (StackElement*)malloc(len * sizeof(StackElement));
    int top = -1;
    
    for (int i = 0; i < len; i++) {
        if (top >= 0 && stack[top].ch == s[i]) {
            // Same character, increment count
            stack[top].count++;
            
            // If count reaches k, remove
            if (stack[top].count == k) {
                top--;
            }
        } else {
            // New character
            stack[++top].ch = s[i];
            stack[top].count = 1;
        }
    }
    
    // Build result
    int resultLen = 0;
    for (int i = 0; i <= top; i++) {
        resultLen += stack[i].count;
    }
    
    char* result = (char*)malloc((resultLen + 1) * sizeof(char));
    int idx = 0;
    for (int i = 0; i <= top; i++) {
        for (int j = 0; j < stack[i].count; j++) {
            result[idx++] = stack[i].ch;
        }
    }
    result[resultLen] = '\0';
    
    free(stack);
    return result;
}

// Test function
void testRemoveDuplicates() {
    char s[] = "deeedbbcccbdaa";
    int k = 3;
    
    printf("Input: %s, k = %d\n", s, k);
    
    char* result = removeDuplicates(s, k);
    printf("Output: %s\n", result);
    
    free(result);
}
```

**Why Monotonic Stack (with count)?**
- Track **ADJACENT duplicates**
- Stack element stores character and count
- Remove when count reaches k

### 12.4 Decision Checklist for Monotonic Stack

```
Use Monotonic Stack IF:
☑ Need next/previous greater/smaller element
☑ Keywords: "next greater", "next smaller", "previous greater/smaller"
☑ Histogram or rectangle problems
☑ Removing elements based on comparison
☑ Finding spans or ranges
☑ Can process elements in one pass

DON'T Use Monotonic Stack IF:
☒ Need all greater/smaller (not just next)
☒ No monotonic relationship
☒ Need random access to elements
☒ Problem requires sorting entire array
```

### 12.5 Practice Problems

#### Beginner Level

1. **Next Greater Element I**
   - Input: Two arrays
   - Output: Next greater for nums1 in nums2
   - Hint: Build map with stack

2. **Daily Temperatures**
   - Input: Temperature array
   - Output: Days until warmer
   - Hint: Stack of indices

3. **Remove All Adjacent Duplicates in String**
   - Input: String
   - Output: String after removing adjacent duplicates
   - Hint: Stack, pop when same

4. **Baseball Game**
   - Input: Operations array
   - Output: Sum of scores
   - Hint: Stack to track scores

5. **Backspace String Compare**
   - Input: Two strings with # (backspace)
   - Output: Are they equal?
   - Hint: Stack or two pointers

#### Intermediate Level

6. **Next Greater Element II** (circular array)
   - Input: Circular array
   - Output: Next greater for each element
   - Hint: Process array twice

7. **Remove K Digits**
   - Input: Number string, k
   - Output: Smallest number after removing k digits
   - Hint: Monotonic increasing stack

8. **Largest Rectangle in Histogram**
   - Input: Heights array
   - Output: Largest rectangle area
   - Hint: Stack for left/right boundaries

9. **Maximal Rectangle**
   - Input: Binary matrix
   - Output: Largest rectangle of 1s
   - Hint: Convert to histogram problem

10. **Sum of Subarray Minimums**
    - Input: Array
    - Output: Sum of minimums of all subarrays
    - Hint: Monotonic stack for contribution of each element

#### Advanced Level

11. **Online Stock Span**
    - Input: Stream of stock prices
    - Output: Span (consecutive days ≤ current)
    - Hint: Monotonic decreasing stack

12. **Trapping Rain Water**
    - Input: Heights array
    - Output: Water trapped
    - Hint: Stack or two pointers

13. **Remove Duplicate Letters**
    - Input: String with duplicates
    - Output: Smallest lexicographic string without duplicates
    - Hint: Monotonic stack with seen map

14. **132 Pattern**
    - Input: Array
    - Output: Does 132 pattern exist? (i < j < k, nums[i] < nums[k] < nums[j])
    - Hint: Stack to track potential patterns

15. **Maximum Width Ramp**
    - Input: Array
    - Output: Maximum j - i where nums[i] ≤ nums[j]
    - Hint: Monotonic decreasing stack from left

---

## Section 13: Union Find Pattern

### 13.1 Pattern Overview

**Core Concept**: Track elements partitioned into disjoint sets. Efficiently performs union (merge sets) and find (which set contains element) operations.

**Also Known As**: Disjoint Set Union (DSU), Union-Find Disjoint Sets (UFDS)

**When to Use - STRONG Signals:**
- ✅ Keywords: **"connected components"**, **"network connectivity"**, **"grouping"**
- ✅ Dynamic connectivity queries
- ✅ Detect cycles in undirected graphs
- ✅ Finding clusters or groups
- ✅ Minimum spanning tree (Kruskal's algorithm)
- ✅ Problems asking "are X and Y in same group?"

**When NOT to Use:**
- ❌ Need to traverse paths (use BFS/DFS)
- ❌ Directed graphs (Union Find works on undirected)
- ❌ Need to track specific connections between elements
- ❌ Sets need to be split (Union Find doesn't support split)

**Visual Representation:**
```
Initial Sets: {0} {1} {2} {3} {4}

After union(0, 1):
{0, 1} {2} {3} {4}

After union(2, 3):
{0, 1} {2, 3} {4}

After union(0, 3):
{0, 1, 2, 3} {4}

find(1) and find(3) return same representative
```

### 13.2 Core Implementation

```c
/**
 * UNION FIND DATA STRUCTURE
 */
typedef struct {
    int* parent;     // parent[i] = parent of i
    int* rank;       // rank[i] = approximate depth of tree rooted at i
    int count;       // number of disjoint sets
    int size;        // total elements
} UnionFind;

/**
 * Initialize Union Find
 */
UnionFind* createUnionFind(int n) {
    UnionFind* uf = (UnionFind*)malloc(sizeof(UnionFind));
    uf->parent = (int*)malloc(n * sizeof(int));
    uf->rank = (int*)malloc(n * sizeof(int));
    uf->count = n;
    uf->size = n;
    
    // Initially, each element is its own parent (separate set)
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
    
    return uf;
}

/**
 * Find with Path Compression
 * Returns representative (root) of set containing x
 * 
 * Path compression: make all nodes point directly to root
 */
int find(UnionFind* uf, int x) {
    if (uf->parent[x] != x) {
        // Path compression: recursively find root and update parent
        uf->parent[x] = find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

/**
 * Union by Rank
 * Merge sets containing x and y
 * Returns true if merged, false if already in same set
 */
bool unionSets(UnionFind* uf, int x, int y) {
    int rootX = find(uf, x);
    int rootY = find(uf, y);
    
    if (rootX == rootY) {
        return false;  // Already in same set
    }
    
    // Union by rank: attach smaller tree under root of larger tree
    if (uf->rank[rootX] < uf->rank[rootY]) {
        uf->parent[rootX] = rootY;
    } else if (uf->rank[rootX] > uf->rank[rootY]) {
        uf->parent[rootY] = rootX;
    } else {
        uf->parent[rootY] = rootX;
        uf->rank[rootX]++;
    }
    
    uf->count--;  // Merged two sets
    return true;
}

/**
 * Check if x and y are in same set
 */
bool isConnected(UnionFind* uf, int x, int y) {
    return find(uf, x) == find(uf, y);
}

/**
 * Get number of disjoint sets
 */
int getCount(UnionFind* uf) {
    return uf->count;
}

/**
 * Free Union Find structure
 */
void freeUnionFind(UnionFind* uf) {
    free(uf->parent);
    free(uf->rank);
    free(uf);
}
```

**Time Complexity:**
- Find: O(α(n)) ≈ O(1) with path compression
- Union: O(α(n)) ≈ O(1) with union by rank
- α(n) is inverse Ackermann function (grows extremely slowly)

### 13.3 Implementation Examples

#### Example 1: Number of Connected Components

**Problem**: Count number of connected components in undirected graph.

```c
/**
 * Number of Connected Components in Undirected Graph
 * 
 * Input: n = 5, edges = [[0,1], [1,2], [3,4]]
 * Output: 2 (components: {0,1,2} and {3,4})
 * 
 * Time: O(E * α(V)), Space: O(V)
 */
int countComponents(int n, int** edges, int edgesSize, int* edgesColSize) {
    UnionFind* uf = createUnionFind(n);
    
    // Process each edge
    for (int i = 0; i < edgesSize; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        unionSets(uf, u, v);
    }
    
    int count = getCount(uf);
    freeUnionFind(uf);
    
    return count;
}

// Test function
void testCountComponents() {
    int n = 5;
    int** edges = (int**)malloc(3 * sizeof(int*));
    edges[0] = (int*)malloc(2 * sizeof(int));
    edges[0][0] = 0; edges[0][1] = 1;
    edges[1] = (int*)malloc(2 * sizeof(int));
    edges[1][0] = 1; edges[1][1] = 2;
    edges[2] = (int*)malloc(2 * sizeof(int));
    edges[2][0] = 3; edges[2][1] = 4;
    
    int colSize = 2;
    int result = countComponents(n, edges, 3, &colSize);
    
    printf("Number of nodes: %d\n", n);
    printf("Edges: [0,1] [1,2] [3,4]\n");
    printf("Connected components: %d\n", result);
    
    for (int i = 0; i < 3; i++) {
        free(edges[i]);
    }
    free(edges);
}
```

**Why Union Find?**
- Need to count **CONNECTED COMPONENTS**
- Each edge unions two nodes
- Final count is number of disjoint sets

#### Example 2: Redundant Connection

**Problem**: Find edge that can be removed to make tree (detect cycle).

```c
/**
 * Redundant Connection
 * 
 * Input: edges = [[1,2], [1,3], [2,3]]
 * Output: [2,3] (last edge that creates cycle)
 * 
 * Time: O(N * α(N)), Space: O(N)
 */
int* findRedundantConnection(int** edges, int edgesSize, int* edgesColSize, int* returnSize) {
    *returnSize = 2;
    int* result = (int*)malloc(2 * sizeof(int));
    
    UnionFind* uf = createUnionFind(edgesSize + 1);  // Nodes numbered 1 to N
    
    for (int i = 0; i < edgesSize; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        
        // If already connected, this edge creates cycle
        if (!unionSets(uf, u, v)) {
            result[0] = u;
            result[1] = v;
            freeUnionFind(uf);
            return result;
        }
    }
    
    freeUnionFind(uf);
    return result;
}

// Test function
void testRedundantConnection() {
    int** edges = (int**)malloc(3 * sizeof(int*));
    edges[0] = (int*)malloc(2 * sizeof(int));
    edges[0][0] = 1; edges[0][1] = 2;
    edges[1] = (int*)malloc(2 * sizeof(int));
    edges[1][0] = 1; edges[1][1] = 3;
    edges[2] = (int*)malloc(2 * sizeof(int));
    edges[2][0] = 2; edges[2][1] = 3;
    
    int returnSize;
    int colSize = 2;
    int* result = findRedundantConnection(edges, 3, &colSize, &returnSize);
    
    printf("Edges: [1,2] [1,3] [2,3]\n");
    printf("Redundant connection: [%d,%d]\n", result[0], result[1]);
    
    free(result);
    for (int i = 0; i < 3; i++) {
        free(edges[i]);
    }
    free(edges);
}
```

**Why Union Find?**
- Detect **CYCLE** in undirected graph
- If union returns false, edge creates cycle
- First edge that fails is the redundant one

#### Example 3: Number of Provinces

**Problem**: Count number of provinces (groups of directly/indirectly connected cities).

```c
/**
 * Number of Provinces
 * 
 * Input: isConnected = [[1,1,0],
 *                       [1,1,0],
 *                       [0,0,1]]
 * Output: 2 (province 1: cities 0,1; province 2: city 2)
 * 
 * Time: O(N^2 * α(N)), Space: O(N)
 */
int findCircleNum(int** isConnected, int isConnectedSize, int* isConnectedColSize) {
    int n = isConnectedSize;
    UnionFind* uf = createUnionFind(n);
    
    // Process adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (isConnected[i][j] == 1) {
                unionSets(uf, i, j);
            }
        }
    }
    
    int count = getCount(uf);
    freeUnionFind(uf);
    
    return count;
}

// Test function
void testFindCircleNum() {
    int** isConnected = (int**)malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        isConnected[i] = (int*)malloc(3 * sizeof(int));
    }
    
    isConnected[0][0] = 1; isConnected[0][1] = 1; isConnected[0][2] = 0;
    isConnected[1][0] = 1; isConnected[1][1] = 1; isConnected[1][2] = 0;
    isConnected[2][0] = 0; isConnected[2][1] = 0; isConnected[2][2] = 1;
    
    int colSize = 3;
    int result = findCircleNum(isConnected, 3, &colSize);
    
    printf("Adjacency matrix:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", isConnected[i][j]);
        }
        printf("\n");
    }
    printf("Number of provinces: %d\n", result);
    
    for (int i = 0; i < 3; i++) {
        free(isConnected[i]);
    }
    free(isConnected);
}
```

**Why Union Find?**
- Count **CONNECTED COMPONENTS** in graph
- Adjacency matrix gives connections
- Union connected cities, count final sets

#### Example 4: Accounts Merge

**Problem**: Merge accounts with common emails.

```c
/**
 * Accounts Merge
 * 
 * Input: accounts = [["John","john@mail.com","john_work@mail.com"],
 *                    ["John","john_other@mail.com"],
 *                    ["John","john@mail.com","john_other@mail.com"]]
 * Output: [["John","john@mail.com","john_other@mail.com","john_work@mail.com"]]
 * 
 * Time: O(N * K * α(N)) where N = accounts, K = emails per account
 * Space: O(N * K)
 */

// This is a simplified version showing the Union Find logic
// Full implementation would need hash maps for email-to-account mapping

int accountsMergeUsingUnionFind(char*** accounts, int accountsSize) {
    // Create Union Find for accounts
    UnionFind* uf = createUnionFind(accountsSize);
    
    // Build email to account index map (simplified - would need hash map)
    // For each account, union with accounts sharing emails
    
    // Example logic:
    // for each account i:
    //   for each email in account i:
    //     if email seen before in account j:
    //       unionSets(uf, i, j)
    //     else:
    //       mark email as belonging to account i
    
    int mergedCount = getCount(uf);
    freeUnionFind(uf);
    
    return mergedCount;
}
```

**Why Union Find?**
- Merge **GROUPS** with common elements (emails)
- Union accounts sharing emails
- Result is merged groups

#### Example 5: Satisfiability of Equality Equations

**Problem**: Check if equations are simultaneously satisfiable.

```c
/**
 * Satisfiability of Equality Equations
 * 
 * Input: equations = ["a==b","b!=a"]
 * Output: false
 * 
 * Input: equations = ["a==b","b==c","a==c"]
 * Output: true
 * 
 * Time: O(N * α(26)), Space: O(26)
 */
bool equationsPossible(char** equations, int equationsSize) {
    // Union Find for 26 letters
    UnionFind* uf = createUnionFind(26);
    
    // Phase 1: Process all equality equations
    for (int i = 0; i < equationsSize; i++) {
        if (equations[i][1] == '=') {  // "==" equation
            int var1 = equations[i][0] - 'a';
            int var2 = equations[i][3] - 'a';
            unionSets(uf, var1, var2);
        }
    }
    
    // Phase 2: Check all inequality equations
    for (int i = 0; i < equationsSize; i++) {
        if (equations[i][1] == '!') {  // "!=" equation
            int var1 = equations[i][0] - 'a';
            int var2 = equations[i][3] - 'a';
            
            // If they're in same set, contradiction
            if (isConnected(uf, var1, var2)) {
                freeUnionFind(uf);
                return false;
            }
        }
    }
    
    freeUnionFind(uf);
    return true;
}

// Test function
void testEquationsPossible() {
    char* equations1[] = {"a==b", "b!=a"};
    bool result1 = equationsPossible(equations1, 2);
    printf("Equations: a==b, b!=a\n");
    printf("Possible: %s\n\n", result1 ? "true" : "false");
    
    char* equations2[] = {"a==b", "b==c", "a==c"};
    bool result2 = equationsPossible(equations2, 3);
    printf("Equations: a==b, b==c, a==c\n");
    printf("Possible: %s\n", result2 ? "true" : "false");
}
```

**Why Union Find?**
- Track **EQUALITY** relationships (transitive)
- Union equal variables
- Check if inequalities violate equalities

### 13.4 Decision Checklist for Union Find

```
Use Union Find IF:
☑ Need to track connected components
☑ Dynamic connectivity queries
☑ Detecting cycles in undirected graphs
☑ Grouping/clustering problems
☑ Keywords: "connected", "groups", "network", "components"
☑ Need to efficiently merge sets and check membership

DON'T Use Union Find IF:
☒ Need to traverse paths between nodes (use BFS/DFS)
☒ Directed graphs (Union Find for undirected)
☒ Need to split sets (Union Find doesn't support)
☒ Need detailed connection information
☒ Small dataset where simpler approaches work
```

### 13.5 Practice Problems

#### Beginner Level

1. **Number of Provinces**
   - Input: Adjacency matrix
   - Output: Number of provinces
   - Hint: Union connected cities, count sets

2. **Friend Circles** (same as provinces)
   - Input: Friendship matrix
   - Output: Number of friend circles
   - Hint: Union friends, count components

3. **Number of Connected Components in Undirected Graph**
   - Input: n nodes, edges list
   - Output: Number of components
   - Hint: Union each edge, return count

4. **Graph Valid Tree**
   - Input: n nodes, edges
   - Output: Is it a valid tree?
   - Hint: Tree has n-1 edges and no cycles

5. **Redundant Connection**
   - Input: Edges forming tree + 1 extra
   - Output: Extra edge
   - Hint: First edge that creates cycle

#### Intermediate Level

6. **Most Stones Removed with Same Row or Column**
   - Input: Stone positions
   - Output: Maximum stones removable
   - Hint: Union stones in same row/col

7. **Accounts Merge**
   - Input: Accounts with emails
   - Output: Merged accounts
   - Hint: Union accounts with common emails

8. **Satisfiability of Equality Equations**
   - Input: Equality/inequality equations
   - Output: Are they satisfiable?
   - Hint: Union equals, check inequals

9. **Regions Cut By Slashes**
   - Input: Grid with / and \ slashes
   - Output: Number of regions
   - Hint: Divide each cell into 4 parts, union

10. **Minimize Malware Spread**
    - Input: Graph, initial infected nodes
    - Output: Which node to remove to minimize spread
    - Hint: Union Find for components

#### Advanced Level

11. **Smallest String With Swaps**
    - Input: String, list of swappable index pairs
    - Output: Lexicographically smallest string
    - Hint: Union swappable indices, sort each component

12. **Evaluate Division**
    - Input: Equations with values, queries
    - Output: Query results
    - Hint: Weighted Union Find

13. **Redundant Connection II** (directed graph)
    - Input: Directed edges
    - Output: Edge to remove
    - Hint: Check for two parents or cycle

14. **Number of Islands II** (online)
    - Input: Add land positions one by one
    - Output: Number of islands after each addition
    - Hint: Union Find with 4-directional checks

15. **Minimum Cost to Connect All Points** (MST)
    - Input: Points in 2D plane
    - Output: Minimum cost to connect all
    - Hint: Kruskal's algorithm with Union Find

---

# PART 3: PRACTICAL APPLICATION

## Section 14: Pattern Combination Strategies

### 14.1 When Multiple Patterns Apply

Many problems require **combining** multiple patterns. Here's how to identify and implement combinations:

#### Common Pattern Combinations

**1. Two Pointers + Sliding Window**
```
Problem: Longest Substring with At Most K Distinct Characters
- Two Pointers: windowStart and windowEnd
- Sliding Window: Dynamic size based on distinct count
- HashMap: Track character frequencies
```

**2. BFS/DFS + Union Find**
```
Problem: Number of Islands with connectivity queries
- BFS/DFS: Initial island identification
- Union Find: Handle dynamic connectivity queries
```

**3. Heap + Sliding Window**
```
Problem: Sliding Window Median
- Sliding Window: Fixed size k
- Two Heaps: Maintain median (max heap left, min heap right)
```

**4. Backtracking + Memoization**
```
Problem: Word Break II (all possible sentences)
- Backtracking: Generate all combinations
- Memoization: Cache results for substrings
```

**5. Stack + Hash Map**
```
Problem: Next Greater Element with frequency
- Monotonic Stack: Find next greater
- Hash Map: Store results for query
```

### 14.2 Decision Framework for Combinations

```
STEP 1: Identify Primary Pattern
├─ What is the main problem asking for?
├─ What's the core algorithmic challenge?
└─ Start with the most obvious pattern

STEP 2: Identify Bottlenecks
├─ Is the primary pattern efficient enough?
├─ What sub-problems need solving?
└─ What data needs tracking?

STEP 3: Add Supporting Patterns
├─ What data structure helps with sub-problems?
├─ Can another pattern optimize a step?
└─ Is there redundant computation to cache?

STEP 4: Verify Complexity
├─ Combined time complexity acceptable?
├─ Space complexity within limits?
└─ Implementation complexity manageable?
```

### 14.3 Example: Complex Problem Breakdown

**Problem**: Find K Closest Points to Origin, but points are added dynamically (stream).

**Analysis:**
```
Primary Pattern: Top K Elements (Heap)
- Need to maintain K closest points
- Heap of size K is natural choice

Challenge: Points added dynamically
- Can't sort all points upfront

Solution Pattern Combination:
1. Max Heap (Top K Pattern)
   - Size K, stores K closest
   - Max at top for easy removal

2. Distance Calculation (Math)
   - Squared distance: x² + y²
   - Avoid sqrt for efficiency

3. Comparison Logic (Custom)
   - Compare by distance
   - Remove farthest when heap full

Implementation:
- Heap of size K
- For each new point:
  - If heap not full: add point
  - Else if point closer than max: remove max, add point
  - Else: ignore point
```

### 14.4 Common Pitfalls

**Pitfall 1: Over-complicating**
```
❌ BAD: Using Union Find + BFS + Heap for simple connectivity
✅ GOOD: Just BFS is sufficient

Rule: Start simple, add complexity only when needed
```

**Pitfall 2: Ignoring Time Complexity**
```
❌ BAD: Sliding Window O(n) + Sorting O(n log n) in window = O(n² log n)
✅ GOOD: Use heap in window for O(n log k)

Rule: Combined complexity is product/sum of steps
```

**Pitfall 3: Pattern Mismatch**
```
❌ BAD: Using Backtracking when DP applies (overlapping subproblems)
✅ GOOD: Recognize memoization opportunity

Rule: If recomputing same subproblems, consider DP
```

**Pitfall 4: Incorrect Pattern Choice**
```
❌ BAD: DFS for shortest path in unweighted graph
✅ GOOD: BFS guarantees shortest path

Rule: Match pattern characteristics to problem requirements
```

---

## Section 15: Complete Decision Framework

### 15.1 The Master Checklist

Use this systematic approach for **every** problem:

#### Phase 1: Problem Analysis (2-3 minutes)

```
INPUT ANALYSIS:
□ What is the input type? (array, tree, graph, string, etc.)
□ What is the input size? (determines complexity requirement)
□ Is input sorted? Modified? Contains duplicates?
□ What are value ranges?

OUTPUT ANALYSIS:
□ Single value or multiple values?
□ All solutions or optimal solution?
□ Boolean or actual result?
□ Modified input or new structure?

CONSTRAINT ANALYSIS:
□ Time limit implications: O(n²) acceptable?
□ Space limit: Can we use extra space?
□ Special constraints: In-place? Immutable?
□ Edge cases: Empty input? Single element?
```

#### Phase 2: Pattern Recognition (2-3 minutes)

```
PRIMARY SIGNALS:
□ Check keyword matches (next greater, all paths, top k, etc.)
□ Identify data structure type
□ Note if input sorted/unsorted
□ Check if asking for all/optimal solutions

PATTERN CANDIDATES:
□ List 2-3 possible patterns
□ For each, estimate time/space complexity
□ Consider implementation difficulty

PATTERN SELECTION:
□ Choose pattern with best complexity
□ Consider edge cases support
□ Verify pattern actually solves problem
```

#### Phase 3: Implementation (15-20 minutes)

```
TEMPLATE APPLICATION:
□ Start with pattern template
□ Adapt to specific problem requirements
□ Add problem-specific logic

IMPLEMENTATION CHECKLIST:
□ Handle base cases
□ Handle edge cases
□ Verify loop bounds
□ Check off-by-one errors
□ Proper memory management (C specific)

OPTIMIZATION:
□ Can any step be optimized?
□ Redundant computations?
□ Early termination possible?
```

#### Phase 4: Testing & Verification (3-5 minutes)

```
TEST CASES:
□ Normal case (from problem)
□ Edge case: empty input
□ Edge case: single element
□ Edge case: all same values
□ Edge case: maximum size

VERIFICATION:
□ Dry run on paper
□ Check time complexity
□ Check space complexity
□ Verify correctness
```

### 15.2 Quick Pattern Selection Guide

**USE THIS DECISION TREE:**

```
┌─────────────────────────────────────────────────────────┐
│                    START: ANALYZE INPUT                 │
└─────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────┐
│ Q1: Is input SORTED?                                    │
│ └─ YES → Two Pointers (90% confidence)                  │
│ └─ NO  → Continue                                       │
└─────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────┐
│ Q2: Looking for SUBARRAY/SUBSTRING?                     │
│ └─ YES, contiguous → Sliding Window                     │
│ └─ NO → Continue                                        │
└─────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────┐
│ Q3: Data structure type?                                │
│ ├─ Linked List → Fast & Slow Pointers                   │
│ ├─ Tree → BFS (level) or DFS (paths)                    │
│ ├─ Graph → BFS (shortest) or DFS (all paths)            │
│ ├─ Intervals → Merge Intervals                          │
│ └─ Other → Continue                                     │
└─────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────┐
│ Q4: What are you finding?                               │
│ ├─ Top/Bottom K elements → Top K (Heap)                 │
│ ├─ All combinations/permutations → Backtracking         │
│ ├─ Next greater/smaller → Monotonic Stack               │
│ ├─ Connected components → Union Find or BFS/DFS         │
│ └─ Cycles → Fast & Slow or DFS                          │
└─────────────────────────────────────────────────────────┘
```

### 15.3 Pattern Priority Matrix

When multiple patterns seem applicable, use this priority:

| Problem Type | 1st Choice | 2nd Choice | 3rd Choice |
|--------------|-----------|------------|------------|
| **Sorted Array + Pair/Sum** | Two Pointers | Hash Map | Binary Search |
| **Subarray with Condition** | Sliding Window | Prefix Sum | Two Pointers |
| **Linked List Middle/Cycle** | Fast & Slow | Hash Set | Two Pass |
| **Tree Level Order** | BFS | DFS + Level Tracking | Recursion |
| **Graph Shortest Path** | BFS | Dijkstra (weighted) | DFS (small) |
| **All Combinations** | Backtracking | DFS | Iteration |
| **Top K Elements** | Heap | Quick Select | Sort |
| **Next Greater** | Monotonic Stack | Brute Force | Sorting |
| **Connected Components** | Union Find | DFS | BFS |

---

## Section 16: Practice Roadmap

### 16.1 Learning Path by Stage

#### Stage 1: Pattern Recognition (Weeks 1-2)
**Goal**: Recognize patterns instantly

**Daily Practice:**
- 2-3 problems per pattern
- Focus on identifying pattern from problem statement
- Use templates, don't worry about optimization

**Recommended Order:**
1. Two Pointers (3 days)
2. Sliding Window (3 days)
3. Fast & Slow Pointers (2 days)
4. BFS (2 days)
5. DFS (2 days)
6. Review (2 days)

#### Stage 2: Template Mastery (Weeks 3-4)
**Goal**: Implement patterns from memory

**Daily Practice:**
- 4-5 problems per day
- Write templates without reference
- Time yourself (20-25 min per problem)

**Focus Areas:**
- All 10 core patterns
- Edge case handling
- Code quality

#### Stage 3: Pattern Combination (Weeks 5-6)
**Goal**: Solve complex problems requiring multiple patterns

**Daily Practice:**
- 3-4 medium/hard problems
- Identify pattern combinations
- Optimize solutions

**Problem Types:**
- Multiple pattern problems
- Optimization challenges
- Real interview questions

#### Stage 4: Speed & Polish (Weeks 7-8)
**Goal**: Interview-ready speed and clean code

**Daily Practice:**
- 5-6 problems per day (mix of easy/medium/hard)
- 15 min for easy, 25 min for medium, 35 min for hard
- Mock interviews

**Focus:**
- Communication while coding
- Edge case discussion
- Complexity analysis

### 16.2 Problem Selection Strategy

**Week-by-Week Problem Count:**

```
Week 1-2: Pattern Learning
├─ Easy: 15-20 problems
├─ Medium: 5-10 problems
└─ Goal: Recognize patterns

Week 3-4: Template Practice
├─ Easy: 10-15 problems
├─ Medium: 20-25 problems
├─ Hard: 5 problems
└─ Goal: Fluent implementation

Week 5-6: Complex Problems
├─ Easy: 5 problems
├─ Medium: 25-30 problems
├─ Hard: 10-15 problems
└─ Goal: Pattern combinations

Week 7-8: Interview Prep
├─ Easy: 10 problems
├─ Medium: 30-35 problems
├─ Hard: 15-20 problems
└─ Goal: Speed and confidence

TOTAL: ~200-250 problems in 8 weeks
```

### 16.3 Daily Study Schedule

**2 Hours per Day:**

```
Time Block 1 (45 min): Problem Solving
├─ Pick pattern of the day
├─ Solve 2-3 problems
└─ Focus on understanding, not speed

Time Block 2 (30 min): Review & Analysis
├─ Review solutions
├─ Identify mistakes
├─ Read optimal solutions
└─ Update template notes

Time Block 3 (30 min): Reinforcement
├─ Solve 1 previous problem from memory
├─ Or solve similar problem
└─ Build muscle memory

Time Block 4 (15 min): Meta-Learning
├─ Review pattern decision tree
├─ Update pattern notes
└─ Track progress
```

### 16.4 Progress Tracking

**Create a tracking spreadsheet:**

| Pattern | Easy | Medium | Hard | Total | Confidence |
|---------|------|--------|------|-------|------------|
| Two Pointers | 5 | 8 | 2 | 15 | 80% |
| Sliding Window | 4 | 6 | 1 | 11 | 70% |
| Fast & Slow | 3 | 4 | 1 | 8 | 85% |
| ... | ... | ... | ... | ... | ... |

**Confidence Levels:**
- 0-30%: Still learning pattern
- 30-60%: Can solve with hints
- 60-80%: Can solve independently
- 80-100%: Can solve quickly and teach others

**Goal**: 80%+ confidence in all patterns

### 16.5 Common Learning Mistakes

**Mistake 1: Jumping to Hard Problems Too Early**
```
❌ Trying hard problems without mastering easy/medium
✅ Build strong foundation with easier problems first
```

**Mistake 2: Not Reviewing Mistakes**
```
❌ Moving to next problem after getting one wrong
✅ Understand why you got it wrong, what pattern you missed
```

**Mistake 3: Memorizing Solutions**
```
❌ Memorizing specific problem solutions
✅ Understanding pattern templates and when to apply them
```

**Mistake 4: Ignoring Time Limits**
```
❌ Spending 2 hours on one problem
✅ 25-30 min per problem, then look at solution
```

**Mistake 5: Only Solving Problems**
```
❌ Just doing problems without analysis
✅ Review, reflect, update notes, teach others
```

### 16.6 Resource Recommendations

**For C Programming:**
- Practice on platforms supporting C
- Focus on memory management
- Study pointer manipulation
- Review common C patterns

**Problem Sources by Pattern:**

**Two Pointers:**
- LeetCode: 15, 16, 18, 26, 27, 75, 167, 283

**Sliding Window:**
- LeetCode: 3, 76, 209, 239, 424, 438, 567

**Fast & Slow Pointers:**
- LeetCode: 141, 142, 202, 234, 876

**Merge Intervals:**
- LeetCode: 56, 57, 252, 253, 435, 986

**Top K Elements:**
- LeetCode: 215, 347, 373, 451, 692, 973

**BFS:**
- LeetCode: 102, 103, 107, 111, 127, 199, 515

**DFS:**
- LeetCode: 104, 113, 200, 207, 257, 543, 695

**Backtracking:**
- LeetCode: 17, 22, 39, 40, 46, 47, 51, 78, 79

**Monotonic Stack:**
- LeetCode: 42, 84, 85, 316, 402, 496, 503, 739, 901

**Union Find:**
- LeetCode: 128, 200, 261, 323, 547, 684, 685, 721, 990

---

## Section 17: Interview Tips & Strategies

### 17.1 Communication Framework

**BEFORE Coding:**

```
1. CLARIFY (2 min)
   "Let me make sure I understand the problem..."
   - Restate problem in your words
   - Ask about edge cases
   - Confirm input/output format
   - Ask about constraints

2. EXAMPLE (1 min)
   "Let me trace through an example..."
   - Use provided example or create your own
   - Walk through step by step
   - Verify your understanding

3. APPROACH (2-3 min)
   "Here's how I'm thinking about this..."
   - Identify the pattern
   - Explain high-level approach
   - Discuss time/space complexity
   - Get buy-in before coding

4. PLAN (1 min)
   "I'll structure my solution like this..."
   - Outline main steps
   - Mention helper functions if needed
   - Discuss data structures needed
```

**DURING Coding:**

```
1. NARRATE
   "Now I'll create a stack to track..."
   - Explain what you're doing
   - Don't code in silence
   - Think out loud

2. STRUCTURE
   - Write clean, readable code
   - Use meaningful variable names
   - Add comments for complex logic
   - Keep consistent style

3. TEST
   - Test with example as you go
   - Mention edge cases you're handling
   - Fix bugs as you spot them
```

**AFTER Coding:**

```
1. VERIFY (2-3 min)
   "Let me trace through this..."
   - Walk through your code with example
   - Check edge cases
   - Look for obvious bugs

2. OPTIMIZE (if time)
   "This could be improved by..."
   - Discuss optimization opportunities
   - Trade-offs between time and space
   - Alternative approaches

3. ANALYZE
   "The time complexity is O(n) because..."
   - Explain time complexity
   - Explain space complexity
   - Justify your analysis
```

### 17.2 Pattern-Specific Interview Tips

**Two Pointers:**
```
✓ Mention why two pointers work (sorted input, opposite directions, etc.)
✓ Explain pointer movement logic clearly
✓ Discuss why this beats brute force O(n²)
```

**Sliding Window:**
```
✓ Explain window expansion/shrinking criteria
✓ Mention how you're tracking window state
✓ Discuss why this is O(n) not O(n²)
```

**Fast & Slow Pointers:**
```
✓ Explain the speed difference (why 2x?)
✓ Mention Floyd's algorithm if applicable
✓ Discuss why this detects cycles
```

**BFS/DFS:**
```
✓ Justify why BFS vs DFS for this problem
✓ Mention queue (BFS) or stack/recursion (DFS)
✓ Discuss space complexity of recursion
```

**Backtracking:**
```
✓ Explain the choose-explore-unchoose pattern
✓ Mention pruning opportunities
✓ Discuss time complexity (often exponential)
```

### 17.3 Handling Tough Situations

**If Stuck:**
1. Re-read the problem
2. Try a simpler example
3. Think about related problems
4. Ask for hints
5. Start with brute force

**If Bug Found:**
1. Don't panic
2. Trace through with example
3. Add debug prints (in interview, trace on paper)
4. Check loop bounds
5. Check edge cases

**If Time Running Out:**
1. Prioritize: working solution > optimal solution
2. State what you'd optimize given more time
3. Write pseudocode for unimplemented parts
4. Explain your thought process

### 17.4 Final Checklist

**Before Interview:**
```
□ Review all 10+ patterns
□ Practice 2-3 problems per pattern
□ Can recognize patterns from keywords
□ Can implement templates from memory
□ Know time/space complexity of each pattern
□ Practiced communicating while coding
```

**During Interview:**
```
□ Listen carefully to problem
□ Clarify before coding
□ Explain approach before implementing
□ Write clean, readable code
□ Test with examples
□ Analyze complexity
□ Ask questions when unsure
```

**After Each Problem:**
```
□ Review what went well
□ Identify what to improve
□ Update pattern notes
□ Practice similar problems
□ Build confidence
```

---

## CONCLUSION

### Key Takeaways

1. **Patterns are Tools, Not Solutions**
   - Understand WHY each pattern works
   - Know WHEN to apply each pattern
   - Practice ADAPTING templates to specific problems

2. **Recognition is Key**
   - 80% of solving is identifying the right pattern
   - Use keywords, constraints, and input type as signals
   - Build pattern recognition through practice

3. **Master the Fundamentals**
   - Solid understanding of data structures
   - Clean code and good habits
   - Complexity analysis

4. **Practice Deliberately**
   - Focus on patterns, not random problems
   - Review mistakes thoroughly
   - Track progress and confidence

5. **Communication Matters**
   - Explain your thinking
   - Justify your choices
   - Stay calm and organized

### Next Steps

1. **Print this guide** or keep it handy
2. **Start with Week 1** problems
3. **Track your progress** in a spreadsheet
4. **Review patterns** weekly
5. **Practice mock interviews** monthly
6. **Teach others** to reinforce learning

### Remember

> "The master has failed more times than the beginner has even tried."

- It's okay to struggle initially
- Patterns become intuitive with practice
- Every problem solved makes you stronger
- Interview success is a marathon, not a sprint

**Good luck with your coding interview preparation!**

---

## Appendix: Quick Reference

### Pattern Complexity Table

| Pattern | Time | Space | Best For |
|---------|------|-------|----------|
| Two Pointers | O(n) | O(1) | Sorted arrays, pairs |
| Sliding Window | O(n) | O(k) | Subarrays/substrings |
| Fast & Slow | O(n) | O(1) | Linked lists, cycles |
| Merge Intervals | O(n log n) | O(n) | Overlapping ranges |
| Top K Elements | O(n log k) | O(k) | K largest/smallest |
| BFS | O(V+E) | O(w) | Shortest path, levels |
| DFS | O(V+E) | O(h) | All paths, cycles |
| Backtracking | O(b^d) | O(d) | All combinations |
| Monotonic Stack | O(n) | O(n) | Next greater/smaller |
| Union Find | O(α(n)) | O(n) | Connected components |

### Pattern Keywords Cheatsheet

```
SORTED → Two Pointers
SUBARRAY/SUBSTRING → Sliding Window
LINKED LIST → Fast & Slow Pointers
INTERVALS/RANGES → Merge Intervals
TOP K / K-TH → Top K Elements (Heap)
LEVEL ORDER / SHORTEST → BFS
ALL PATHS / ALL SOLUTIONS → DFS / Backtracking
NEXT GREATER / SMALLER → Monotonic Stack
CONNECTED / COMPONENTS → Union Find
```

---

**END OF GUIDE**
