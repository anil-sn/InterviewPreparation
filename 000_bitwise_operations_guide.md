# Mastering Bitwise Operations: A Complete Guide
## From Fundamentals to Advanced Patterns

**Author's Note**: Bitwise operations are among the most powerful and efficient tools in programming. This guide will transform you from basic understanding to expert-level bit manipulation skills.

---

## TABLE OF CONTENTS

### PART 1: FUNDAMENTALS
1. Binary Number System Basics
2. Bitwise Operators Deep Dive
3. Bit Manipulation Fundamentals

### PART 2: ESSENTIAL PATTERNS
4. Setting, Clearing, and Toggling Bits
5. Checking and Extracting Bits
6. Counting Bits Pattern
7. Power of Two Pattern
8. XOR Magic Pattern
9. Bit Masking Pattern

### PART 3: ADVANCED PATTERNS
10. Subset Generation Pattern
11. Bit Manipulation Tricks
12. Gray Code Pattern
13. Hamming Distance Pattern

### PART 4: PRACTICAL APPLICATIONS
14. Problem-Solving Framework
15. Common Bitwise Problems
16. Practice Roadmap

---

# PART 1: FUNDAMENTALS

## Section 1: Binary Number System

### 1.1 Understanding Binary

**Decimal vs Binary:**
```
Decimal 13 = Binary 1101

1101 (binary) = 1×2³ + 1×2² + 0×2¹ + 1×2⁰
              = 8 + 4 + 0 + 1
              = 13 (decimal)

Bit positions (right to left):
Position: 3  2  1  0
Value:    8  4  2  1
Binary:   1  1  0  1
```

**Binary Representation in C:**
```c
#include <stdio.h>

// Print binary representation of a number
void printBinary(int n) {
    // For 32-bit integer
    for (int i = 31; i >= 0; i--) {
        int bit = (n >> i) & 1;
        printf("%d", bit);
        if (i % 4 == 0) printf(" ");  // Space every 4 bits for readability
    }
    printf("\n");
}

// Test function
void testPrintBinary() {
    printf("Binary of 13: ");
    printBinary(13);
    // Output: 0000 0000 0000 0000 0000 0000 0000 1101
    
    printf("Binary of -13: ");
    printBinary(-13);
    // Output: 1111 1111 1111 1111 1111 1111 1111 0011 (Two's complement)
}
```

### 1.2 Signed vs Unsigned Numbers

**Representation:**
```c
// 8-bit examples for clarity

unsigned char a = 255;     // 1111 1111 = 255
signed char b = -1;        // 1111 1111 = -1 (two's complement)

// Two's complement for negative numbers:
// -n = ~n + 1 (flip bits and add 1)

// Example: -5
// 5  = 0000 0101
// ~5 = 1111 1010 (flip bits)
// +1 = 1111 1011 (add 1) = -5 in two's complement
```

**Important Ranges:**
```c
// 8-bit
unsigned char:  0 to 255 (0x00 to 0xFF)
signed char:   -128 to 127

// 16-bit
unsigned short: 0 to 65,535 (0x0000 to 0xFFFF)
signed short:  -32,768 to 32,767

// 32-bit
unsigned int:   0 to 4,294,967,295
signed int:    -2,147,483,648 to 2,147,483,647

// 64-bit
unsigned long long: 0 to 18,446,744,073,709,551,615
signed long long:  -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
```

### 1.3 Common Number Representations

```c
// Decimal, Hexadecimal, Binary, Octal representations
void numberRepresentations() {
    int num;
    
    num = 15;        // Decimal
    num = 0xF;       // Hexadecimal (prefix 0x)
    num = 0b1111;    // Binary (prefix 0b, C23 or GCC extension)
    num = 017;       // Octal (prefix 0)
    
    // All represent the same value: 15
    
    printf("Decimal: %d\n", 15);      // 15
    printf("Hex: 0x%X\n", 15);        // 0xF
    printf("Octal: %o\n", 15);        // 17
    
    // Hex to Binary quick reference:
    // 0x0 = 0000    0x8 = 1000
    // 0x1 = 0001    0x9 = 1001
    // 0x2 = 0010    0xA = 1010
    // 0x3 = 0011    0xB = 1011
    // 0x4 = 0100    0xC = 1100
    // 0x5 = 0101    0xD = 1101
    // 0x6 = 0110    0xE = 1110
    // 0x7 = 0111    0xF = 1111
}
```

---

## Section 2: Bitwise Operators

### 2.1 The Six Bitwise Operators

```c
/**
 * BITWISE OPERATORS IN C
 * 
 * &  - AND
 * |  - OR
 * ^  - XOR (exclusive OR)
 * ~  - NOT (complement)
 * << - Left Shift
 * >> - Right Shift
 */

void bitwiseOperatorsBasics() {
    int a = 12;  // 1100 in binary
    int b = 10;  // 1010 in binary
    
    // AND: Both bits must be 1
    printf("12 & 10 = %d\n", a & b);  // 8 (1000)
    /*
        1100  (12)
      & 1010  (10)
      ------
        1000  (8)
    */
    
    // OR: At least one bit must be 1
    printf("12 | 10 = %d\n", a | b);  // 14 (1110)
    /*
        1100  (12)
      | 1010  (10)
      ------
        1110  (14)
    */
    
    // XOR: Bits must be different
    printf("12 ^ 10 = %d\n", a ^ b);  // 6 (0110)
    /*
        1100  (12)
      ^ 1010  (10)
      ------
        0110  (6)
    */
    
    // NOT: Flip all bits
    printf("~12 = %d\n", ~a);  // -13
    /*
        0000 1100  (12)
      ~ 1111 0011  (-13 in two's complement)
    */
    
    // Left Shift: Multiply by 2^n
    printf("12 << 2 = %d\n", a << 2);  // 48
    /*
        1100  (12)
        << 2
        110000  (48) - shifted left by 2, added 0s on right
    */
    
    // Right Shift: Divide by 2^n
    printf("12 >> 2 = %d\n", a >> 2);  // 3
    /*
        1100  (12)
        >> 2
        0011  (3) - shifted right by 2, removed rightmost bits
    */
}
```

### 2.2 Detailed AND Operator (&)

**Truth Table:**
```
A   B   A & B
0   0     0
0   1     0
1   0     0
1   1     1
```

**Common Uses:**
```c
#include <stdio.h>
#include <stdbool.h> // Include for using bool, true, and false

/**
 * AND OPERATOR PATTERNS
 * This program demonstrates various bit manipulation techniques using the bitwise AND (&) operator.
 */

// 1. Check if a specific bit is set (i.e., if it's a 1)
bool isBitSet(int num, int pos) {
    // (1 << pos) creates a "mask" with a single '1' at the desired position 'pos'.
    // For example, if pos is 5, (1 << 5) results in the binary number ...00100000.
    //
    // The bitwise AND (&) compares 'num' with this mask. The result will be non-zero
    // *only if* the bit at 'pos' in 'num' is also a '1'.
    //
    // For example, to check bit 5 in 214 (11010110):
    //   11010110 (num)
    // & 00100000 (mask: 1 << 5)
    // ----------
    //   00100000 (result is 32, which is not 0)
    //
    // The expression '!= 0' converts this numeric result into a boolean (true/false) value.
    return (num & (1 << pos)) != 0;
}

// 2. Clear specific bits (set them to 0) using a mask
int clearBits(int num, int mask) {
    // The bitwise NOT (~) operator inverts every bit in the mask.
    // If a bit in 'mask' is 1, it becomes 0. If it's 0, it becomes 1.
    // This creates an "inverted mask" where the bits we want to clear are 0,
    // and the bits we want to keep are 1.
    //
    // By ANDing the original number 'num' with this inverted mask (~mask), we achieve the following:
    // - Any bit in 'num' corresponding to a '1' in the original 'mask' gets ANDed with '0', clearing it.
    // - Any bit in 'num' corresponding to a '0' in the original 'mask' gets ANDed with '1', leaving it unchanged.
    return num & ~mask;
}

// 3. Extract the 'n' least significant (lower) bits from a number
int getLowerNBits(int num, int n) {
    // To get the lower 'n' bits, we need a mask with 'n' ones at the end.
    // A clever way to create this is by calculating (2^n - 1).
    // (1 << n) calculates 2^n. For example, if n=4, (1 << 4) is 16 (binary 10000).
    // Subtracting 1 from this gives 15 (binary 01111), which is the mask we need.
    int mask = (1 << n) - 1;

    // ANDing the number with this mask will set all higher bits to 0,
    // effectively isolating and returning only the lower 'n' bits.
    return num & mask;
}

// 4. Check if a number is even
bool isEven(int num) {
    // An even number's binary representation always ends with a 0.
    // An odd number's binary representation always ends with a 1.
    //
    // The expression (num & 1) isolates the least significant bit (LSB).
    // - If the LSB is 0, (num & 1) results in 0.
    // - If the LSB is 1, (num & 1) results in 1.
    //
    // We check if the result is 0 to determine if the number is even.
    return (num & 1) == 0;
}

// 5. Check if a number is a power of 2 (e.g., 2, 4, 8, 16, ...)
bool isPowerOfTwo(int num) {
    // A number is a power of two if it's positive and has exactly one '1' bit in its binary representation.
    // Example: 16 is 00010000
    //
    // If we subtract 1 from a power of two, the leading '1' becomes a '0',
    // and all the trailing '0's become '1's.
    // Example: 15 is 00001111
    //
    // When we perform a bitwise AND between a power of two and (itself - 1),
    // the result will always be 0, because there are no positions where both numbers have a '1'.
    //   00010000 (16)
    // & 00001111 (15)
    // ----------
    //   00000000 (0)
    //
    // 'num > 0' is an essential check because 0 would otherwise pass the (num & (num - 1)) == 0 test.
    return num > 0 && (num & (num - 1)) == 0;
}

// Examples to test the functions
void testAND() {
    // Initialize num with the binary value 11010110 (which is 214 in decimal).
    int num = 0b11010110;
    
    // Test isBitSet:
    // Bit 5 is the 6th bit from the right (positions are 0-indexed). In 11010110, it's 1.
    printf("Is bit 5 set? %d\n", isBitSet(num, 5));  // Expected: 1 (true)
    // Bit 0 is the last bit. In 11010110, it's 0.
    printf("Is bit 0 set? %d\n", isBitSet(num, 0));  // Expected: 0 (false)
    
    // Test isEven:
    // 42 is even, so its last bit is 0. (42 & 1) == 0.
    printf("Is 42 even? %d\n", isEven(42));          // Expected: 1 (true)
    // 43 is odd, so its last bit is 1. (43 & 1) != 0.
    printf("Is 43 even? %d\n", isEven(43));          // Expected: 0 (false)
    
    // Test isPowerOfTwo:
    // 16 (00010000) & 15 (00001111) is 0.
    printf("Is 16 power of 2? %d\n", isPowerOfTwo(16));  // Expected: 1 (true)
    // 18 (00010010) & 17 (00010001) is 16 (00010000), which is not 0.
    printf("Is 18 power of 2? %d\n", isPowerOfTwo(18));  // Expected: 0 (false)
}

// Main function to run the tests
int main() {
    testAND();
    return 0;
}
```

### 2.3 Detailed OR Operator (|)

**Truth Table:**
```
A   B   A | B
0   0     0
0   1     1
1   0     1
1   1     1
```

**Common Uses:**
```c
#include <stdio.h>
#include <limits.h> // Required for CHAR_BIT to calculate the number of bits in an integer

/**
 * OR OPERATOR PATTERNS
 * This program demonstrates common bit manipulation patterns using the bitwise OR (|) operator.
 */

// Helper function to print the binary representation of an integer.
// This is not part of the OR patterns but helps visualize the results in the examples.
void printBinary(int n) {
    // We iterate from the most significant bit to the least significant.
    // sizeof(int) * CHAR_BIT gives the total number of bits in an integer (e.g., 32).
    // We start from the bit before that (e.g., bit 31).
    for (int i = sizeof(int) * CHAR_BIT - 1; i >= 0; i--) {
        // Use the right shift (>>) operator to move the bit at position 'i' to the very end.
        // Then, use a bitwise AND (&) with 1 to check if that bit is a 1 or a 0.
        int bit = (n >> i) & 1;
        printf("%d", bit);
        // Add a space after every 4 bits for readability.
        if (i % 4 == 0) printf(" ");
    }
    printf("\n");
}


// 1. Set a specific bit to 1
int setBit(int num, int pos) {
    // (1 << pos) creates a "mask" with a single '1' at the desired position 'pos'.
    // For example, if pos is 2, the mask is ...00000100.
    //
    // The bitwise OR (|) operation compares the bits of 'num' and the mask.
    // The result for each bit is 1 if *either* of the operand bits is 1.
    // - ORing a bit with 0 leaves it unchanged (0|0=0, 1|0=1).
    // - ORing a bit with 1 sets it to 1 (0|1=1, 1|1=1).
    //
    // This reliably "turns on" the bit at 'pos' without affecting any other bits.
    return num | (1 << pos);
}

// 2. Set multiple bits to 1 using a mask
int setMultipleBits(int num, int mask) {
    // The 'mask' is an integer where the bits we want to set are already 1s.
    // For example, a mask of 0b111 (or 7) indicates we want to set bits 0, 1, and 2.
    //
    // The OR operation will set any bit in 'num' to 1 if the corresponding bit in 'mask' is 1.
    // Bits in 'num' where the mask's bit is 0 will be left untouched.
    return num | mask;
}

// 3. Combine multiple flags into a single value
int combineFlags(int flag1, int flag2, int flag3) {
    // This is a very common use case for the OR operator.
    // Flags are typically defined as unique powers of two (e.g., 1, 2, 4, 8),
    // which means each flag corresponds to a single bit position.
    //
    // By ORing them together, we create a single integer where multiple bits are set,
    // each representing an active flag. This is an efficient way to store a set of boolean options.
    return flag1 | flag2 | flag3;
}

// 4. Round up to the next power of 2 (a classic bit-twiddling algorithm)
unsigned int nextPowerOf2(unsigned int n) {
    // This algorithm works by smearing the most significant '1' bit across all the bits to its right.
    
    // First, decrement n. This is crucial for cases where n is already a power of two.
    // If n=16, we want the result to be 16. Without this step, we would get 32.
    // By starting with 15 (0...01111), the algorithm will correctly produce 16.
    n--;

    // The following sequence of operations "smears" the highest set bit to the right.
    // Let's trace n = 12 (after n--, it becomes 11, which is 0000 1011)
    n |= n >> 1; // 0000 1011 | 0000 0101 = 0000 1111
    n |= n >> 2; // 0000 1111 | 0000 0011 = 0000 1111
    n |= n >> 4; // 0000 1111 | 0000 0000 = 0000 1111
    n |= n >> 8; // (and so on...)
    n |= n >> 16;// This ensures the smearing works for all bits in a 32-bit integer.
    
    // After the smearing, n is now a sequence of all 1s from the original highest bit down.
    // For our example, it's 15 (0000 1111).
    
    // Incrementing this value gives the next power of two. (15 + 1 = 16).
    n++;
    return n;
}

// Examples to test the functions
void testOR() {
    // Initialize num with the binary value 10000000 (128 decimal).
    int num = 0b10000000;
    
    printf("Original number: ");
    printBinary(num); // 1000 0000

    printf("Set bit 2:       ");
    // Should turn on the 3rd bit from the right.
    printBinary(setBit(num, 2));  // Expected: 1000 0100 (132)
    
    printf("Set bits 0,1,2:  ");
    // The mask 0b111 has the first three bits set to 1.
    int mask = 0b111;
    // The result should have the original bit 7 and the new bits 0, 1, 2 set.
    printBinary(setMultipleBits(num, mask));  // Expected: 1000 0111 (135)
    
    // Flag examples using preprocessor definitions for readability.
    #define READ  0x01   // Binary 0001
    #define WRITE 0x02   // Binary 0010
    #define EXEC  0x04   // Binary 0100
    
    // Combine READ and WRITE permissions.
    // 0001 | 0010 = 0011 (which is 3 in decimal).
    int permissions = READ | WRITE;
    // 0x%X prints the number in hexadecimal format. 3 in hex is 3.
    printf("\nCombined Permissions (READ|WRITE): 0x%X\n", permissions);

    printf("Next power of 2 for 12 is: %u\n", nextPowerOf2(12)); // Expected: 16
    printf("Next power of 2 for 16 is: %u\n", nextPowerOf2(16)); // Expected: 16
    printf("Next power of 2 for 17 is: %u\n", nextPowerOf2(17)); // Expected: 32
}

// Main function to run the tests
int main() {
    testOR();
    return 0;
}
```

### 2.4 Detailed XOR Operator (^)

**Truth Table:**
```
A   B   A ^ B
0   0     0
0   1     1
1   0     1
1   1     0
```

**XOR Properties (CRITICAL):**
```c
#include <stdio.h>
#include <stdlib.h> // For malloc and free
#include <string.h> // For strlen

// Helper function to print the binary representation of an integer for visualization.
void printBinary(int n) {
    for (int i = 7; i >= 0; i--) { // Print 8 bits for simplicity
        printf("%d", (n >> i) & 1);
    }
}

/**
 * XOR PROPERTIES - These are the foundation for all XOR patterns.
 * The XOR (^) operator, or "exclusive OR", returns 1 if the two bits are different, and 0 if they are the same.
 * 0 ^ 0 = 0
 * 0 ^ 1 = 1
 * 1 ^ 0 = 1
 * 1 ^ 1 = 0
 */
void xorProperties() {
    printf("--- 1. Fundamental XOR Properties ---\n");
    int a = 42; // Binary: 00101010
    int b = 17; // Binary: 00010001
    int c = 99; // Binary: 01100011

    // Property 1: XOR with 0 is the identity operation. It doesn't change the number.
    // (a ^ 0) = a
    printf("1. Identity:      %d ^ 0 = %d\n", a, a ^ 0);

    // Property 2: XOR with itself results in 0.
    // (a ^ a) = 0
    printf("2. Self-Inverse:  %d ^ %d = %d\n", a, a, a ^ a);

    // Property 3: Commutativity. The order of operands doesn't matter.
    // (a ^ b) = (b ^ a)
    printf("3. Commutative:   %d ^ %d = %d  and  %d ^ %d = %d\n", a, b, a ^ b, b, a, b ^ a);

    // Property 4: Associativity. Grouping of operands doesn't matter.
    // (a ^ b) ^ c = a ^ (b ^ c)
    printf("4. Associative:   (%d ^ %d) ^ %d = %d\n", a, b, c, (a ^ b) ^ c);

    // Property 5: The "Cancellation" property. XORing with a number twice cancels it out, returning the original value.
    // This is the basis for XOR swapping and simple encryption.
    // (a ^ b ^ b) = a ^ (b ^ b) = a ^ 0 = a
    printf("5. Cancellation:  (%d ^ %d) ^ %d = %d\n", a, b, b, a ^ b ^ b);
}

/**
 * XOR USE CASES AND CREATIVE SOLUTIONS
 */
void xorUseCases() {
    printf("\n--- 2. Classic Use Case: Swapping Variables ---\n");
    // This is a classic "trick" to swap two variables without needing a temporary variable.
    // It relies entirely on the self-inverse and cancellation properties of XOR.
    int x = 5, y = 10;
    printf("Before swap: x = %d, y = %d\n", x, y);
    x = x ^ y; // x now holds the combined, unique bits of the original x and y.
    y = x ^ y; // y = (original_x ^ original_y) ^ original_y  -->  y becomes original_x
    x = x ^ y; // x = (original_x ^ original_y) ^ new_y (which is original_x) --> x becomes original_y
    printf("After swap:  x = %d, y = %d\n", x, y);

    printf("\n--- 3. Toggling a Specific Bit ---\n");
    // XORing with a mask toggles the bits. If a bit in the number is 1, it becomes 0; if it's 0, it becomes 1.
    // This is because (1 ^ 1 = 0) and (0 ^ 1 = 1). Other bits are XORed with 0 and remain unchanged.
    int num = 0b11010110; // 214
    int mask = 1 << 4;    // Mask to toggle the 4th bit: 00010000
    printf("Original number: "); printBinary(num); printf(" (214)\n");
    num = num ^ mask;
    printf("Toggled bit 4:   "); printBinary(num); printf(" (230)\n");
    num = num ^ mask; // Toggle it again
    printf("Toggled back:    "); printBinary(num); printf(" (214)\n");

    printf("\n--- 4. Finding the Single Unique Number in an Array ---\n");
    // If you XOR all elements in an array where every number appears twice except for one,
    // the pairs will cancel each other out (a ^ a = 0), leaving only the unique number.
    // Example: (5 ^ 8 ^ 2 ^ 5 ^ 8) = (5 ^ 5) ^ (8 ^ 8) ^ 2 = 0 ^ 0 ^ 2 = 2
    int arr[] = {5, 8, 2, 11, 8, 5, 11};
    int unique = 0;
    for (size_t i = 0; i < 7; i++) {
        unique = unique ^ arr[i];
    }
    printf("In {5, 8, 2, 11, 8, 5, 11}, the unique number is: %d\n", unique);

    printf("\n--- 5. Simple XOR Encryption/Decryption ---\n");
    // This simple cipher encrypts a message by XORing each character with a repeating key.
    // The exact same function decrypts it because of the cancellation property: (message ^ key) ^ key = message.
    // NOTE: This is NOT cryptographically secure, but demonstrates the principle.
    char message[] = "Hello World";
    char key[] = "SECRET";
    printf("Original message:  %s\n", message);
    
    // "Encrypt"
    for (size_t i = 0; i < strlen(message); i++) {
        message[i] = message[i] ^ key[i % strlen(key)];
    }
    printf("Encrypted message: %s\n", message);
    
    // "Decrypt"
    for (size_t i = 0; i < strlen(message); i++) {
        message[i] = message[i] ^ key[i % strlen(key)];
    }
    printf("Decrypted message: %s\n", message);
    
    printf("\n--- 6. Finding the Missing Number in a Sequence ---\n");
    // Given an array of n-1 unique integers in the range [0, n-1], find the missing one.
    // We can XOR all numbers from 0 to n, and then XOR that result with all numbers in the array.
    // The numbers present in both will cancel out, leaving only the missing number.
    // Example: n=4, array={0, 1, 3}.
    // XOR of all nums: (0^1^2^3). XOR of array: (0^1^3).
    // Combined: (0^1^2^3) ^ (0^1^3) = (0^0)^(1^1)^2^(3^3) = 0^0^2^0 = 2. The missing number is 2.
    int sequence[] = {0, 1, 3, 4, 2, 6}; // Missing 5, n=7
    int n = 7;
    int missing = 0;
    // XOR all numbers from 0 to n (inclusive of 0, exclusive of n)
    for (int i = 0; i < n; i++) {
        missing ^= i;
    }
    // XOR with all numbers in the actual sequence
    for (size_t i = 0; i < sizeof(sequence)/sizeof(int); i++) {
        missing ^= sequence[i];
    }
    printf("In the sequence {0,1,3,4,2,6}, the missing number is: %d\n", missing);
}

// Main function to run the demonstrations
int main() {
    xorProperties();
    xorUseCases();
    return 0;
}
```

**Common XOR Uses:**
```c
#include <stdio.h>
#include <stdbool.h> // For using the bool type and true/false values

// Helper function to print the binary representation of an integer for visualization.
void printBinary(int n) {
    // We iterate from left to right (most significant bit to least significant).
    // This example prints 8 bits, which is suitable for char-sized integers or small test values.
    for (int i = 7; i >= 0; i--) {
        // (n >> i) shifts the bit at position 'i' to the very end (position 0).
        // (& 1) isolates this bit. The result is either 1 or 0.
        printf("%d", (n >> i) & 1);
    }
}

/**
 * XOR OPERATOR PATTERNS
 * Demonstrates several powerful algorithms and tricks using the bitwise XOR (^) operator.
 */

// 1. Toggle a specific bit (change 0 to 1, or 1 to 0)
int toggleBit(int num, int pos) {
    // (1 << pos) creates a mask with only the bit at 'pos' set to 1.
    // The XOR operator flips a bit if the corresponding mask bit is 1, and leaves it unchanged if the mask bit is 0.
    // - If the bit in 'num' is 1: 1 ^ 1 = 0 (toggles off)
    // - If the bit in 'num' is 0: 0 ^ 1 = 1 (toggles on)
    // - All other bits in 'num' are XORed with 0, which leaves them unchanged (x ^ 0 = x).
    return num ^ (1 << pos);
}

// 2. Find the single number in an array where all other numbers appear exactly twice
int singleNumber(int* nums, int numsSize) {
    // This algorithm relies on two key properties of XOR:
    // 1. XORing a number with itself results in 0 (a ^ a = 0).
    // 2. XORing a number with 0 results in the number itself (a ^ 0 = a).
    int result = 0;
    for (int i = 0; i < numsSize; i++) {
        // As we XOR all elements together, every pair of identical numbers will cancel each other out.
        // For example: (4 ^ 1 ^ 2 ^ 1 ^ 2) can be rearranged (due to associativity) as:
        // 4 ^ (1 ^ 1) ^ (2 ^ 2) = 4 ^ 0 ^ 0 = 4.
        result ^= nums[i];
    }
    // The final result will be the only number that did not have a pair.
    return result;
}

// 3. Swap the values of two integer pointers without using a temporary variable
void swapXOR(int* a, int* b) {
    // This check is crucial. If 'a' and 'b' point to the same memory location,
    // the first step (*a = *a ^ *a) would set the value to 0, and the original value would be lost.
    if (a != b) {
        // Step 1: *a now holds the result of (*a ^ *b).
        *a = *a ^ *b;
        // Step 2: *b becomes (*a ^ *b) ^ *b = *a (the original value of *a).
        // This is because the new *a contains the original *a and *b, and XORing with the original *b cancels it out.
        *b = *a ^ *b;
        // Step 3: *a becomes (*a ^ *b) ^ *a = *b (the original value of *b).
        // The new *a contains (original *a ^ original *b), and the new *b is (original *a).
        // So this step is (original *a ^ original *b) ^ (original *a), which cancels out original *a, leaving original *b.
        *a = *a ^ *b;
    }
}

// 4. Check if two integers have different signs
bool differentSigns(int a, int b) {
    // The sign of an integer is stored in its most significant bit (MSB).
    // - For positive numbers (and zero), the MSB is 0.
    // - For negative numbers, the MSB is 1.
    //
    // When we XOR 'a' and 'b':
    // - If signs are the same (0^0 or 1^1), the resulting MSB will be 0. The number will be positive.
    // - If signs are different (0^1 or 1^0), the resulting MSB will be 1. The number will be negative.
    //
    // Therefore, checking if (a ^ b) is less than 0 is a highly efficient way to check if they had different signs.
    return (a ^ b) < 0;
}

// 5. Find the missing number in an array containing numbers from 0 to n, with one number missing
int missingNumber(int* nums, int numsSize) {
    // 'numsSize' is effectively 'n' in this context (the value that should be at the end of the full sequence).
    // This clever algorithm finds the missing number by canceling out pairs of numbers.
    // The complete set of numbers should be {0, 1, ..., n} and the indices of the array are {0, 1, ..., n-1}.
    
    // We initialize the result with n because 'n' is the one number in the complete set that doesn't have a corresponding index.
    int result = numsSize;
    
    for (int i = 0; i < numsSize; i++) {
        // In each step, we XOR the result with an index 'i' and a value from the array 'nums[i]'.
        // If the array were complete and sorted, nums[i] would be equal to i, and (i ^ nums[i]) would be 0.
        // Because one number is missing, one index 'i' will never match its value.
        // All other pairs of (index, value) will eventually cancel each other out.
        // The final result will be the XOR of 'n' (our initial value) and the missing number.
        // Example: nums = {0, 1, 3}, n=3.
        // result starts at 3.
        // i=0: result = 3 ^ 0 ^ nums[0](0) = 3 ^ 0 ^ 0 = 3
        // i=1: result = 3 ^ 1 ^ nums[1](1) = 3 ^ 1 ^ 1 = 3
        // i=2: result = 3 ^ 2 ^ nums[2](3) = 3 ^ 2 ^ 3 = (3^3)^2 = 0^2 = 2
        // The missing number is 2.
        result ^= i ^ nums[i];
    }
    return result;
}

// Examples to test the functions
void testXOR() {
    // Start with 172 (binary 10101100)
    int num = 0b10101100;
    printf("Original num: "); printBinary(num); printf("\n");
    
    // toggleBit should flip the bit at position 2 (the 3rd from the right) from 1 to 0.
    // 10101100 -> 10101000
    printf("Toggle bit 2: ");
    printBinary(toggleBit(num, 2));
    printf("\n\n");
    
    // In this array, 1 and 2 appear twice, while 4 appears once.
    int arr[] = {4, 1, 2, 1, 2};
    printf("Array is {4, 1, 2, 1, 2}\n");
    printf("Single number: %d\n\n", singleNumber(arr, 5)); // Expected: 4
    
    // The array contains numbers from 0 to 5, but is missing the number 2. (n=5)
    int missing[] = {0, 1, 3, 4, 5};
    printf("Array is {0, 1, 3, 4, 5}\n");
    printf("Missing number: %d\n\n", missingNumber(missing, 5)); // Expected: 2

    printf("Signs of 10 and -5 are different: %s\n", differentSigns(10, -5) ? "true" : "false");
    printf("Signs of 10 and 5 are different: %s\n", differentSigns(10, 5) ? "true" : "false");
}

int main() {
    testXOR();
    return 0;
}
```

### 2.5 Detailed NOT Operator (~)

**Operation:**
```
~0 = 1
~1 = 0
```

**Important Note:**
```c
#include <stdio.h>

// Helper function to print the binary representation of an integer for visualization.
void printBinary(int n) {
    // We iterate from left to right (most significant bit to least significant).
    // This example prints 8 bits for simplicity and clarity.
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
}

/**
 * NOT OPERATOR FUNDAMENTALS
 * The bitwise NOT (~) operator, also known as the complement operator, inverts every single bit of its operand.
 * Every 0 becomes a 1, and every 1 becomes a 0.
 */
void notOperatorFundamentals() {
    printf("--- NOT Operator Fundamentals ---\n");
    // Using an unsigned char (an 8-bit integer) for a clear, small example.
    unsigned char a = 5; // Binary: 0000 0101
    
    printf("Original 'a' (5):         "); printBinary(a); printf("\n");
    
    // Applying the NOT operator flips all bits.
    // 0000 0101 becomes 1111 1010
    // As an unsigned 8-bit integer, 1111 1010 is 250 in decimal.
    printf("~a (~5) as unsigned char: "); printBinary(~a); printf(" (Decimal: %u)\n", (unsigned char)~a);

    // --- Two's Complement ---
    // This is the standard way signed integers are represented in most computers.
    // The formula to get the negative of a number is: -n = ~n + 1
    int b = 5; // In a 32-bit system: 0...0000 0101
               // ~b is then:             1...1111 1010
               // ~b + 1 is:              1...1111 1011 (which is -5 in two's complement)
    printf("\nTwo's complement for -5: ~5 + 1 = %d\n", ~b + 1);

    // --- Creating Masks ---
    // The NOT operator is extremely useful for creating bitmasks.
    
    // A mask of all 1s can be created by NOTing 0.
    // 0 is all zero bits, so ~0 is all one bits.
    unsigned int allOnes = ~0;
    // On a 32-bit system, this will be 32 ones, which is 0xFFFFFFFF in hexadecimal.
    printf("Mask of all 1s (~0): 0x%X\n", allOnes);
    
    // Create a mask to isolate the lower 'n' bits.
    int n = 4;
    // Step 1: ~0 gives all 1s (e.g., 1111 1111)
    // Step 2: ~0 << n shifts the ones 'n' positions to the left, filling the right with zeros (e.g., 1111 0000 for n=4)
    // Step 3: ~(...) inverts this result, giving a mask with only the lower 'n' bits set to 1 (e.g., 0000 1111)
    unsigned int lowerNMask = ~(~0 << n);
    printf("Mask for lower %d bits:  0x%X (Binary: ", n, lowerNMask); printBinary(lowerNMask); printf(")\n");
}


/**
 * COMMON USES OF THE NOT OPERATOR
 * The NOT operator is rarely used alone; it's most powerful when combined with other bitwise operators like AND (&).
 */

// 1. Clear a specific bit (set it to 0)
int clearBit(int num, int pos) {
    // This is a fundamental bit manipulation technique.
    // Step 1: (1 << pos) creates a mask with a '1' only at the target position (e.g., 0001 0000 for pos=4).
    // Step 2: ~(1 << pos) inverts this mask, creating a new mask with a '0' only at the target position and '1's everywhere else (e.g., 1110 1111).
    // Step 3: num & ... performs a bitwise AND.
    //   - The '0' in the mask guarantees the bit at 'pos' in the result is 0.
    //   - The '1's in the mask ensure that all other bits from 'num' are preserved.
    return num & ~(1 << pos);
}

// 2. Clear all lower 'n' bits (set them to 0)
int clearLowerNBits(int num, int n) {
    // This function sets the 'n' least significant bits to zero.
    // Step 1: ~0 creates a mask of all ones (e.g., 1111 1111).
    // Step 2: (~0 << n) shifts the ones 'n' positions to the left, creating a mask where the lower 'n' bits are zero
    //         and all higher bits are one (e.g., for n=4, the mask is 1111 0000).
    // Step 3: num & ... performs a bitwise AND. The 0s in the mask's lower bits will clear the corresponding bits in 'num',
    //         while the 1s in the mask's upper bits will preserve the corresponding bits in 'num'.
    return num & (~0 << n);
}

// 3. Invert all bits of a number
int invertBits(int num) {
    // This is the most direct use of the NOT operator. It simply returns the bitwise complement of the number.
    return ~num;
}

// Main function to run the examples
int main() {
    notOperatorFundamentals();
    
    printf("\n--- Common NOT Use Cases ---\n");
    int num = 0b11010110; // 214
    int position = 5;
    int n_bits = 4;

    printf("Original number:      "); printBinary(num); printf(" (214)\n");
    
    int cleared = clearBit(num, position);
    printf("After clearing bit %d: ", position); printBinary(cleared); printf(" (%d)\n", cleared);
    
    int lower_cleared = clearLowerNBits(num, n_bits);
    printf("After clearing lower %d: ", n_bits); printBinary(lower_cleared); printf(" (%d)\n", lower_cleared);
    
    int inverted = invertBits(num);
    // The decimal value depends on whether it's interpreted as signed (-215) or unsigned.
    printf("After inverting all:  "); printBinary(inverted); printf(" (%d)\n", inverted);

    return 0;
}
```

### 2.6 Left Shift Operator (<<)

**Operation:**
```
n << k = n * 2^k
```

**Detailed Explanation:**
```c
#include <stdio.h>

// Helper function to print the binary representation of an integer for visualization.
void printBinary(int n) {
    // This example prints 8 bits for simplicity and clarity.
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
}

/**
 * LEFT SHIFT (<<) OPERATOR FUNDAMENTALS
 * The left shift operator `<<` shifts the bits of its left operand to the left
 * by the number of positions specified by its right operand.
 * The vacant bit positions on the right are filled with zeros.
 */
void leftShiftFundamentals() {
    printf("--- Left Shift Fundamentals ---\n");
    int num = 5; // Binary (8-bit): 0000 0101
    
    printf("Original number %d: ", num); printBinary(num); printf("\n");

    // Shifting by 0 positions results in no change.
    printf("%d << 0 = %2d  | Binary: ", num, num << 0); printBinary(num << 0); printf("\n");
    
    // Shifting left by 1 position. The MSB is discarded, and a 0 is added as the LSB.
    // 0000 0101 -> 0000 1010
    printf("%d << 1 = %2d  | Binary: ", num, num << 1); printBinary(num << 1); printf("\n");
    
    // 0000 0101 -> 0001 0100
    printf("%d << 2 = %2d  | Binary: ", num, num << 2); printBinary(num << 2); printf("\n");
    
    // 0000 0101 -> 0010 1000
    printf("%d << 3 = %2d  | Binary: ", num, num << 3); printBinary(num << 3); printf("\n");
    
    printf("\n--- Mathematical Equivalence ---\n");
    // Each left shift by one position effectively multiplies the number by 2.
    // This is often much faster than using the standard multiplication operator.
    printf("%d * 2  (same as %d << 1) = %d\n", num, num, num << 1);
    printf("%d * 4  (same as %d << 2) = %d\n", num, num, num << 2);
    printf("%d * 8  (same as %d << 3) = %d\n", num, num, num << 3);
    
    printf("\n--- Creating a Bitmask ---\n");
    // A very common use is to create a mask with a single '1' at a specific position.
    // Start with 1 (0000 0001) and shift it left.
    int pos = 3;
    int mask = 1 << pos;  // 0000 0001 << 3  ->  0000 1000 (which is 8)
    printf("Mask for bit %d (1 << %d): %d (Binary: ", pos, pos, mask); printBinary(mask); printf(")\n");
    
    // --- WARNING ---
    // Shifting by a number of bits greater than or equal to the width of the type is UNDEFINED BEHAVIOR.
    // For a 32-bit int, `1 << 31` is valid, but `1 << 32` or `1 << 33` is not.
    // The result can be unpredictable and is a common source of bugs.
}

/**
 * COMMON LEFT SHIFT PATTERNS
 */

// 1. Create a value with a single bit set at a specific position.
// This is the primary way to generate masks for other bitwise operations (AND, OR, XOR).
int createBit(int pos) {
    // Starts with the integer 1 (binary ...0001) and shifts this single '1' bit
    // 'pos' places to the left.
    return 1 << pos;
}

// 2. Create a mask with the 'n' least significant bits set to 1.
// Useful for extracting the lower 'n' bits of a number when used with the AND operator.
unsigned int createMaskForLowerNBits(int n) {
    // This is a two-step process:
    // 1. (1 << n): Creates a '1' at position 'n'. For n=4, this is 16 (binary 0001 0000).
    // 2. (... - 1): Subtracting 1 from a power of two flips the leading '1' to a '0'
    //    and all trailing '0's to '1's. For 16, this becomes 15 (binary 0000 1111).
    return (1 << n) - 1;
}

// 3. Calculate a power of 2 quickly.
// 2^n is equivalent to shifting 1 by n bits.
int powerOf2(int n) {
    // 2^0 = 1 << 0 = 1
    // 2^1 = 1 << 1 = 2
    // 2^5 = 1 << 5 = 32
    return 1 << n;
}

// 4. Multiply a number by a power of 2.
// Multiplying by 2^power is equivalent to left-shifting by 'power'.
int multiplyByPowerOf2(int num, int power) {
    // Example: num * 8 is the same as num * (2^3), which is num << 3.
    return num << power;
}

// Main function to test the patterns
void testLeftShiftPatterns() {
    printf("\n--- Testing Common Left Shift Patterns ---\n");
    
    int position = 5;
    printf("1. createBit(%d):          %d (Binary: ", position, createBit(position)); printBinary(createBit(position)); printf(")\n");
    
    int num_bits = 4;
    printf("2. createMaskForLowerNBits(%d): %d (Binary: ", num_bits, createMaskForLowerNBits(num_bits)); printBinary(createMaskForLowerNBits(num_bits)); printf(")\n");
    
    int exponent = 6;
    printf("3. powerOf2(%d) [2^%d]:     %d\n", exponent, exponent, powerOf2(exponent));
    
    int base_num = 7;
    int power = 3; // Multiply by 2^3 = 8
    printf("4. multiplyByPowerOf2(%d, %d): %d\n", base_num, power, multiplyByPowerOf2(base_num, power));
}


int main() {
    leftShiftFundamentals();
    testLeftShiftPatterns();
    return 0;
}
```

### 2.7 Right Shift Operator (>>)

**Operation:**
```
n >> k = n / 2^k (for positive numbers)
```

**Logical vs Arithmetic Shift:**
```c
#include <stdio.h>
#include <limits.h> // For CHAR_BIT

// Helper function to print the binary representation of an integer for visualization.
void printBinary(int n) {
    // We iterate from the most significant bit to the least significant.
    // sizeof(int) * CHAR_BIT gives the total number of bits in an int (usually 32).
    // This loop prints all bits of the integer.
    for (int i = (sizeof(int) * CHAR_BIT - 1); i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i > 0 && i % 8 == 0) printf(" "); // Add space for readability
    }
}

/**
 * RIGHT SHIFT (>>) OPERATOR FUNDAMENTALS
 * The right shift operator `>>` shifts the bits of its left operand to the right
 * by the number of positions specified by its right operand.
 * How the vacant bits on the left are filled depends on the type of the variable.
 */
void rightShiftFundamentals() {
    printf("--- Right Shift Fundamentals ---\n");

    // --- 1. UNSIGNED types use a LOGICAL SHIFT ---
    // A logical shift always fills the vacant bits on the left with zeros.
    printf("Unsigned (Logical Shift):\n");
    unsigned int positive = 20; // Binary (32-bit): 0000...0001 0100
    printf("Original 20: "); printBinary(positive); printf("\n");
    
    unsigned int p_shifted1 = positive >> 1; // 10 (0...0000 1010)
    printf("      20 >> 1: "); printBinary(p_shifted1); printf(" (%u)\n", p_shifted1);
    
    unsigned int p_shifted2 = positive >> 2; // 5 (0...0000 0101)
    printf("      20 >> 2: "); printBinary(p_shifted2); printf(" (%u)\n", p_shifted2);

    // --- 2. SIGNED types typically use an ARITHMETIC SHIFT ---
    // An arithmetic shift fills the vacant bits by copying the sign bit (the most significant bit).
    // This process is called "sign extension" and it preserves the number's sign.
    // Note: This behavior is technically "implementation-defined" by the C standard, but is nearly universal on modern systems.
    printf("\nSigned (Arithmetic Shift):\n");
    int negative = -20; // Binary (32-bit two's complement): 1111...1110 1100
    printf("Original -20: "); printBinary(negative); printf("\n");
    
    int n_shifted1 = negative >> 1; // -10 (1111...1111 0110) - the '1' is copied
    printf("     -20 >> 1: "); printBinary(n_shifted1); printf(" (%d)\n", n_shifted1);
    
    int n_shifted2 = negative >> 2; // -5 (1111...1111 1011) - the '1' is copied again
    printf("     -20 >> 2: "); printBinary(n_shifted2); printf(" (%d)\n", n_shifted2);

    // --- 3. Mathematical Equivalence: Fast Division ---
    // For positive integers, right shifting is a fast way to perform integer division by a power of 2.
    printf("\nFast Division by Powers of 2:\n");
    int num = 100;
    printf("100 / 2 (100 >> 1) = %d\n", num >> 1);  // 50
    printf("100 / 4 (100 >> 2) = %d\n", num >> 2);  // 25
    // For integer division, 100 / 8 = 12. The remainder is discarded.
    printf("100 / 8 (100 >> 3) = %d\n", num >> 3);  // 12
}

/**
 * COMMON RIGHT SHIFT PATTERNS
 */

// 1. Extract a specific range of bits from a number.
// For example, extract 'count' bits starting from bit 'start'.
int extractBits(int num, int start, int count) {
    // Step 1: (num >> start)
    // Shift the number to the right so the desired bits are at the very end (the least significant positions).
    //
    // Step 2: ((1 << count) - 1)
    // This creates a mask with 'count' number of 1s. For example, if count=3, the mask is 0...0111.
    //
    // Step 3: (...) & (...)
    // The bitwise AND with the mask isolates only the bits we are interested in, clearing all higher bits.
    return (num >> start) & ((1 << count) - 1);
}

// 2. Divide a number by a power of 2.
// num / (2^power) is equivalent to num >> power.
int divideByPowerOf2(int num, int power) {
    // This is generally faster than using the division operator.
    // For negative numbers, the result rounds towards negative infinity (e.g., -5 >> 1 = -3),
    // which can be different from the '/' operator's rounding (which rounds towards zero, e.g., -5 / 2 = -2).
    return num >> power;
}

// 3. Get the value of a single bit at a specific position.
int getBit(int num, int pos) {
    // Step 1: (num >> pos)
    // Shift the bit at 'pos' all the way to the rightmost position (position 0).
    //
    // Step 2: (...) & 1
    // ANDing with 1 (binary ...0001) isolates this bit. The result will be either 1 or 0.
    return (num >> pos) & 1;
}

// 4. Remove the lower 'n' bits of a number.
// This is effectively the same as dividing by 2^n.
int removeLowerBits(int num, int n) {
    // Shifting right by 'n' positions discards the 'n' least significant bits.
    return num >> n;
}

// Main function to test the patterns
void testRightShiftPatterns() {
    printf("\n--- Testing Common Right Shift Patterns ---\n");
    int num = 0b11010110; // 214

    printf("Original number: 214 (Binary 11010110)\n");
    printf("1. Get bit at position 5: %d\n", getBit(num, 5)); // Should be 1
    printf("2. Divide 214 by 4 (2^2): %d\n", divideByPowerOf2(num, 2)); // 214 / 4 = 53
    printf("3. Remove lower 4 bits:   %d (Binary ", removeLowerBits(num, 4)); printBinary(removeLowerBits(num, 4) & 0xFF); printf(")\n"); // 1101 -> 13
    
    // Extract 3 bits starting from position 2 (the bits are '101')
    printf("4. Extract 3 bits from pos 2: %d (Binary ", extractBits(num, 2, 3)); printBinary(extractBits(num, 2, 3)); printf(")\n"); // Should be 5
}

int main() {
    rightShiftFundamentals();
    testRightShiftPatterns();
    return 0;
}
```

---

## Section 3: Bit Manipulation Fundamentals

### 3.1 Essential Bit Operations

```c
/**
 * FUNDAMENTAL BIT OPERATIONS
 * Master these - they're the building blocks!
 */

// 1. SET a bit (make it 1)
int setBit(int num, int pos) {
    return num | (1 << pos);
}

// 2. CLEAR a bit (make it 0)
int clearBit(int num, int pos) {
    return num & ~(1 << pos);
}

// 3. TOGGLE a bit (flip it)
int toggleBit(int num, int pos) {
    return num ^ (1 << pos);
}

// 4. CHECK if bit is set
bool checkBit(int num, int pos) {
    return (num & (1 << pos)) != 0;
}

// 5. MODIFY a bit to specific value
int modifyBit(int num, int pos, bool value) {
    // Clear bit first, then set if needed
    int mask = ~(1 << pos);
    return (num & mask) | (value << pos);
}

// Test all operations
void testBasicOperations() {
    int num = 0b10101100;  // 172
    
    printf("Original: ");
    printBinary(num);
    
    printf("Set bit 0: ");
    printBinary(setBit(num, 0));  // 10101101
    
    printf("Clear bit 3: ");
    printBinary(clearBit(num, 3));  // 10100100
    
    printf("Toggle bit 2: ");
    printBinary(toggleBit(num, 2));  // 10101000
    
    printf("Check bit 5: %d\n", checkBit(num, 5));  // 1 (set)
    printf("Check bit 0: %d\n", checkBit(num, 0));  // 0 (clear)
    
    printf("Modify bit 1 to 1: ");
    printBinary(modifyBit(num, 1, 1));  // 10101110
}
```

### 3.2 Range Operations

```c
/**
 * OPERATIONS ON RANGES OF BITS
 */

// 1. SET multiple bits in range [start, start+count)
int setRange(int num, int start, int count) {
    int mask = ((1 << count) - 1) << start;
    return num | mask;
}

// 2. CLEAR multiple bits in range
int clearRange(int num, int start, int count) {
    int mask = ((1 << count) - 1) << start;
    return num & ~mask;
}

// 3. TOGGLE multiple bits in range
int toggleRange(int num, int start, int count) {
    int mask = ((1 << count) - 1) << start;
    return num ^ mask;
}

// 4. EXTRACT bits from range
int extractRange(int num, int start, int count) {
    return (num >> start) & ((1 << count) - 1);
}

// 5. INSERT bits into range
int insertRange(int num, int start, int count, int value) {
    // Clear the range first
    int clearMask = ~(((1 << count) - 1) << start);
    num &= clearMask;
    
    // Insert new value
    return num | ((value & ((1 << count) - 1)) << start);
}

// Examples
void testRangeOperations() {
    int num = 0b10101100;  // 172
    
    printf("Original: ");
    printBinary(num);
    
    printf("Set bits 1-3: ");
    printBinary(setRange(num, 1, 3));  // 10101110
    
    printf("Clear bits 2-4: ");
    printBinary(clearRange(num, 2, 3));  // 10100000
    
    printf("Extract bits 2-5: %d\n", extractRange(num, 2, 4));  // 1011 = 11
    
    printf("Insert 0101 at position 2: ");
    printBinary(insertRange(num, 2, 4, 0b0101));  // 10010100
}
```

### 3.3 Quick Reference: Common Bit Tricks

```c
/**
 * QUICK REFERENCE: BIT MANIPULATION TRICKS
 */

void bitTricksReference() {
    int n = 42;
    
    // Check if even/odd
    bool isEven = (n & 1) == 0;
    
    // Multiply/divide by 2
    int doubled = n << 1;
    int halved = n >> 1;
    
    // Check if power of 2
    bool isPow2 = (n > 0) && ((n & (n - 1)) == 0);
    
    // Get rightmost set bit
    int rightmost = n & -n;
    
    // Clear rightmost set bit
    int cleared = n & (n - 1);
    
    // Isolate rightmost 0 bit
    int rightmost0 = ~n & (n + 1);
    
    // Create mask of n bits
    unsigned int mask = (1 << n) - 1;
    
    // Swap two numbers
    int a = 5, b = 10;
    a ^= b; b ^= a; a ^= b;  // Now a=10, b=5
    
    // Absolute value (for int)
    int num = -15;
    int sign = num >> 31;  // All 1s if negative, all 0s if positive
    int abs = (num ^ sign) - sign;
    
    // Check if opposite signs
    bool oppositeSigns = (a ^ b) < 0;
    
    // Count trailing zeros
    int trailingZeros = __builtin_ctz(n);  // GCC built-in
    
    // Count leading zeros
    int leadingZeros = __builtin_clz(n);  // GCC built-in
    
    // Count set bits
    int setBits = __builtin_popcount(n);  // GCC built-in
}
```

---

# PART 2: ESSENTIAL PATTERNS

## Section 4: Setting, Clearing, and Toggling Bits

### 4.1 Pattern Overview

**When to Use:**
- ✅ Need to modify specific bit(s)
- ✅ Flag manipulation
- ✅ State management
- ✅ Configuration registers
- ✅ Permissions and access control

### 4.2 Complete Implementation

```c
/**
 * COMPREHENSIVE BIT MODIFICATION TOOLKIT
 */

// ============ SINGLE BIT OPERATIONS ============

// Set bit at position (make it 1)
int setBit(int num, int pos) {
    return num | (1 << pos);
}

// Clear bit at position (make it 0)
int clearBit(int num, int pos) {
    return num & ~(1 << pos);
}

// Toggle bit at position (flip it)
int toggleBit(int num, int pos) {
    return num ^ (1 << pos);
}

// Update bit to specific value (0 or 1)
int updateBit(int num, int pos, int value) {
    int clearMask = ~(1 << pos);
    return (num & clearMask) | ((value & 1) << pos);
}

// ============ MULTIPLE BIT OPERATIONS ============

// Set all bits in range [start, start+len)
int setRange(int num, int start, int len) {
    int mask = ((1 << len) - 1) << start;
    return num | mask;
}

// Clear all bits in range
int clearRange(int num, int start, int len) {
    int mask = ((1 << len) - 1) << start;
    return num & ~mask;
}

// Toggle all bits in range
int toggleRange(int num, int start, int len) {
    int mask = ((1 << len) - 1) << start;
    return num ^ mask;
}

// ============ SPECIAL OPERATIONS ============

// Clear all bits from most significant down to position (inclusive)
int clearBitsMSBToI(int num, int pos) {
    int mask = (1 << pos) - 1;
    return num & mask;
}

// Clear all bits from position to LSB (inclusive)
int clearBitsIToLSB(int num, int pos) {
    int mask = ~0 << (pos + 1);
    return num & mask;
}

// Set all bits to 1
int setAllBits(int num) {
    return ~0;
}

// Clear all bits to 0
int clearAllBits(int num) {
    return 0;
}
```

### 4.3 Practical Examples

#### Example 1: Permission System

```c
/**
 * FILE PERMISSION SYSTEM (like Unix chmod)
 * 
 * Bits: | Owner | Group | Others |
 *       | R W X | R W X | R  W X |
 *       | 8 7 6 | 5 4 3 | 2  1 0 |
 */

#define OWNER_READ    (1 << 8)
#define OWNER_WRITE   (1 << 7)
#define OWNER_EXEC    (1 << 6)
#define GROUP_READ    (1 << 5)
#define GROUP_WRITE   (1 << 4)
#define GROUP_EXEC    (1 << 3)
#define OTHERS_READ   (1 << 2)
#define OTHERS_WRITE  (1 << 1)
#define OTHERS_EXEC   (1 << 0)

typedef struct {
    int permissions;
    char filename[256];
} File;

// Grant permission
void grantPermission(File* file, int permission) {
    file->permissions |= permission;
}

// Revoke permission
void revokePermission(File* file, int permission) {
    file->permissions &= ~permission;
}

// Check permission
bool hasPermission(File* file, int permission) {
    return (file->permissions & permission) != 0;
}

// Set permission using octal notation (like chmod 755)
void setPermissionOctal(File* file, int owner, int group, int others) {
    file->permissions = (owner << 6) | (group << 3) | others;
}

// Display permissions (like ls -l)
void displayPermissions(File* file) {
    printf("%s: ", file->filename);
    
    // Owner
    printf(hasPermission(file, OWNER_READ) ? "r" : "-");
    printf(hasPermission(file, OWNER_WRITE) ? "w" : "-");
    printf(hasPermission(file, OWNER_EXEC) ? "x" : "-");
    
    // Group
    printf(hasPermission(file, GROUP_READ) ? "r" : "-");
    printf(hasPermission(file, GROUP_WRITE) ? "w" : "-");
    printf(hasPermission(file, GROUP_EXEC) ? "x" : "-");
    
    // Others
    printf(hasPermission(file, OTHERS_READ) ? "r" : "-");
    printf(hasPermission(file, OTHERS_WRITE) ? "w" : "-");
    printf(hasPermission(file, OTHERS_EXEC) ? "x" : "-");
    
    printf("\n");
}

// Test permission system
void testPermissionSystem() {
    File myFile = {0};
    strcpy(myFile.filename, "document.txt");
    
    // Set permission to 644 (rw-r--r--)
    setPermissionOctal(&myFile, 6, 4, 4);  // 110 100 100
    displayPermissions(&myFile);
    
    // Grant execute permission to owner
    grantPermission(&myFile, OWNER_EXEC);
    displayPermissions(&myFile);  // rwxr--r--
    
    // Revoke write permission from owner
    revokePermission(&myFile, OWNER_WRITE);
    displayPermissions(&myFile);  // r-xr--r--
}
```

#### Example 2: Status Flags System

```c
/**
 * STATUS FLAGS (like TCP flags or device status)
 */

// Status bit definitions
#define STATUS_ACTIVE      (1 << 0)  // Bit 0: Active/Inactive
#define STATUS_ERROR       (1 << 1)  // Bit 1: Error flag
#define STATUS_WARNING     (1 << 2)  // Bit 2: Warning flag
#define STATUS_BUSY        (1 << 3)  // Bit 3: Busy
#define STATUS_READY       (1 << 4)  // Bit 4: Ready
#define STATUS_CONNECTED   (1 << 5)  // Bit 5: Connected
#define STATUS_AUTHORIZED  (1 << 6)  // Bit 6: Authorized
#define STATUS_ENCRYPTED   (1 << 7)  // Bit 7: Encrypted

typedef struct {
    unsigned char status;
    char name[50];
} Device;

// Set status flag
void setStatus(Device* device, unsigned char flag) {
    device->status |= flag;
}

// Clear status flag
void clearStatus(Device* device, unsigned char flag) {
    device->status &= ~flag;
}

// Toggle status flag
void toggleStatus(Device* device, unsigned char flag) {
    device->status ^= flag;
}

// Check if status flag is set
bool isStatusSet(Device* device, unsigned char flag) {
    return (device->status & flag) != 0;
}

// Clear all status flags
void clearAllStatus(Device* device) {
    device->status = 0;
}

// Get status string
void getStatusString(Device* device, char* buffer, int bufferSize) {
    buffer[0] = '\0';
    
    if (isStatusSet(device, STATUS_ACTIVE)) strcat(buffer, "ACTIVE ");
    if (isStatusSet(device, STATUS_ERROR)) strcat(buffer, "ERROR ");
    if (isStatusSet(device, STATUS_WARNING)) strcat(buffer, "WARNING ");
    if (isStatusSet(device, STATUS_BUSY)) strcat(buffer, "BUSY ");
    if (isStatusSet(device, STATUS_READY)) strcat(buffer, "READY ");
    if (isStatusSet(device, STATUS_CONNECTED)) strcat(buffer, "CONNECTED ");
    if (isStatusSet(device, STATUS_AUTHORIZED)) strcat(buffer, "AUTHORIZED ");
    if (isStatusSet(device, STATUS_ENCRYPTED)) strcat(buffer, "ENCRYPTED ");
}

// Test device status
void testDeviceStatus() {
    Device device = {0};
    strcpy(device.name, "Sensor-001");
    char statusStr[200];
    
    // Initial state
    setStatus(&device, STATUS_ACTIVE | STATUS_READY | STATUS_CONNECTED);
    getStatusString(&device, statusStr, sizeof(statusStr));
    printf("%s Status: %s\n", device.name, statusStr);
    
    // Start operation
    setStatus(&device, STATUS_BUSY);
    clearStatus(&device, STATUS_READY);
    getStatusString(&device, statusStr, sizeof(statusStr));
    printf("%s Status: %s\n", device.name, statusStr);
    
    // Error occurred
    setStatus(&device, STATUS_ERROR);
    getStatusString(&device, statusStr, sizeof(statusStr));
    printf("%s Status: %s\n", device.name, statusStr);
}
```

#### Example 3: Bit Field Packing

```c
/**
 * PACKING MULTIPLE VALUES INTO SINGLE INTEGER
 * Useful for saving memory in embedded systems
 */

// Pack RGB color into 32-bit integer
// Format: 0xAARRGGBB (Alpha, Red, Green, Blue)
unsigned int packRGB(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

// Unpack RGB color
void unpackRGB(unsigned int color, unsigned char* red, unsigned char* green, unsigned char* blue, unsigned char* alpha) {
    *alpha = (color >> 24) & 0xFF;
    *red = (color >> 16) & 0xFF;
    *green = (color >> 8) & 0xFF;
    *blue = color & 0xFF;
}

// Pack date into 16-bit integer
// Format: YYYYYYYMMMMDDDDD (7 bits year offset from 2000, 4 bits month, 5 bits day)
unsigned short packDate(int year, int month, int day) {
    int yearOffset = year - 2000;  // Years since 2000
    return (yearOffset << 9) | (month << 5) | day;
}

// Unpack date
void unpackDate(unsigned short packed, int* year, int* month, int* day) {
    *year = ((packed >> 9) & 0x7F) + 2000;
    *month = (packed >> 5) & 0x0F;
    *day = packed & 0x1F;
}

// Pack time into 16-bit integer
// Format: HHHHHMMMMMMXXXXX (5 bits hour, 6 bits minute, 5 bits second/2)
unsigned short packTime(int hour, int minute, int second) {
    return (hour << 11) | (minute << 5) | (second / 2);
}

// Unpack time
void unpackTime(unsigned short packed, int* hour, int* minute, int* second) {
    *hour = (packed >> 11) & 0x1F;
    *minute = (packed >> 5) & 0x3F;
    *second = (packed & 0x1F) * 2;
}

// Test packing/unpacking
void testPacking() {
    // RGB color
    unsigned int color = packRGB(255, 128, 64, 200);
    printf("Packed color: 0x%08X\n", color);
    
    unsigned char r, g, b, a;
    unpackRGB(color, &r, &g, &b, &a);
    printf("Unpacked: R=%d, G=%d, B=%d, A=%d\n\n", r, g, b, a);
    
    // Date
    unsigned short date = packDate(2025, 10, 3);
    printf("Packed date: 0x%04X\n", date);
    
    int year, month, day;
    unpackDate(date, &year, &month, &day);
    printf("Unpacked date: %d-%02d-%02d\n\n", year, month, day);
    
    // Time
    unsigned short time = packTime(14, 30, 45);
    printf("Packed time: 0x%04X\n", time);
    
    int hour, minute, second;
    unpackTime(time, &hour, &minute, &second);
    printf("Unpacked time: %02d:%02d:%02d\n", hour, minute, second);
}
```

### 4.4 Practice Problems

#### Beginner Level

1. **Set Bit**
   - Input: `num = 10` (binary: 1010), `pos = 0`
   - Output: `11` (binary: 1011)
   - Hint: Use OR with shifted 1

2. **Clear Bit**
   - Input: `num = 15` (binary: 1111), `pos = 2`
   - Output: `11` (binary: 1011)
   - Hint: Use AND with NOT of shifted 1

3. **Toggle Bit**
   - Input: `num = 10` (binary: 1010), `pos = 1`
   - Output: `8` (binary: 1000)
   - Hint: Use XOR with shifted 1

4. **Update Bit**
   - Input: `num = 10`, `pos = 0`, `value = 1`
   - Output: `11`
   - Hint: Clear then set if value is 1

5. **Check Bit**
   - Input: `num = 10` (binary: 1010), `pos = 3`
   - Output: `true`
   - Hint: Shift and mask with 1

---

## Section 5: Checking and Extracting Bits

### 5.1 Pattern Overview

**When to Use:**
- ✅ Need to read specific bit(s)
- ✅ Extract fields from packed data
- ✅ Check status or flags
- ✅ Parse binary protocols
- ✅ Decode bit fields

### 5.2 Core Operations

```c
/**
 * BIT CHECKING AND EXTRACTION OPERATIONS
 */

// ============ SINGLE BIT CHECKING ============

// Check if bit at position is set
bool isBitSet(int num, int pos) {
    return (num & (1 << pos)) != 0;
}

// Check if bit at position is clear
bool isBitClear(int num, int pos) {
    return (num & (1 << pos)) == 0;
}

// Get bit value at position (returns 0 or 1)
int getBit(int num, int pos) {
    return (num >> pos) & 1;
}

// ============ MULTIPLE BITS EXTRACTION ============

// Extract n bits starting from position
int extractBits(int num, int pos, int n) {
    // Create mask of n ones: (1 << n) - 1
    // Shift num right by pos, then mask
    return (num >> pos) & ((1 << n) - 1);
}

// Extract bits in range [start, end] (inclusive)
int extractRange(int num, int start, int end) {
    int len = end - start + 1;
    return (num >> start) & ((1 << len) - 1);
}

// Get lower n bits
int getLowerBits(int num, int n) {
    return num & ((1 << n) - 1);
}

// Get upper n bits (for 32-bit integer)
int getUpperBits(int num, int n) {
    return (unsigned int)num >> (32 - n);
}

// ============ SPECIAL CHECKS ============

// Check if multiple bits are all set
bool areAllBitsSet(int num, int mask) {
    return (num & mask) == mask;
}

// Check if any bit in mask is set
bool isAnyBitSet(int num, int mask) {
    return (num & mask) != 0;
}

// Check if no bits in mask are set
bool areNoBitsSet(int num, int mask) {
    return (num & mask) == 0;
}

// Count set bits in a number
int countSetBits(int num) {
    int count = 0;
    while (num) {
        count += num & 1;
        num >>= 1;
    }
    return count;
}

// Alternative: Brian Kernighan's Algorithm (faster)
int countSetBitsFast(int num) {
    int count = 0;
    while (num) {
        num &= (num - 1);  // Remove rightmost set bit
        count++;
    }
    return count;
}
```

### 5.3 Practical Examples

#### Example 1: IP Address Parsing

```c
/**
 * IP ADDRESS MANIPULATION
 * IPv4 address stored as 32-bit integer: A.B.C.D
 */

// Convert IP octets to 32-bit integer
unsigned int ipToInt(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
}

// Extract octets from 32-bit IP
void intToIp(unsigned int ip, unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) {
    *a = (ip >> 24) & 0xFF;
    *b = (ip >> 16) & 0xFF;
    *c = (ip >> 8) & 0xFF;
    *d = ip & 0xFF;
}

// Check if IP is in subnet
bool isInSubnet(unsigned int ip, unsigned int subnet, unsigned int mask) {
    return (ip & mask) == (subnet & mask);
}

// Get network address
unsigned int getNetworkAddress(unsigned int ip, unsigned int mask) {
    return ip & mask;
}

// Get broadcast address
unsigned int getBroadcastAddress(unsigned int ip, unsigned int mask) {
    return ip | ~mask;
}

// Count number of hosts in subnet
unsigned int countHosts(unsigned int mask) {
    // Number of zero bits in mask minus 2 (network and broadcast)
    int zeroBits = countSetBits(~mask);
    return (1 << zeroBits) - 2;
}

// Test IP operations
void testIPOperations() {
    unsigned int ip = ipToInt(192, 168, 1, 100);
    unsigned int mask = ipToInt(255, 255, 255, 0);
    unsigned int subnet = ipToInt(192, 168, 1, 0);
    
    printf("IP: %u.%u.%u.%u\n", 
           (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, 
           (ip >> 8) & 0xFF, ip & 0xFF);
    
    printf("Is in subnet: %s\n", isInSubnet(ip, subnet, mask) ? "Yes" : "No");
    
    unsigned int network = getNetworkAddress(ip, mask);
    printf("Network: %u.%u.%u.%u\n",
           (network >> 24) & 0xFF, (network >> 16) & 0xFF,
           (network >> 8) & 0xFF, network & 0xFF);
    
    unsigned int broadcast = getBroadcastAddress(ip, mask);
    printf("Broadcast: %u.%u.%u.%u\n",
           (broadcast >> 24) & 0xFF, (broadcast >> 16) & 0xFF,
           (broadcast >> 8) & 0xFF, broadcast & 0xFF);
    
    printf("Available hosts: %u\n", countHosts(mask));
}
```

#### Example 2: Network Packet Header

```c
/**
 * PARSING NETWORK PACKET HEADERS
 * Example: IPv4 header fields
 */

typedef struct {
    unsigned int header;  // 32-bit packed header
    /*
     * Bits 0-3:   Version (4 bits)
     * Bits 4-7:   IHL - Internet Header Length (4 bits)
     * Bits 8-13:  DSCP - Differentiated Services (6 bits)
     * Bits 14-15: ECN - Explicit Congestion Notification (2 bits)
     * Bits 16-31: Total Length (16 bits)
     */
} IPv4Header;

// Extract version (bits 0-3)
int getVersion(IPv4Header* header) {
    return extractBits(header->header, 0, 4);
}

// Extract IHL (bits 4-7)
int getIHL(IPv4Header* header) {
    return extractBits(header->header, 4, 4);
}

// Extract DSCP (bits 8-13)
int getDSCP(IPv4Header* header) {
    return extractBits(header->header, 8, 6);
}

// Extract ECN (bits 14-15)
int getECN(IPv4Header* header) {
    return extractBits(header->header, 14, 2);
}

// Extract Total Length (bits 16-31)
int getTotalLength(IPv4Header* header) {
    return extractBits(header->header, 16, 16);
}

// Set version
void setVersion(IPv4Header* header, int version) {
    // Clear version bits and set new value
    header->header = (header->header & ~0x0F) | (version & 0x0F);
}

// Build header from components
unsigned int buildHeader(int version, int ihl, int dscp, int ecn, int totalLength) {
    return (version & 0x0F) | 
           ((ihl & 0x0F) << 4) | 
           ((dscp & 0x3F) << 8) | 
           ((ecn & 0x03) << 14) | 
           ((totalLength & 0xFFFF) << 16);
}

// Test packet header
void testPacketHeader() {
    IPv4Header header;
    header.header = buildHeader(4, 5, 0, 0, 1500);
    
    printf("Version: %d\n", getVersion(&header));
    printf("IHL: %d\n", getIHL(&header));
    printf("DSCP: %d\n", getDSCP(&header));
    printf("ECN: %d\n", getECN(&header));
    printf("Total Length: %d\n", getTotalLength(&header));
}
```

#### Example 3: Bitboard (Chess/Checkers)

```c
/**
 * BITBOARD REPRESENTATION
 * Used in chess engines - 64-bit board, each bit = square
 */

typedef unsigned long long Bitboard;

// Set piece at position
Bitboard setPiece(Bitboard board, int square) {
    return board | (1ULL << square);
}

// Remove piece at position
Bitboard removePiece(Bitboard board, int square) {
    return board & ~(1ULL << square);
}

// Check if square occupied
bool isOccupied(Bitboard board, int square) {
    return (board & (1ULL << square)) != 0;
}

// Move piece from one square to another
Bitboard movePiece(Bitboard board, int from, int to) {
    board = removePiece(board, from);
    board = setPiece(board, to);
    return board;
}

// Count number of pieces
int countPieces(Bitboard board) {
    return countSetBitsFast(board);
}

// Get position of least significant bit (first piece)
int getFirstPiece(Bitboard board) {
    if (board == 0) return -1;
    return __builtin_ctzll(board);  // Count trailing zeros
}

// Get position of most significant bit (last piece)
int getLastPiece(Bitboard board) {
    if (board == 0) return -1;
    return 63 - __builtin_clzll(board);  // 63 - count leading zeros
}

// Print board (8x8 grid)
void printBitboard(Bitboard board) {
    for (int rank = 7; rank >= 0; rank--) {
        printf("%d ", rank + 1);
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            printf("%c ", isOccupied(board, square) ? 'X' : '.');
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

// Test bitboard
void testBitboard() {
    Bitboard board = 0;
    
    // Place pieces
    board = setPiece(board, 0);   // a1
    board = setPiece(board, 7);   // h1
    board = setPiece(board, 56);  // a8
    board = setPiece(board, 63);  // h8
    
    printf("Initial board:\n");
    printBitboard(board);
    
    printf("\nNumber of pieces: %d\n", countPieces(board));
    
    // Move piece from a1 to d4
    board = movePiece(board, 0, 27);
    printf("\nAfter moving a1 to d4:\n");
    printBitboard(board);
}
```

### 5.4 Practice Problems

#### Beginner Level

1. **Get Bit**
   - Input: `num = 10` (1010), `pos = 3`
   - Output: `1`
   - Hint: Shift right and mask

2. **Extract 3 Bits**
   - Input: `num = 0b11010110`, `pos = 2`, `n = 3`
   - Output: `0b101` (5)
   - Hint: Shift and mask

3. **Count Set Bits**
   - Input: `num = 15` (1111)
   - Output: `4`
   - Hint: Loop or Brian Kernighan's algorithm

4. **Check Multiple Bits**
   - Input: `num = 0b1010`, `mask = 0b1000`
   - Output: `true` (bit is set)
   - Hint: AND with mask

5. **Extract Byte**
   - Input: `num = 0x12345678`, `bytePos = 2`
   - Output: `0x34`
   - Hint: Shift and mask with 0xFF

#### Intermediate Level

6. **Parse IPv4 Address**
   - Input: `ip = 3232235876` (192.168.1.100)
   - Output: `192, 168, 1, 100`
   - Hint: Extract each byte

7. **Extract Bit Range**
   - Input: `num = 0xFF`, `start = 2`, `end = 5`
   - Output: `0b1111` (15)
   - Hint: Calculate length and extract

8. **Check Power of 4**
   - Input: `num = 16`
   - Output: `true`
   - Hint: Power of 2 and even number of trailing zeros

9. **Reverse Bits in Byte**
   - Input: `byte = 0b11010010`
   - Output: `0b01001011`
   - Hint: Extract and rebuild

10. **Hamming Weight**
    - Input: `num = 11` (1011)
    - Output: `3` (number of 1s)
    - Hint: Count set bits

---

## Section 6: Counting Bits Pattern

### 6.1 Pattern Overview

**When to Use:**
- ✅ Count number of 1s (set bits)
- ✅ Count number of 0s (clear bits)
- ✅ Population count (popcount)
- ✅ Hamming weight
- ✅ Parity checking

### 6.2 Counting Algorithms

```c
/**
 * BIT COUNTING ALGORITHMS
 */

// Method 1: Naive approach - check each bit
int countSetBitsNaive(unsigned int n) {
    int count = 0;
    while (n) {
        if (n & 1) count++;
        n >>= 1;
    }
    return count;
}

// Method 2: Brian Kernighan's Algorithm
// Faster - only loops for each set bit
int countSetBitsKernighan(unsigned int n) {
    int count = 0;
    while (n) {
        n &= (n - 1);  // Clear rightmost set bit
        count++;
    }
    return count;
}

// Method 3: Lookup table (fastest for repeated calls)
static unsigned char bitCountTable[256];

void initBitCountTable() {
    bitCountTable[0] = 0;
    for (int i = 1; i < 256; i++) {
        bitCountTable[i] = bitCountTable[i / 2] + (i & 1);
    }
}

int countSetBitsTable(unsigned int n) {
    return bitCountTable[n & 0xFF] + 
           bitCountTable[(n >> 8) & 0xFF] +
           bitCountTable[(n >> 16) & 0xFF] +
           bitCountTable[(n >> 24) & 0xFF];
}

// Method 4: Parallel counting (SWAR algorithm)
int countSetBitsParallel(unsigned int n) {
    n = n - ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n + (n >> 4)) & 0x0F0F0F0F;
    n = n + (n >> 8);
    n = n + (n >> 16);
    return n & 0x3F;
}

// Method 5: Built-in function (compiler intrinsic)
int countSetBitsBuiltin(unsigned int n) {
    return __builtin_popcount(n);  // GCC/Clang
}

// Count zero bits
int countZeroBits(unsigned int n) {
    return 32 - countSetBits(n);
}

// Count trailing zeros (position of rightmost set bit)
int countTrailingZeros(unsigned int n) {
    if (n == 0) return 32;
    return __builtin_ctz(n);  // GCC/Clang
}

// Count leading zeros
int countLeadingZeros(unsigned int n) {
    if (n == 0) return 32;
    return __builtin_clz(n);  // GCC/Clang
}
```

### 6.3 Practical Examples

#### Example 1: Hamming Distance

```c
/**
 * HAMMING DISTANCE
 * Number of positions at which corresponding bits are different
 */

int hammingDistance(int x, int y) {
    // XOR gives 1 where bits differ
    return countSetBits(x ^ y);
}

// Total Hamming distance between all pairs in array
int totalHammingDistance(int* nums, int numsSize) {
    int total = 0;
    
    // For each bit position
    for (int bit = 0; bit < 32; bit++) {
        int countOnes = 0;
        
        // Count how many numbers have this bit set
        for (int i = 0; i < numsSize; i++) {
            if (nums[i] & (1 << bit)) {
                countOnes++;
            }
        }
        
        // Each 1 paired with each 0 contributes to distance
        int countZeros = numsSize - countOnes;
        total += countOnes * countZeros;
    }
    
    return total;
}

// Test Hamming distance
void testHammingDistance() {
    printf("Hamming distance between 1 and 4: %d\n", hammingDistance(1, 4));
    // 1 = 001, 4 = 100, distance = 2
    
    int arr[] = {4, 14, 2};
    printf("Total Hamming distance: %d\n", totalHammingDistance(arr, 3));
}
```

#### Example 2: Binary Watch

```c
/**
 * BINARY WATCH
 * LED watch with 10 LEDs representing time in binary
 * Top row: 4 LEDs for hours (0-11)
 * Bottom row: 6 LEDs for minutes (0-59)
 */

char** readBinaryWatch(int turnedOn, int* returnSize) {
    char** result = (char**)malloc(1000 * sizeof(char*));
    *returnSize = 0;
    
    // Try all possible times
    for (int hour = 0; hour < 12; hour++) {
        for (int minute = 0; minute < 60; minute++) {
            // Count total LEDs that would be on
            int leds = countSetBits(hour) + countSetBits(minute);
            
            if (leds == turnedOn) {
                result[*returnSize] = (char*)malloc(6 * sizeof(char));
                sprintf(result[*returnSize], "%d:%02d", hour, minute);
                (*returnSize)++;
            }
        }
    }
    
    return result;
}

// Test binary watch
void testBinaryWatch() {
    int returnSize;
    char** times = readBinaryWatch(1, &returnSize);
    
    printf("Times with 1 LED on:\n");
    for (int i = 0; i < returnSize; i++) {
        printf("%s ", times[i]);
        free(times[i]);
    }
    printf("\n");
    free(times);
}
```

#### Example 3: Counting Bits for Range

```c
/**
 * COUNTING BITS IN RANGE
 * For each number from 0 to n, count set bits
 */

int* countBits(int n, int* returnSize) {
    *returnSize = n + 1;
    int* result = (int*)malloc((*returnSize) * sizeof(int));
    
    // Method 1: Simple approach
    for (int i = 0; i <= n; i++) {
        result[i] = countSetBits(i);
    }
    
    return result;
}

int* countBitsDP(int n, int* returnSize) {
    *returnSize = n + 1;
    int* result = (int*)malloc((*returnSize) * sizeof(int));
    
    result[0] = 0;
    for (int i = 1; i <= n; i++) {
        // Key insight: count[i] = count[i/2] + (i%2)
        // or equivalently: count[i] = count[i>>1] + (i&1)
        result[i] = result[i >> 1] + (i & 1);
    }
    
    return result;
}

// Another DP approach: using i & (i-1)
int* countBitsDP2(int n, int* returnSize) {
    *returnSize = n + 1;
    int* result = (int*)malloc((*returnSize) * sizeof(int));
    
    result[0] = 0;
    for (int i = 1; i <= n; i++) {
        // i & (i-1) removes rightmost set bit
        // so count[i] = count[i & (i-1)] + 1
        result[i] = result[i & (i - 1)] + 1;
    }
    
    return result;
}

// Test counting bits
void testCountBits() {
    int returnSize;
    int* result = countBitsDP(10, &returnSize);
    
    printf("Bit counts from 0 to 10:\n");
    for (int i = 0; i < returnSize; i++) {
        printf("%d: %d bits\n", i, result[i]);
    }
    free(result);
}
```

### 6.4 Practice Problems

#### Beginner Level

1. **Number of 1 Bits**
   - Input: `n = 11` (1011)
   - Output: `3`
   - Hint: Count set bits

2. **Hamming Distance**
   - Input: `x = 1`, `y = 4`
   - Output: `2`
   - Hint: XOR then count bits

3. **Counting Bits**
   - Input: `n = 5`
   - Output: `[0,1,1,2,1,2]`
   - Hint: Use DP with bit patterns

4. **Binary Watch**
   - Input: `turnedOn = 1`
   - Output: All valid times
   - Hint: Enumerate and count

5. **Power of Four**
   - Input: `n = 16`
   - Output: `true`
   - Hint: One bit set and even position

#### Intermediate Level

6. **Total Hamming Distance**
   - Input: `nums = [4,14,2]`
   - Output: `6`
   - Hint: Count bit differences at each position

7. **Prime Number of Set Bits**
   - Input: `left = 10`, `right = 15`
   - Output: `5` (numbers with prime bit count)
   - Hint: Count bits, check if prime

8. **Binary Number with Alternating Bits**
   - Input: `n = 5` (101)
   - Output: `true`
   - Hint: Check consecutive bits differ

9. **Number Complement**
   - Input: `num = 5` (101)
   - Output: `2` (010)
   - Hint: Flip all bits up to highest set bit

10. **Find the Difference**
    - Input: Two strings, one has extra character
    - Output: The extra character
    - Hint: XOR all characters

---

## Section 7: Power of Two Pattern

### 7.1 Pattern Overview

**Key Property:**
```
Power of 2 has exactly ONE bit set
Examples:
1  = 0001 = 2^0
2  = 0010 = 2^1
4  = 0100 = 2^2
8  = 1000 = 2^3
16 = 10000 = 2^4
```

**Critical Trick:**
```c
// For power of 2: n & (n-1) == 0
// Example: 8 & 7 = 1000 & 0111 = 0000
```

### 7.2 Power of Two Operations

```c
/**
 * POWER OF TWO PATTERNS
 */

// Check if number is power of 2
bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// Check if number is power of 4
bool isPowerOfFour(int n) {
    // Must be power of 2 AND
    // The single bit must be at even position (0, 2, 4, 6...)
    // Mask 0x55555555 = 01010101... (bits at even positions)
    return n > 0 && (n & (n - 1)) == 0 && (n & 0x55555555) != 0;
}

// Find next power of 2
unsigned int nextPowerOfTwo(unsigned int n) {
    if (n == 0) return 1;
    
    // If already power of 2, return it
    if ((n & (n - 1)) == 0) return n;
    
    // Otherwise, find next power of 2
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    
    return n;
}

// Find previous power of 2
unsigned int prevPowerOfTwo(unsigned int n) {
    // Set all bits after the highest set bit
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    
    // Remove all but highest bit
    return n - (n >> 1);
}

// Get position of the single set bit (for power of 2)
int getLogBase2(unsigned int n) {
    if (!isPowerOfTwo(n)) return -1;
    
    // Count trailing zeros
    return __builtin_ctz(n);
}

// Round down to nearest power of 2
unsigned int floorPowerOfTwo(unsigned int n) {
    if (n == 0) return 0;
    
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    
    return n - (n >> 1);
}

// Round up to nearest power of 2
unsigned int ceilPowerOfTwo(unsigned int n) {
    if (n == 0) return 1;
    if ((n & (n - 1)) == 0) return n;  // Already power of 2
    
    return nextPowerOfTwo(n);
}

// Multiply by power of 2 (fast)
int multiplyByPowerOfTwo(int n, int power) {
    return n << power;
}

// Divide by power of 2 (fast)
int divideByPowerOfTwo(int n, int power) {
    return n >> power;
}

// Check if divisible by power of 2
bool isDivisibleByPowerOfTwo(int n, int power) {
    int divisor = 1 << power;
    return (n & (divisor - 1)) == 0;
}
```

### 7.3 Practical Examples

#### Example 1: Memory Alignment

```c
/**
 * MEMORY ALIGNMENT
 * Align addresses to power-of-2 boundaries
 */

// Align address up to nearest boundary
void* alignUp(void* ptr, size_t alignment) {
    // alignment must be power of 2
    if (!isPowerOfTwo(alignment)) return NULL;
    
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t mask = alignment - 1;
    
    // Round up: (addr + mask) & ~mask
    return (void*)((addr + mask) & ~mask);
}

// Align address down to nearest boundary
void* alignDown(void* ptr, size_t alignment) {
    if (!isPowerOfTwo(alignment)) return NULL;
    
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t mask = alignment - 1;
    
    // Round down: addr & ~mask
    return (void*)(addr & ~mask);
}

// Check if address is aligned
bool isAligned(void* ptr, size_t alignment) {
    if (!isPowerOfTwo(alignment)) return false;
    
    uintptr_t addr = (uintptr_t)ptr;
    return (addr & (alignment - 1)) == 0;
}

// Get alignment of address (largest power of 2 that divides address)
size_t getAlignment(void* ptr) {
    uintptr_t addr = (uintptr_t)ptr;
    if (addr == 0) return 0;
    
    // Isolate rightmost set bit
    return addr & -addr;
}

// Test alignment
void testAlignment() {
    char buffer[100];
    void* ptr = &buffer[5];
    
    printf("Original pointer: %p\n", ptr);
    printf("Is 4-byte aligned? %s\n", isAligned(ptr, 4) ? "Yes" : "No");
    
    void* aligned8 = alignUp(ptr, 8);
    printf("Aligned up to 8 bytes: %p\n", aligned8);
    printf("Is 8-byte aligned? %s\n", isAligned(aligned8, 8) ? "Yes" : "No");
    
    void* aligned4down = alignDown(ptr, 4);
    printf("Aligned down to 4 bytes: %p\n", aligned4down);
}
```

#### Example 2: Fast Modulo with Power of 2

```c
/**
 * FAST MODULO OPERATIONS
 * When divisor is power of 2, use bitwise AND
 */

// Fast modulo for power of 2
int fastModulo(int n, int powerOf2) {
    // n % powerOf2 = n & (powerOf2 - 1)
    // Works because powerOf2 - 1 creates a mask
    // Example: n % 8 = n & 7 = n & 0111
    return n & (powerOf2 - 1);
}

// Check if number is multiple of power of 2
bool isMultipleOfPowerOf2(int n, int powerOf2) {
    return (n & (powerOf2 - 1)) == 0;
}

// Circular buffer index (wrap around)
int circularIndex(int index, int bufferSize) {
    // bufferSize must be power of 2
    return index & (bufferSize - 1);
}

// Circular buffer implementation
typedef struct {
    int* data;
    int capacity;  // Must be power of 2
    int head;
    int tail;
} CircularBuffer;

CircularBuffer* createCircularBuffer(int capacity) {
    // Round up to nearest power of 2
    capacity = ceilPowerOfTwo(capacity);
    
    CircularBuffer* buf = (CircularBuffer*)malloc(sizeof(CircularBuffer));
    buf->data = (int*)malloc(capacity * sizeof(int));
    buf->capacity = capacity;
    buf->head = 0;
    buf->tail = 0;
    
    return buf;
}

void circularBufferPush(CircularBuffer* buf, int value) {
    buf->data[buf->tail] = value;
    buf->tail = (buf->tail + 1) & (buf->capacity - 1);  // Fast modulo
}

int circularBufferPop(CircularBuffer* buf) {
    int value = buf->data[buf->head];
    buf->head = (buf->head + 1) & (buf->capacity - 1);  // Fast modulo
    return value;
}

bool circularBufferIsEmpty(CircularBuffer* buf) {
    return buf->head == buf->tail;
}

// Test circular buffer
void testCircularBuffer() {
    CircularBuffer* buf = createCircularBuffer(7);  // Will be rounded to 8
    
    printf("Buffer capacity: %d\n", buf->capacity);
    
    for (int i = 0; i < 5; i++) {
        circularBufferPush(buf, i * 10);
    }
    
    while (!circularBufferIsEmpty(buf)) {
        printf("%d ", circularBufferPop(buf));
    }
    printf("\n");
    
    free(buf->data);
    free(buf);
}
```

#### Example 3: Bit Indexing

```c
/**
 * BIT INDEXING TRICKS
 * Finding positions of set bits efficiently
 */

// Get index of rightmost set bit (0-indexed)
int getRightmostSetBit(unsigned int n) {
    if (n == 0) return -1;
    return __builtin_ctz(n);  // Count trailing zeros
}

// Get index of leftmost set bit (0-indexed)
int getLeftmostSetBit(unsigned int n) {
    if (n == 0) return -1;
    return 31 - __builtin_clz(n);  // 31 - count leading zeros
}

// Isolate rightmost set bit
unsigned int isolateRightmostSetBit(unsigned int n) {
    return n & -n;
}

// Clear rightmost set bit
unsigned int clearRightmostSetBit(unsigned int n) {
    return n & (n - 1);
}

// Isolate rightmost 0 bit
unsigned int isolateRightmost0Bit(unsigned int n) {
    return ~n & (n + 1);
}

// Set rightmost 0 bit
unsigned int setRightmost0Bit(unsigned int n) {
    return n | (n + 1);
}

// Create mask from rightmost set bit to LSB
unsigned int maskToRightmostSetBit(unsigned int n) {
    return n ^ (n - 1);
}

// Test bit indexing
void testBitIndexing() {
    unsigned int n = 0b101100;  // 44
    
    printf("Number: ");
    printBinary(n);
    
    printf("Rightmost set bit position: %d\n", getRightmostSetBit(n));
    printf("Leftmost set bit position: %d\n", getLeftmostSetBit(n));
    
    printf("Isolate rightmost set bit: ");
    printBinary(isolateRightmostSetBit(n));
    
    printf("Clear rightmost set bit: ");
    printBinary(clearRightmostSetBit(n));
    
    printf("Mask to rightmost set bit: ");
    printBinary(maskToRightmostSetBit(n));
}
```

### 7.4 Practice Problems

#### Beginner Level

1. **Power of Two**
   - Input: `n = 16`
   - Output: `true`
   - Hint: Check if n & (n-1) == 0

2. **Power of Four**
   - Input: `n = 16`
   - Output: `true`
   - Hint: Power of 2 + even bit position

3. **Power of Three** (NOT bitwise, but related)
   - Input: `n = 27`
   - Output: `true`
   - Hint: Different approach needed

4. **Find Next Power of 2**
   - Input: `n = 10`
   - Output: `16`
   - Hint: Propagate highest bit

5. **Log Base 2**
   - Input: `n = 16`
   - Output: `4`
   - Hint: Count trailing zeros

#### Intermediate Level

6. **Single Number**
   - Input: `nums = [4,1,2,1,2]`
   - Output: `4`
   - Hint: XOR all numbers

7. **Missing Number**
   - Input: `nums = [0,1,3]` (0 to n)
   - Output: `2`
   - Hint: XOR indices and values

8. **Find Duplicate**
   - Input: Array with one duplicate
   - Output: The duplicate
   - Hint: XOR with expected values

9. **Reverse Bits**
   - Input: `n = 43261596`
   - Output: Reversed bit pattern
   - Hint: Extract and rebuild

10. **Bitwise AND of Range**
    - Input: `left = 5`, `right = 7`
    - Output: Common bits
    - Hint: Find common prefix

---

## Section 8: XOR Magic Pattern

### 8.1 XOR Properties (Essential!)

```c
/**
 * XOR PROPERTIES - THE FOUNDATION
 */

void xorPropertiesDemo() {
    int a = 42, b = 17;
    
    // Property 1: Identity
    printf("a ^ 0 = a: %d ^ 0 = %d\n", a, a ^ 0);
    
    // Property 2: Self-inverse
    printf("a ^ a = 0: %d ^ %d = %d\n", a, a, a ^ a);
    
    // Property 3: Commutative
    printf("a ^ b = b ^ a: %d = %d\n", a ^ b, b ^ a);
    
    // Property 4: Associative
    int c = 99;
    printf("(a^b)^c = a^(b^c): %d = %d\n", (a^b)^c, a^(b^c));
    
    // Property 5: Cancellation
    printf("a ^ b ^ b = a: %d ^ %d ^ %d = %d\n", a, b, b, a^b^b);
    
    // Property 6: Finding difference
    printf("If a^b = c, then a^c = b: %d ^ %d = %d\n", a, a^b, b);
}
```

### 8.2 XOR Patterns

```c
/**
 * ESSENTIAL XOR PATTERNS
 */

// Pattern 1: Find single number (all others appear twice)
int singleNumber(int* nums, int numsSize) {
    int result = 0;
    for (int i = 0; i < numsSize; i++) {
        result ^= nums[i];  // Pairs cancel out
    }
    return result;
}

// Pattern 2: Find two single numbers (all others appear twice)
int* singleNumberTwo(int* nums, int numsSize, int* returnSize) {
    *returnSize = 2;
    int* result = (int*)malloc(2 * sizeof(int));
    
    // Step 1: XOR all numbers (gives num1 ^ num2)
    int xorAll = 0;
    for (int i = 0; i < numsSize; i++) {
        xorAll ^= nums[i];
    }
    
    // Step 2: Find a bit where num1 and num2 differ
    int diffBit = xorAll & -xorAll;  // Rightmost set bit
    
    // Step 3: Partition numbers into two groups
    result[0] = 0;
    result[1] = 0;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] & diffBit) {
            result[0] ^= nums[i];
        } else {
            result[1] ^= nums[i];
        }
    }
    
    return result;
}

// Pattern 3: Find missing number in range [0, n]
int missingNumber(int* nums, int numsSize) {
    int result = numsSize;  // Start with n
    
    for (int i = 0; i < numsSize; i++) {
        result ^= i ^ nums[i];  // XOR index and value
    }
    
    return result;
}

// Pattern 4: Find duplicate number
int findDuplicate(int* nums, int numsSize) {
    int xorAll = 0;
    
    // XOR all array elements
    for (int i = 0; i < numsSize; i++) {
        xorAll ^= nums[i];
    }
    
    // XOR with expected values [1, n]
    for (int i = 1; i < numsSize; i++) {
        xorAll ^= i;
    }
    
    return xorAll;
}

// Pattern 5: Swap without temporary variable
void swapXOR(int* a, int* b) {
    if (a != b) {  // Check not same address
        *a ^= *b;
        *b ^= *a;  // b = (a^b) ^ old_b = old_a
        *a ^= *b;  // a = (a^b) ^ old_a = old_b
    }
}

// Pattern 6: Check if numbers have opposite signs
bool oppositeSigns(int x, int y) {
    return (x ^ y) < 0;
}

// Pattern 7: Toggle case of letter
char toggleCase(char c) {
    return c ^ 32;  // Toggle bit 5 (difference between 'A' and 'a')
}

// Pattern 8: Encrypt/Decrypt with XOR (simple cipher)
void xorCipher(char* data, int len, char key) {
    for (int i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

// Pattern 9: Find element appearing once (others appear 3 times)
int singleNumberThree(int* nums, int numsSize) {
    int ones = 0, twos = 0;
    
    for (int i = 0; i < numsSize; i++) {
        // Add to twos if already in ones
        twos |= ones & nums[i];
        
        // Add to ones
        ones ^= nums[i];
        
        // Remove if appears in both (means seen 3 times)
        int threes = ones & twos;
        ones &= ~threes;
        twos &= ~threes;
    }
    
    return ones;
}
```

### 8.3 Practical Examples

#### Example 1: Data Validation with Parity

```c
/**
 * PARITY BIT CHECKING
 * Even parity: total 1s should be even
 * Odd parity: total 1s should be odd
 */

// Calculate parity bit for even parity
int calculateParityEven(unsigned int data) {
    int parity = 0;
    while (data) {
        parity ^= (data & 1);
        data >>= 1;
    }
    return parity;  // 0 for even count of 1s, 1 for odd
}

// Add parity bit to data
unsigned int addParityBit(unsigned int data, bool evenParity) {
    int parity = calculateParityEven(data);
    
    if (evenParity) {
        return (data << 1) | parity;
    } else {
        return (data << 1) | (parity ^ 1);
    }
}

// Check if data is valid (including parity bit)
bool checkParity(unsigned int dataWithParity, bool evenParity) {
    int parity = calculateParityEven(dataWithParity);
    return evenParity ? (parity == 0) : (parity == 1);
}

// Test parity
void testParity() {
    unsigned int data = 0b1011;  // 3 ones
    
    printf("Original data: ");
    printBinary(data);
    
    unsigned int withParity = addParityBit(data, true);
    printf("With even parity: ");
    printBinary(withParity);
    
    printf("Parity check: %s\n", 
           checkParity(withParity, true) ? "Valid" : "Invalid");
    
    // Simulate bit flip
    withParity ^= (1 << 2);
    printf("After bit flip: ");
    printBinary(withParity);
    printf("Parity check: %s\n", 
           checkParity(withParity, true) ? "Valid" : "Invalid");
}
```

#### Example 2: Error Detection (Checksum)

```c
/**
 * XOR CHECKSUM
 * Simple error detection mechanism
 */

// Calculate XOR checksum
unsigned char calculateChecksum(unsigned char* data, int length) {
    unsigned char checksum = 0;
    for (int i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

// Verify data with checksum
bool verifyChecksum(unsigned char* data, int length, unsigned char checksum) {
    unsigned char calculated = calculateChecksum(data, length);
    return calculated == checksum;
}

// Add checksum to data
void addChecksum(unsigned char* buffer, int dataLength) {
    unsigned char checksum = calculateChecksum(buffer, dataLength);
    buffer[dataLength] = checksum;
}

// Test checksum
void testChecksum() {
    unsigned char data[] = {0x12, 0x34, 0x56, 0x78, 0x00};  // Last byte for checksum
    int dataLen = 4;
    
    printf("Original data: ");
    for (int i = 0; i < dataLen; i++) {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
    
    addChecksum(data, dataLen);
    printf("Checksum: 0x%02X\n", data[dataLen]);
    
    printf("Verification: %s\n", 
           verifyChecksum(data, dataLen, data[dataLen]) ? "Valid" : "Invalid");
    
    // Simulate corruption
    data[1] ^= 0x01;
    printf("After corruption, verification: %s\n", 
           verifyChecksum(data, dataLen, data[dataLen]) ? "Valid" : "Invalid");
}
```

#### Example 3: Finding Missing and Duplicate

```c
/**
 * FIND MISSING AND DUPLICATE IN ARRAY
 * Array should contain 1 to n, but one number is missing and one is duplicate
 */

void findMissingAndDuplicate(int* nums, int numsSize, int* missing, int* duplicate) {
    int xorAll = 0;
    
    // XOR all array elements and expected values [1, n]
    for (int i = 0; i < numsSize; i++) {
        xorAll ^= nums[i];
        xorAll ^= (i + 1);
    }
    
    // xorAll now contains missing ^ duplicate
    
    // Find a bit where they differ
    int diffBit = xorAll & -xorAll;
    
    // Partition into two groups
    int x = 0, y = 0;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] & diffBit) {
            x ^= nums[i];
        } else {
            y ^= nums[i];
        }
        
        if ((i + 1) & diffBit) {
            x ^= (i + 1);
        } else {
            y ^= (i + 1);
        }
    }
    
    // Determine which is missing and which is duplicate
    bool xFound = false;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] == x) {
            xFound = true;
            break;
        }
    }
    
    if (xFound) {
        *duplicate = x;
        *missing = y;
    } else {
        *duplicate = y;
        *missing = x;
    }
}

// Test finding missing and duplicate
void testFindMissingDuplicate() {
    int nums[] = {1, 2, 2, 4, 5};  // Missing 3, duplicate 2
    int missing, duplicate;
    
    findMissingAndDuplicate(nums, 5, &missing, &duplicate);
    
    printf("Array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    
    printf("Missing: %d\n", missing);
    printf("Duplicate: %d\n", duplicate);
}
```

### 8.4 Practice Problems

#### Beginner Level

1. **Single Number**
   - Input: `[4,1,2,1,2]`
   - Output: `4`
   - Hint: XOR all elements

2. **Missing Number**
   - Input: `[0,1,3]` (0 to n with one missing)
   - Output: `2`
   - Hint: XOR indices and values

3. **Find Difference**
   - Input: Two strings, one has extra char
   - Output: Extra character
   - Hint: XOR all characters

4. **Toggle Case**
   - Input: `'A'` or `'a'`
   - Output: `'a'` or `'A'`
   - Hint: XOR with 32

5. **Check Opposite Signs**
   - Input: `x = 5`, `y = -3`
   - Output: `true`
   - Hint: Check sign bit with XOR

#### Intermediate Level

6. **Single Number II**
   - Input: Array where every element appears 3 times except one
   - Output: The single element
   - Hint: Count bits modulo 3

7. **Single Number III**
   - Input: Array where every element appears twice except two
   - Output: The two single elements
   - Hint: XOR all, then partition

8. **Maximum XOR of Two Numbers**
   - Input: `nums = [3,10,5,25,2,8]`
   - Output: `28` (5 XOR 25)
   - Hint: Trie or greedy bit-by-bit

9. **Find Missing and Duplicate**
   - Input: Array 1 to n with one missing, one duplicate
   - Output: Both numbers
   - Hint: XOR partitioning

10. **Minimize XOR**
    - Input: Two numbers
    - Output: Number with minimum XOR
    - Hint: Match bits greedily

---

## Section 9: Bit Masking Pattern

### 9.1 Pattern Overview

**Bit Mask:** A pattern of bits used to select or modify specific bits.

**Common Masks:**
```c
// Single bit masks
#define BIT0  0x01  // 0000 0001
#define BIT1  0x02  // 0000 0010
#define BIT2  0x04  // 0000 0100
#define BIT3  0x08  // 0000 1000
#define BIT4  0x10  // 0001 0000
#define BIT5  0x20  // 0010 0000
#define BIT6  0x40  // 0100 0000
#define BIT7  0x80  // 1000 0000

// Multi-bit masks
#define LOWER_NIBBLE  0x0F  // 0000 1111
#define UPPER_NIBBLE  0xF0  // 1111 0000
#define LOWER_BYTE    0xFF  // 1111 1111
#define ALL_BITS      0xFFFFFFFF
```

### 9.2 Mask Operations

```c
/**
 * BIT MASKING OPERATIONS
 */

// Create mask with n bits set starting from position
unsigned int createMask(int pos, int n) {
    return ((1 << n) - 1) << pos;
}

// Apply mask to extract bits
unsigned int applyMask(unsigned int value, unsigned int mask) {
    return value & mask;
}

// Set bits specified by mask
unsigned int setBitsMask(unsigned int value, unsigned int mask) {
    return value | mask;
}

// Clear bits specified by mask
unsigned int clearBitsMask(unsigned int value, unsigned int mask) {
    return value & ~mask;
}

// Toggle bits specified by mask
unsigned int toggleBitsMask(unsigned int value, unsigned int mask) {
    return value ^ mask;
}

// Check if all bits in mask are set
bool checkAllBitsSet(unsigned int value, unsigned int mask) {
    return (value & mask) == mask;
}

// Check if any bit in mask is set
bool checkAnyBitSet(unsigned int value, unsigned int mask) {
    return (value & mask) != 0;
}

// Replace bits in value with bits from newBits (using mask)
unsigned int replaceBits(unsigned int value, unsigned int newBits, unsigned int mask) {
    return (value & ~mask) | (newBits & mask);
}

// Extract field using mask and position
unsigned int extractField(unsigned int value, unsigned int mask) {
    // Count trailing zeros in mask to get position
    int pos = __builtin_ctz(mask);
    return (value & mask) >> pos;
}

// Insert field using mask and position
unsigned int insertField(unsigned int value, unsigned int field, unsigned int mask) {
    int pos = __builtin_ctz(mask);
    return (value & ~mask) | ((field << pos) & mask);
}
```

### 9.3 Practical Examples

#### Example 1: Status Register Management

```c
/**
 * HARDWARE STATUS REGISTER
 * Common in embedded systems
 */

// Status register bit definitions
#define STATUS_READY       (1 << 0)
#define STATUS_ERROR       (1 << 1)
#define STATUS_OVERFLOW    (1 << 2)
#define STATUS_UNDERFLOW   (1 << 3)
#define STATUS_BUSY        (1 << 4)
#define STATUS_INTERRUPT   (1 << 5)
#define STATUS_DMA_ENABLE  (1 << 6)
#define STATUS_POWER_ON    (1 << 7)

// Combined masks
#define STATUS_ERROR_MASK  (STATUS_ERROR | STATUS_OVERFLOW | STATUS_UNDERFLOW)
#define STATUS_CONTROL_MASK (STATUS_DMA_ENABLE | STATUS_POWER_ON)

typedef struct {
    unsigned char status;
    char name[50];
} HardwareRegister;

// Initialize register
void initRegister(HardwareRegister* reg, const char* name) {
    strcpy(reg->name, name);
    reg->status = 0;
}

// Set specific flags
void setFlags(HardwareRegister* reg, unsigned char flags) {
    reg->status |= flags;
}

// Clear specific flags
void clearFlags(HardwareRegister* reg, unsigned char flags) {
    reg->status &= ~flags;
}

// Check if any error flag is set
bool hasError(HardwareRegister* reg) {
    return (reg->status & STATUS_ERROR_MASK) != 0;
}

// Check if ready and not busy
bool isReadyForOperation(HardwareRegister* reg) {
    unsigned char mask = STATUS_READY | STATUS_BUSY;
    return (reg->status & mask) == STATUS_READY;
}

// Get error type
const char* getErrorType(HardwareRegister* reg) {
    if (reg->status & STATUS_ERROR) return "General Error";
    if (reg->status & STATUS_OVERFLOW) return "Overflow";
    if (reg->status & STATUS_UNDERFLOW) return "Underflow";
    return "No Error";
}

// Clear all error flags
void clearAllErrors(HardwareRegister* reg) {
    clearFlags(reg, STATUS_ERROR_MASK);
}

// Print register status
void printRegisterStatus(HardwareRegister* reg) {
    printf("Register: %s (0x%02X)\n", reg->name, reg->status);
    printf("  Ready:      %s\n", (reg->status & STATUS_READY) ? "Yes" : "No");
    printf("  Busy:       %s\n", (reg->status & STATUS_BUSY) ? "Yes" : "No");
    printf("  Error:      %s\n", hasError(reg) ? "Yes" : "No");
    if (hasError(reg)) {
        printf("  Error Type: %s\n", getErrorType(reg));
    }
    printf("  DMA:        %s\n", (reg->status & STATUS_DMA_ENABLE) ? "Enabled" : "Disabled");
    printf("  Power:      %s\n", (reg->status & STATUS_POWER_ON) ? "On" : "Off");
}

// Test hardware register
void testHardwareRegister() {
    HardwareRegister uart;
    initRegister(&uart, "UART0");
    
    // Power on and enable
    setFlags(&uart, STATUS_POWER_ON | STATUS_READY);
    printRegisterStatus(&uart);
    printf("\n");
    
    // Start operation
    setFlags(&uart, STATUS_BUSY);
    clearFlags(&uart, STATUS_READY);
    printRegisterStatus(&uart);
    printf("\n");
    
    // Error occurs
    setFlags(&uart, STATUS_OVERFLOW);
    printRegisterStatus(&uart);
    printf("\n");
    
    // Clear errors
    clearAllErrors(&uart);
    printRegisterStatus(&uart);
}
```

#### Example 2: Bitmap for Set Operations

```c
/**
 * BITMAP SET IMPLEMENTATION
 * Efficient set operations using bit arrays
 */

#define MAX_ELEMENTS 256
#define BITS_PER_WORD 32
#define WORD_COUNT (MAX_ELEMENTS / BITS_PER_WORD)

typedef struct {
    unsigned int bits[WORD_COUNT];
    int size;
} BitSet;

// Initialize empty set
void bitSetInit(BitSet* set) {
    for (int i = 0; i < WORD_COUNT; i++) {
        set->bits[i] = 0;
    }
    set->size = 0;
}

// Add element to set
void bitSetAdd(BitSet* set, int element) {
    if (element < 0 || element >= MAX_ELEMENTS) return;
    
    int wordIndex = element / BITS_PER_WORD;
    int bitIndex = element % BITS_PER_WORD;
    
    unsigned int mask = 1U << bitIndex;
    if (!(set->bits[wordIndex] & mask)) {
        set->bits[wordIndex] |= mask;
        set->size++;
    }
}

// Remove element from set
void bitSetRemove(BitSet* set, int element) {
    if (element < 0 || element >= MAX_ELEMENTS) return;
    
    int wordIndex = element / BITS_PER_WORD;
    int bitIndex = element % BITS_PER_WORD;
    
    unsigned int mask = 1U << bitIndex;
    if (set->bits[wordIndex] & mask) {
        set->bits[wordIndex] &= ~mask;
        set->size--;
    }
}

// Check if element exists
bool bitSetContains(BitSet* set, int element) {
    if (element < 0 || element >= MAX_ELEMENTS) return false;
    
    int wordIndex = element / BITS_PER_WORD;
    int bitIndex = element % BITS_PER_WORD;
    
    return (set->bits[wordIndex] & (1U << bitIndex)) != 0;
}

// Union of two sets (A ∪ B)
void bitSetUnion(BitSet* result, BitSet* setA, BitSet* setB) {
    for (int i = 0; i < WORD_COUNT; i++) {
        result->bits[i] = setA->bits[i] | setB->bits[i];
    }
    // Recalculate size
    result->size = 0;
    for (int i = 0; i < WORD_COUNT; i++) {
        result->size += __builtin_popcount(result->bits[i]);
    }
}

// Intersection of two sets (A ∩ B)
void bitSetIntersection(BitSet* result, BitSet* setA, BitSet* setB) {
    for (int i = 0; i < WORD_COUNT; i++) {
        result->bits[i] = setA->bits[i] & setB->bits[i];
    }
    result->size = 0;
    for (int i = 0; i < WORD_COUNT; i++) {
        result->size += __builtin_popcount(result->bits[i]);
    }
}

// Difference of two sets (A - B)
void bitSetDifference(BitSet* result, BitSet* setA, BitSet* setB) {
    for (int i = 0; i < WORD_COUNT; i++) {
        result->bits[i] = setA->bits[i] & ~setB->bits[i];
    }
    result->size = 0;
    for (int i = 0; i < WORD_COUNT; i++) {
        result->size += __builtin_popcount(result->bits[i]);
    }
}

// Symmetric difference (A Δ B = elements in A or B but not both)
void bitSetSymmetricDifference(BitSet* result, BitSet* setA, BitSet* setB) {
    for (int i = 0; i < WORD_COUNT; i++) {
        result->bits[i] = setA->bits[i] ^ setB->bits[i];
    }
    result->size = 0;
    for (int i = 0; i < WORD_COUNT; i++) {
        result->size += __builtin_popcount(result->bits[i]);
    }
}

// Check if subset (A ⊆ B)
bool bitSetIsSubset(BitSet* setA, BitSet* setB) {
    for (int i = 0; i < WORD_COUNT; i++) {
        if ((setA->bits[i] & ~setB->bits[i]) != 0) {
            return false;
        }
    }
    return true;
}

// Print set elements
void bitSetPrint(BitSet* set) {
    printf("{");
    bool first = true;
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        if (bitSetContains(set, i)) {
            if (!first) printf(", ");
            printf("%d", i);
            first = false;
        }
    }
    printf("} (size: %d)\n", set->size);
}

// Test bit set
void testBitSet() {
    BitSet setA, setB, result;
    
    bitSetInit(&setA);
    bitSetInit(&setB);
    
    // Add elements to setA: {1, 3, 5, 7, 9}
    bitSetAdd(&setA, 1);
    bitSetAdd(&setA, 3);
    bitSetAdd(&setA, 5);
    bitSetAdd(&setA, 7);
    bitSetAdd(&setA, 9);
    
    // Add elements to setB: {2, 3, 5, 8}
    bitSetAdd(&setB, 2);
    bitSetAdd(&setB, 3);
    bitSetAdd(&setB, 5);
    bitSetAdd(&setB, 8);
    
    printf("Set A: ");
    bitSetPrint(&setA);
    printf("Set B: ");
    bitSetPrint(&setB);
    
    printf("\nUnion (A ∪ B): ");
    bitSetUnion(&result, &setA, &setB);
    bitSetPrint(&result);
    
    printf("Intersection (A ∩ B): ");
    bitSetIntersection(&result, &setA, &setB);
    bitSetPrint(&result);
    
    printf("Difference (A - B): ");
    bitSetDifference(&result, &setA, &setB);
    bitSetPrint(&result);
    
    printf("Symmetric Difference (A Δ B): ");
    bitSetSymmetricDifference(&result, &setA, &setB);
    bitSetPrint(&result);
}
```

#### Example 3: Permission Bits (Unix-style)

```c
/**
 * FILE PERMISSIONS (Unix-style chmod)
 */

// Permission bit masks
#define PERM_OWNER_READ    0400  // Octal: 0400 = Binary: 100 000 000
#define PERM_OWNER_WRITE   0200  // Octal: 0200 = Binary: 010 000 000
#define PERM_OWNER_EXEC    0100  // Octal: 0100 = Binary: 001 000 000
#define PERM_GROUP_READ    0040  // Octal: 0040 = Binary: 000 100 000
#define PERM_GROUP_WRITE   0020  // Octal: 0020 = Binary: 000 010 000
#define PERM_GROUP_EXEC    0010  // Octal: 0010 = Binary: 000 001 000
#define PERM_OTHERS_READ   0004  // Octal: 0004 = Binary: 000 000 100
#define PERM_OTHERS_WRITE  0002  // Octal: 0002 = Binary: 000 000 010
#define PERM_OTHERS_EXEC   0001  // Octal: 0001 = Binary: 000 000 001

// Combined masks
#define PERM_ALL_READ   (PERM_OWNER_READ | PERM_GROUP_READ | PERM_OTHERS_READ)
#define PERM_ALL_WRITE  (PERM_OWNER_WRITE | PERM_GROUP_WRITE | PERM_OTHERS_WRITE)
#define PERM_ALL_EXEC   (PERM_OWNER_EXEC | PERM_GROUP_EXEC | PERM_OTHERS_EXEC)

typedef struct {
    char name[256];
    unsigned int permissions;
} File;

// Set permissions using octal notation (like chmod 755)
void setPermissionsOctal(File* file, int octal) {
    file->permissions = octal;
}

// Grant specific permissions
void grantPermissions(File* file, unsigned int perms) {
    file->permissions |= perms;
}

// Revoke specific permissions
void revokePermissions(File* file, unsigned int perms) {
    file->permissions &= ~perms;
}

// Check specific permission
bool hasPermission(File* file, unsigned int perm) {
    return (file->permissions & perm) != 0;
}

// Check if user can read (owner check)
bool canRead(File* file, bool isOwner, bool isGroup) {
    if (isOwner) return hasPermission(file, PERM_OWNER_READ);
    if (isGroup) return hasPermission(file, PERM_GROUP_READ);
    return hasPermission(file, PERM_OTHERS_READ);
}

// Display permissions in ls -l format
void displayPermissions(File* file) {
    printf("%s: ", file->name);
    
    // Owner
    printf(hasPermission(file, PERM_OWNER_READ) ? "r" : "-");
    printf(hasPermission(file, PERM_OWNER_WRITE) ? "w" : "-");
    printf(hasPermission(file, PERM_OWNER_EXEC) ? "x" : "-");
    
    // Group
    printf(hasPermission(file, PERM_GROUP_READ) ? "r" : "-");
    printf(hasPermission(file, PERM_GROUP_WRITE) ? "w" : "-");
    printf(hasPermission(file, PERM_GROUP_EXEC) ? "x" : "-");
    
    // Others
    printf(hasPermission(file, PERM_OTHERS_READ) ? "r" : "-");
    printf(hasPermission(file, PERM_OTHERS_WRITE) ? "w" : "-");
    printf(hasPermission(file, PERM_OTHERS_EXEC) ? "x" : "-");
    
    printf(" (%03o)\n", file->permissions);
}

// Convert permission string to octal
unsigned int permStringToOctal(const char* str) {
    // str format: "rwxr-xr--" (9 characters)
    unsigned int perm = 0;
    
    if (str[0] == 'r') perm |= PERM_OWNER_READ;
    if (str[1] == 'w') perm |= PERM_OWNER_WRITE;
    if (str[2] == 'x') perm |= PERM_OWNER_EXEC;
    if (str[3] == 'r') perm |= PERM_GROUP_READ;
    if (str[4] == 'w') perm |= PERM_GROUP_WRITE;
    if (str[5] == 'x') perm |= PERM_GROUP_EXEC;
    if (str[6] == 'r') perm |= PERM_OTHERS_READ;
    if (str[7] == 'w') perm |= PERM_OTHERS_WRITE;
    if (str[8] == 'x') perm |= PERM_OTHERS_EXEC;
    
    return perm;
}

// Test file permissions
void testFilePermissions() {
    File myFile;
    strcpy(myFile.name, "data.txt");
    
    // Set permission to 644 (rw-r--r--)
    setPermissionsOctal(&myFile, 0644);
    displayPermissions(&myFile);
    
    // Change to 755 (rwxr-xr-x)
    setPermissionsOctal(&myFile, 0755);
    displayPermissions(&myFile);
    
    // Revoke write for everyone
    revokePermissions(&myFile, PERM_ALL_WRITE);
    displayPermissions(&myFile);
    
    // Grant write to owner only
    grantPermissions(&myFile, PERM_OWNER_WRITE);
    displayPermissions(&myFile);
}
```

### 9.4 Practice Problems

#### Beginner Level

1. **Create Bit Mask**
   - Input: `pos = 3`, `n = 4`
   - Output: `0b01111000` (mask for bits 3-6)
   - Hint: Shift and subtract

2. **Extract Field**
   - Input: `value = 0xABCD`, `mask = 0x0F00`
   - Output: `0xC`
   - Hint: AND then shift

3. **Set Flags**
   - Input: `value = 0x00`, `flags = 0x05`
   - Output: `0x05`
   - Hint: OR operation

4. **Clear Flags**
   - Input: `value = 0xFF`, `flags = 0x0F`
   - Output: `0xF0`
   - Hint: AND with NOT

5. **Toggle Flags**
   - Input: `value = 0xAA`, `flags = 0xFF`
   - Output: `0x55`
   - Hint: XOR operation

#### Intermediate Level

6. **Replace Bits**
   - Input: `value = 0xFF00`, `newBits = 0x00AB`, `mask = 0x00FF`
   - Output: `0xFFAB`
   - Hint: Clear then set

7. **Bitmap Set Operations**
   - Implement union, intersection, difference
   - Hint: Use bitwise OR, AND, AND NOT

8. **Status Register**
   - Design and implement status flags
   - Hint: Define bit positions

9. **Permission System**
   - Implement chmod-like permissions
   - Hint: 9 bits for rwx × 3 users

10. **Color Packing**
    - Pack/unpack RGBA into 32-bit int
    - Hint: 8 bits per channel

---

## Section 10: Subset Generation Pattern

### 10.1 Pattern Overview

**Key Insight:** For a set of n elements, there are 2^n possible subsets. Each subset can be represented by an n-bit number where bit i indicates if element i is included.

**Example:**
```
Set: {a, b, c}
Subsets: 2^3 = 8

000 = {}
001 = {c}
010 = {b}
011 = {b, c}
100 = {a}
101 = {a, c}
110 = {a, b}
111 = {a, b, c}
```

### 10.2 Subset Generation

```c
/**
 * SUBSET GENERATION USING BITS
 */

// Generate all subsets of array
void generateSubsets(int* nums, int numsSize) {
    int totalSubsets = 1 << numsSize;  // 2^n
    
    printf("All subsets:\n");
    for (int mask = 0; mask < totalSubsets; mask++) {
        printf("{");
        bool first = true;
        
        for (int i = 0; i < numsSize; i++) {
            // Check if i-th bit is set
            if (mask & (1 << i)) {
                if (!first) printf(", ");
                printf("%d", nums[i]);
                first = false;
            }
        }
        
        printf("}\n");
    }
}

// Generate all subsets of size k
void generateSubsetsOfSizeK(int* nums, int numsSize, int k) {
    int totalSubsets = 1 << numsSize;
    
    printf("Subsets of size %d:\n", k);
    for (int mask = 0; mask < totalSubsets; mask++) {
        // Count set bits
        if (__builtin_popcount(mask) == k) {
            printf("{");
            bool first = true;
            
            for (int i = 0; i < numsSize; i++) {
                if (mask & (1 << i)) {
                    if (!first) printf(", ");
                    printf("%d", nums[i]);
                    first = false;
                }
            }
            
            printf("}\n");
        }
    }
}

// Return all subsets as array of arrays
int** subsets(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    int totalSubsets = 1 << numsSize;
    *returnSize = totalSubsets;
    
    int** result = (int**)malloc(totalSubsets * sizeof(int*));
    *returnColumnSizes = (int*)malloc(totalSubsets * sizeof(int));
    
    for (int mask = 0; mask < totalSubsets; mask++) {
        int subsetSize = __builtin_popcount(mask);
        result[mask] = (int*)malloc(subsetSize * sizeof(int));
        (*returnColumnSizes)[mask] = subsetSize;
        
        int idx = 0;
        for (int i = 0; i < numsSize; i++) {
            if (mask & (1 << i)) {
                result[mask][idx++] = nums[i];
            }
        }
    }
    
    return result;
}

// Iterate through subsets efficiently (Gosper's hack for fixed k)
void iterateSubsetsOfSizeK(int n, int k) {
    printf("Subsets of size %d from %d elements:\n", k, n);
    
    unsigned int subset = (1 << k) - 1;  // First k bits set
    unsigned int limit = (1 << n);
    
    while (subset < limit) {
        // Print current subset
        printf("Mask: ");
        printBinary(subset);
        
        // Gosper's hack: find next subset with same number of bits
        unsigned int c = subset & -subset;
        unsigned int r = subset + c;
        subset = (((r ^ subset) >> 2) / c) | r;
    }
}

// Test subset generation
void testSubsetGeneration() {
    int nums[] = {1, 2, 3};
    int size = 3;
    
    printf("Array: {1, 2, 3}\n\n");
    generateSubsets(nums, size);
    
    printf("\n");
    generateSubsetsOfSizeK(nums, size, 2);
}
```

### 10.3 Practical Examples

#### Example 1: Subset Sum Problem

```c
/**
 * SUBSET SUM PROBLEM
 * Find if there's a subset with given sum
 */

bool subsetSum(int* nums, int numsSize, int target) {
    int totalSubsets = 1 << numsSize;
    
    for (int mask = 0; mask < totalSubsets; mask++) {
        int sum = 0;
        
        for (int i = 0; i < numsSize; i++) {
            if (mask & (1 << i)) {
                sum += nums[i];
            }
        }
        
        if (sum == target) {
            // Found a subset
            printf("Subset with sum %d: {", target);
            bool first = true;
            for (int i = 0; i < numsSize; i++) {
                if (mask & (1 << i)) {
                    if (!first) printf(", ");
                    printf("%d", nums[i]);
                    first = false;
                }
            }
            printf("}\n");
            return true;
        }
    }
    
    return false;
}

// Find all subsets with given sum
void allSubsetSums(int* nums, int numsSize, int target) {
    int totalSubsets = 1 << numsSize;
    int count = 0;
    
    printf("All subsets with sum %d:\n", target);
    
    for (int mask = 0; mask < totalSubsets; mask++) {
        int sum = 0;
        
        for (int i = 0; i < numsSize; i++) {
            if (mask & (1 << i)) {
                sum += nums[i];
            }
        }
        
        if (sum == target) {
            count++;
            printf("%d: {", count);
            bool first = true;
            for (int i = 0; i < numsSize; i++) {
                if (mask & (1 << i)) {
                    if (!first) printf(", ");
                    printf("%d", nums[i]);
                    first = false;
                }
            }
            printf("}\n");
        }
    }
    
    printf("Total: %d subsets\n", count);
}

// Test subset sum
void testSubsetSum() {
    int nums[] = {3, 34, 4, 12, 5, 2};
    int target = 9;
    
    printf("Array: {3, 34, 4, 12, 5, 2}\n");
    printf("Target: %d\n\n", target);
    
    allSubsetSums(nums, 6, target);
}
```

#### Example 2: Maximum XOR Subset

```c
/**
 * MAXIMUM XOR OF SUBSET
 * Find subset with maximum XOR value
 */

int maxSubsetXOR(int* nums, int numsSize) {
    int totalSubsets = 1 << numsSize;
    int maxXOR = 0;
    int bestMask = 0;
    
    for (int mask = 1; mask < totalSubsets; mask++) {  // Skip empty set
        int xorValue = 0;
        
        for (int i = 0; i < numsSize; i++) {
            if (mask & (1 << i)) {
                xorValue ^= nums[i];
            }
        }
        
        if (xorValue > maxXOR) {
            maxXOR = xorValue;
            bestMask = mask;
        }
    }
    
    // Print best subset
    printf("Maximum XOR: %d\n", maxXOR);
    printf("Subset: {");
    bool first = true;
    for (int i = 0; i < numsSize; i++) {
        if (bestMask & (1 << i)) {
            if (!first) printf(", ");
            printf("%d", nums[i]);
            first = false;
        }
    }
    printf("}\n");
    
    return maxXOR;
}

// Test maximum XOR
void testMaxXOR() {
    int nums[] = {2, 4, 5};
    printf("Array: {2, 4, 5}\n");
    maxSubsetXOR(nums, 3);
}
```

#### Example 3: Partition into Equal Sum Subsets

```c
/**
 * PARTITION EQUAL SUBSET SUM
 * Can array be partitioned into two subsets with equal sum?
 */

bool canPartition(int* nums, int numsSize) {
    // Calculate total sum
    int totalSum = 0;
    for (int i = 0; i < numsSize; i++) {
        totalSum += nums[i];
    }
    
    // If odd sum, can't partition equally
    if (totalSum % 2 != 0) return false;
    
    int target = totalSum / 2;
    
    // Use bit manipulation for DP
    // bit[i] = 1 means sum i is achievable
    unsigned long long dp = 1;  // Empty set has sum 0
    
    for (int i = 0; i < numsSize; i++) {
        // For each number, update achievable sums
        dp |= (dp << nums[i]);
    }
    
    // Check if target sum is achievable
    return (dp >> target) & 1;
}

// Find partition (if exists)
void findPartition(int* nums, int numsSize) {
    int totalSum = 0;
    for (int i = 0; i < numsSize; i++) {
        totalSum += nums[i];
    }
    
    if (totalSum % 2 != 0) {
        printf("Cannot partition into equal sums\n");
        return;
    }
    
    int target = totalSum / 2;
    int totalSubsets = 1 << numsSize;
    
    for (int mask = 0; mask < totalSubsets; mask++) {
        int sum = 0;
        
        for (int i = 0; i < numsSize; i++) {
            if (mask & (1 << i)) {
                sum += nums[i];
            }
        }
        
        if (sum == target) {
            printf("Partition found!\n");
            printf("Subset 1: {");
            bool first = true;
            for (int i = 0; i < numsSize; i++) {
                if (mask & (1 << i)) {
                    if (!first) printf(", ");
                    printf("%d", nums[i]);
                    first = false;
                }
            }
            printf("}\n");
            
            printf("Subset 2: {");
            first = true;
            for (int i = 0; i < numsSize; i++) {
                if (!(mask & (1 << i))) {
                    if (!first) printf(", ");
                    printf("%d", nums[i]);
                    first = false;
                }
            }
            printf("}\n");
            return;
        }
    }
    
    printf("Cannot partition into equal sums\n");
}

// Test partition
void testPartition() {
    int nums[] = {1, 5, 11, 5};
    printf("Array: {1, 5, 11, 5}\n");
    printf("Total sum: 22, target: 11\n\n");
    findPartition(nums, 4);
}
```

### 10.4 Practice Problems

#### Beginner Level

1. **Generate All Subsets**
   - Input: `[1,2,3]`
   - Output: All 8 subsets
   - Hint: Iterate 0 to 2^n-1

2. **Subsets of Size K**
   - Input: `nums=[1,2,3,4]`, `k=2`
   - Output: All subsets with 2 elements
   - Hint: Check popcount

3. **Subset Sum Exists**
   - Input: `nums=[3,34,4,12,5,2]`, `target=9`
   - Output: `true`
   - Hint: Try all subsets

4. **Count Subsets with Sum**
   - Input: `nums=[1,2,3]`, `target=3`
   - Output: `2` (subsets: {3}, {1,2})
   - Hint: Count matching subsets

5. **Empty Subset Check**
   - Input: Any array
   - Output: Always includes empty set
   - Hint: mask=0

#### Intermediate Level

6. **Partition Equal Subset Sum**
   - Input: `[1,5,11,5]`
   - Output: `true` (can partition to {1,5,5} and {11})
   - Hint: Check if sum/2 achievable

7. **Maximum XOR Subset**
   - Input: `[2,4,5]`
   - Output: `7` (subset {2,5})
   - Hint: Try all subsets, track max XOR

8. **Subset with Max Sum**
   - Input: `[-1,2,3,-4]`
   - Output: `5` (subset {2,3})
   - Hint: Consider only positives

9. **Minimum Difference Subsets**
   - Input: `[1,6,11,5]`
   - Output: `1` (partition to {1,5,6} and {11})
   - Hint: Find partition with min difference

10. **Subset Product**
    - Input: `nums=[2,3,5]`, `target=30`
    - Output: `true` (subset {2,3,5})
    - Hint: Similar to subset sum

---

## Section 11: Bit Manipulation Tricks

### 11.1 Essential Tricks Collection

```c
/**
 * COMPREHENSIVE BIT MANIPULATION TRICKS
 */

// ========== BASIC TRICKS ==========

// 1. Check if n is even
bool isEven(int n) {
    return (n & 1) == 0;
}

// 2. Check if n is odd
bool isOdd(int n) {
    return (n & 1) == 1;
}

// 3. Multiply by 2^k
int multiplyBy2Power(int n, int k) {
    return n << k;
}

// 4. Divide by 2^k (positive numbers)
int divideBy2Power(int n, int k) {
    return n >> k;
}

// 5. Get absolute value
int absoluteValue(int n) {
    int mask = n >> 31;  // All 1s if negative, all 0s if positive
    return (n + mask) ^ mask;
    // Alternative: (n ^ mask) - mask
}

// ========== POWER OF 2 TRICKS ==========

// 6. Check if power of 2
bool isPowerOf2(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// 7. Next power of 2
unsigned int nextPowerOf2(unsigned int n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

// 8. Check if divisible by 2^k
bool isDivisibleBy2Power(int n, int k) {
    int divisor = 1 << k;
    return (n & (divisor - 1)) == 0;
}

// ========== BIT ISOLATION TRICKS ==========

// 9. Isolate rightmost set bit
int isolateRightmostBit(int n) {
    return n & -n;
}

// 10. Clear rightmost set bit
int clearRightmostBit(int n) {
    return n & (n - 1);
}

// 11. Isolate rightmost 0 bit
int isolateRightmost0Bit(int n) {
    return ~n & (n + 1);
}

// 12. Set rightmost 0 bit
int setRightmost0Bit(int n) {
    return n | (n + 1);
}

// 13. Get rightmost different bit between x and y
int rightmostDifferentBit(int x, int y) {
    return (x ^ y) & -(x ^ y);
}

// ========== SIGN TRICKS ==========

// 14. Check if opposite signs
bool oppositeSigns(int x, int y) {
    return (x ^ y) < 0;
}

// 15. Get sign (-1, 0, or 1)
int sign(int n) {
    return (n > 0) - (n < 0);
}

// 16. Min of two integers without branching
int minNoBranch(int a, int b) {
    return b ^ ((a ^ b) & -(a < b));
}

// 17. Max of two integers without branching
int maxNoBranch(int a, int b) {
    return a ^ ((a ^ b) & -(a < b));
}

// ========== SWAP TRICKS ==========

// 18. Swap without temp variable
void swapXOR(int* a, int* b) {
    if (a != b) {
        *a ^= *b;
        *b ^= *a;
        *a ^= *b;
    }
}

// 19. Swap using addition/subtraction
void swapAddSub(int* a, int* b) {
    if (a != b) {
        *a = *a + *b;
        *b = *a - *b;
        *a = *a - *b;
    }
}

// ========== ROTATION TRICKS ==========

// 20. Rotate left
unsigned int rotateLeft(unsigned int n, int d) {
    return (n << d) | (n >> (32 - d));
}

// 21. Rotate right
unsigned int rotateRight(unsigned int n, int d) {
    return (n >> d) | (n << (32 - d));
}

// ========== REVERSE TRICKS ==========

// 22. Reverse bits in byte
unsigned char reverseByte(unsigned char b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// 23. Reverse bits in 32-bit integer
unsigned int reverseBits(unsigned int n) {
    n = ((n & 0xFFFF0000) >> 16) | ((n & 0x0000FFFF) << 16);
    n = ((n & 0xFF00FF00) >> 8)  | ((n & 0x00FF00FF) << 8);
    n = ((n & 0xF0F0F0F0) >> 4)  | ((n & 0x0F0F0F0F) << 4);
    n = ((n & 0xCCCCCCCC) >> 2)  | ((n & 0x33333333) << 2);
    n = ((n & 0xAAAAAAAA) >> 1)  | ((n & 0x55555555) << 1);
    return n;
}

// ========== COUNTING TRICKS ==========

// 24. Count trailing zeros
int countTrailingZeros(unsigned int n) {
    if (n == 0) return 32;
    return __builtin_ctz(n);
}

// 25. Count leading zeros
int countLeadingZeros(unsigned int n) {
    if (n == 0) return 32;
    return __builtin_clz(n);
}

// 26. Parity (even/odd number of 1s)
bool parity(unsigned int n) {
    return __builtin_parity(n);
}

// ========== MASKING TRICKS ==========

// 27. Turn off rightmost consecutive 1s
int turnOffRightmost1s(int n) {
    return n & (n + 1);
}

// 28. Turn on rightmost consecutive 0s
int turnOnRightmost0s(int n) {
    return n | (n - 1);
}

// 29. Create mask of n ones
unsigned int createOnes(int n) {
    return (1U << n) - 1;
}

// 30. Check if only one bit is set
bool onlyOneBitSet(unsigned int n) {
    return n != 0 && (n & (n - 1)) == 0;
}

// ========== ADVANCED TRICKS ==========

// 31. Average of two integers without overflow
int averageNoOverflow(int a, int b) {
    return (a & b) + ((a ^ b) >> 1);
}

// 32. Modulo by power of 2
int moduloPowerOf2(int n, int powerOf2) {
    return n & (powerOf2 - 1);
}

// 33. Check if addition would overflow
bool willAddOverflow(int a, int b) {
    int sum = a + b;
    return ((a ^ sum) & (b ^ sum)) < 0;
}

// 34. Set all bits after position n
unsigned int setAllAfter(int n) {
    return (1U << (n + 1)) - 1;
}

// 35. Clear all bits after position n
unsigned int clearAllAfter(unsigned int num, int n) {
    return num & ~((1U << (n + 1)) - 1);
}

// 36. Toggle bit range [i, j]
int toggleRange(int num, int i, int j) {
    int len = j - i + 1;
    int mask = ((1 << len) - 1) << i;
    return num ^ mask;
}

// 37. Copy bit from position src to position dst
int copyBit(int num, int src, int dst) {
    int bit = (num >> src) & 1;
    return (num & ~(1 << dst)) | (bit << dst);
}

// 38. Swap adjacent bits
unsigned int swapAdjacentBits(unsigned int n) {
    return ((n & 0xAAAAAAAA) >> 1) | ((n & 0x55555555) << 1);
}

// 39. Get bit value at position
int getBitValue(int num, int pos) {
    return (num >> pos) & 1;
}

// 40. Count bits to flip to convert A to B
int bitsToFlip(int a, int b) {
    return __builtin_popcount(a ^ b);
}
```

### 11.2 Performance Optimizations

```c
/**
 * PERFORMANCE TRICKS
 */

// Fast multiplication by constants
int multiply10(int n) {
    // n * 10 = (n << 3) + (n << 1)
    return (n << 3) + (n << 1);
}

int multiply9(int n) {
    // n * 9 = (n << 3) + n
    return (n << 3) + n;
}

int multiply7(int n) {
    // n * 7 = (n << 3) - n
    return (n << 3) - n;
}

// Fast division by 3 (approximation)
int divideBy3(int n) {
    // Only works for small positive numbers
    return (n * 0xAAAAAAAB) >> 33;
}

// Check if n is multiple of 3 without division
bool isMultipleOf3(int n) {
    int count = 0;
    if (n < 0) n = -n;
    
    while (n) {
        count ^= (n & 1);
        n >>= 1;
    }
    
    return count == 0;
}

// Check if n is multiple of 9
bool isMultipleOf9(int n) {
    if (n < 0) n = -n;
    
    while (n > 9) {
        n = (n >> 4) + (n & 0xF);
    }
    
    return n == 0 || n == 9;
}

// Count bits faster with lookup table
static int bitCountTable[256];

void initBitCountTable() {
    for (int i = 0; i < 256; i++) {
        bitCountTable[i] = (i & 1) + bitCountTable[i / 2];
    }
}

int countBitsFast(unsigned int n) {
    return bitCountTable[n & 0xFF] + 
           bitCountTable[(n >> 8) & 0xFF] +
           bitCountTable[(n >> 16) & 0xFF] +
           bitCountTable[(n >> 24) & 0xFF];
}
```

### 11.3 Practice Problems

#### Beginner Level

1. **Swap Numbers**
   - Swap two integers without temp
   - Hint: XOR swap

2. **Reverse Bits**
   - Reverse bit pattern
   - Hint: Swap and shift

3. **Absolute Value**
   - Get absolute without branching
   - Hint: Use sign mask

4. **Min/Max Without Branch**
   - Find min/max without if
   - Hint: XOR trick

5. **Rotate Bits**
   - Rotate left/right
   - Hint: Shift and OR

#### Intermediate Level

6. **Gray Code**
   - Convert binary to Gray code
   - Hint: n ^ (n >> 1)

7. **Next Sparse Number**
   - Find next number with no adjacent 1s
   - Hint: Check consecutive bits

8. **Bit Difference**
   - Count bits to flip A to B
   - Hint: XOR and count

9. **Add Binary Numbers**
   - Add without + operator
   - Hint: XOR and carry

10. **Multiply Without * Operator**
    - Multiply using shifts
    - Hint: Add shifted values

---

## Section 12: Gray Code Pattern

### 12.1 Pattern Overview

**Gray Code:** Binary numeral system where consecutive values differ in only one bit.

**Example (3-bit):**
```
Binary  Gray
000  →  000
001  →  001
010  →  011
011  →  010
100  →  110
101  →  111
110  →  101
111  →  100
```

**Conversion Formula:**
```
Gray = Binary ^ (Binary >> 1)
```

### 12.2 Gray Code Operations

```c
/**
 * GRAY CODE OPERATIONS
 */

// Convert binary to Gray code
unsigned int binaryToGray(unsigned int binary) {
    return binary ^ (binary >> 1);
}

// Convert Gray code to binary
unsigned int grayToBinary(unsigned int gray) {
    unsigned int binary = gray;
    
    while (gray >>= 1) {
        binary ^= gray;
    }
    
    return binary;
}

// Generate all n-bit Gray codes
int* grayCode(int n, int* returnSize) {
    int size = 1 << n;  // 2^n codes
    *returnSize = size;
    
    int* result = (int*)malloc(size * sizeof(int));
    
    for (int i = 0; i < size; i++) {
        result[i] = binaryToGray(i);
    }
    
    return result;
}

// Generate Gray codes recursively (reflected binary)
void grayCodeRecursive(int n, int* result, int* index, int current) {
    if (n == 0) {
        result[(*index)++] = current;
        return;
    }
    
    // Generate codes with 0 prefix
    grayCodeRecursive(n - 1, result, index, current);
    
    // Generate codes with 1 prefix (reflected)
    grayCodeRecursive(n - 1, result, index, current | (1 << (n - 1)));
}

// Check if two numbers are adjacent in Gray code
bool areAdjacentInGray(unsigned int a, unsigned int b) {
    unsigned int diff = a ^ b;
    return (diff & (diff - 1)) == 0;  // Only one bit different
}

// Get next Gray code
unsigned int nextGray(unsigned int current) {
    unsigned int binary = grayToBinary(current);
    return binaryToGray(binary + 1);
}

// Test Gray code
void testGrayCode() {
    printf("3-bit Gray codes:\n");
    printf("Binary → Gray\n");
    
    for (int i = 0; i < 8; i++) {
        unsigned int gray = binaryToGray(i);
        printf("%03d → %03d (", i, gray);
        printBinary(gray & 0x7);
        printf(")\n");
    }
    
    printf("\nGray → Binary verification:\n");
    for (int i = 0; i < 8; i++) {
        unsigned int gray = binaryToGray(i);
        unsigned int binary = grayToBinary(gray);
        printf("Gray %03d → Binary %03d %s\n", 
               gray, binary, (binary == i) ? "✓" : "✗");
    }
}
```

### 12.3 Applications

```c
/**
 * GRAY CODE APPLICATIONS
 */

// Karnaugh map optimization
// Gray code ensures adjacent cells differ by one variable

// Position encoding in rotary encoders
// Reduces errors when reading mechanical position

// Error correction in digital communications
// Adjacent codes differ by only one bit

// Genetic algorithms
// Smooth transition between chromosome values

// Example: Generate Hamiltonian path on hypercube
void hypercubeHamiltonianPath(int n) {
    int size = 1 << n;
    
    printf("Hamiltonian path on %d-dimensional hypercube:\n", n);
    
    for (int i = 0; i < size; i++) {
        unsigned int gray = binaryToGray(i);
        
        for (int bit = n - 1; bit >= 0; bit--) {
            printf("%d", (gray >> bit) & 1);
        }
        
        if (i < size - 1) printf(" → ");
    }
    printf("\n");
}
```

---

## Section 13: Hamming Distance Pattern

### 13.1 Pattern Overview

**Hamming Distance:** Number of positions at which corresponding bits are different.

**Formula:**
```
HammingDistance(x, y) = popcount(x XOR y)
```

### 13.2 Hamming Operations

```c
/**
 * HAMMING DISTANCE OPERATIONS
 */

// Hamming distance between two integers
int hammingDistance(int x, int y) {
    return __builtin_popcount(x ^ y);
}

// Total Hamming distance of all pairs
int totalHammingDistance(int* nums, int numsSize) {
    int total = 0;
    
    // For each bit position
    for (int bit = 0; bit < 32; bit++) {
        int countOnes = 0;
        
        // Count numbers with this bit set
        for (int i = 0; i < numsSize; i++) {
            if (nums[i] & (1 << bit)) {
                countOnes++;
            }
        }
        
        // Each 1 pairs with each 0
        int countZeros = numsSize - countOnes;
        total += countOnes * countZeros;
    }
    
    return total;
}

// Hamming weight (number of 1s)
int hammingWeight(unsigned int n) {
    return __builtin_popcount(n);
}

// Find number with minimum Hamming distance to target
int minHammingDistance(int* nums, int numsSize, int target) {
    int minDist = 32;
    int result = -1;
    
    for (int i = 0; i < numsSize; i++) {
        int dist = hammingDistance(nums[i], target);
        if (dist < minDist) {
            minDist = dist;
            result = nums[i];
        }
    }
    
    return result;
}

// Check if Hamming distance is exactly k
bool isHammingDistanceK(int x, int y, int k) {
    return hammingDistance(x, y) == k;
}

// Test Hamming distance
void testHammingDistance() {
    printf("Hamming distance between 1 and 4: %d\n", hammingDistance(1, 4));
    // 1 = 001, 4 = 100, distance = 2
    
    int nums[] = {4, 14, 2};
    printf("Total Hamming distance: %d\n", totalHammingDistance(nums, 3));
    
    printf("Hamming weight of 7: %d\n", hammingWeight(7));  // 111 = 3 ones
}
```

---

## Section 14: Problem-Solving Framework

### 14.1 Decision Tree

```
BITWISE PROBLEM? Ask these questions:

Q1: Does problem mention bits explicitly?
└─ YES → Use bitwise approach
└─ NO → Check if bitwise optimization possible

Q2: What operation is needed?
├─ Set/Clear/Toggle bits → Use |, &~, ^
├─ Check bits → Use & with mask
├─ Count bits → Use popcount
├─ Find patterns → Use XOR properties
├─ Generate subsets → Use bitmasks
└─ Optimize arithmetic → Use shifts

Q3: What's the key property?
├─ Power of 2 → Use n & (n-1)
├─ Pairs cancel → Use XOR
├─ Position matters → Use shifts
└─ Range operations → Use masks

Q4: What's the constraint?
├─ No extra space → In-place bit manipulation
├─ Fast execution → Bit tricks over loops
└─ Small values → Bitmask DP possible
```

### 14.2 Common Problem Patterns

```c
/**
 * PROBLEM PATTERN RECOGNITION
 */

// Pattern 1: Find unique element (others appear even times)
// Solution: XOR all elements
int findUnique(int* nums, int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= nums[i];
    }
    return result;
}

// Pattern 2: Check property of number
// Solution: Use specific bit patterns
bool checkProperty(int n) {
    // Is power of 2?
    if ((n > 0) && (n & (n - 1)) == 0) return true;
    
    // Is power of 4?
    if ((n > 0) && (n & (n - 1)) == 0 && (n & 0x55555555)) return true;
    
    return false;
}

// Pattern 3: Optimize arithmetic
// Solution: Replace with bit operations
int optimizeArithmetic(int n) {
    // Multiply by 8
    int result = n << 3;
    
    // Divide by 4
    result = result >> 2;
    
    // Modulo 16
    result = result & 15;
    
    return result;
}

// Pattern 4: Generate all possibilities
// Solution: Use bitmask iteration
void generateAll(int n) {
    int total = 1 << n;
    
    for (int mask = 0; mask < total; mask++) {
        // Process this combination
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                // Element i is included
            }
        }
    }
}

// Pattern 5: Bit manipulation DP
// Solution: Use bitmask for state
int bitmaskDP(int n) {
    int dp[1 << n];
    
    dp[0] = 0;  // Base case
    
    for (int mask = 0; mask < (1 << n); mask++) {
        // Transition to new states
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) {
                int newMask = mask | (1 << i);
                // Update dp[newMask]
            }
        }
    }
    
    return dp[(1 << n) - 1];
}
```

---

## Section 15: Common Bitwise Problems

### 15.1 Problem Categories

**Category 1: Single/Missing Number**
- Single Number (all others twice)
- Missing Number  
- Find Duplicate
- Two Single Numbers

**Category 2: Bit Counting**
- Number of 1 Bits
- Counting Bits (0 to n)
- Hamming Distance
- Binary Watch

**Category 3: Power of N**
- Power of Two
- Power of Four
- Next Power of 2

**Category 4: Bit Manipulation**
- Reverse Bits
- Add Binary
- Sum of Two Integers (no + operator)
- Divide Two Integers (no / operator)

**Category 5: Subsets**
- Subsets
- Subset Sum
- Partition Equal Subset Sum
- Maximum XOR

---

## Section 16: Practice Roadmap

### 16.1 Week-by-Week Plan

**Week 1: Foundations**
- Day 1-2: Binary basics, operators
- Day 3-4: Set/clear/toggle bits
- Day 5-6: Checking and extracting
- Day 7: Review and practice

**Week 2: Core Patterns**
- Day 1-2: Counting bits
- Day 3-4: Power of two
- Day 5-6: XOR magic
- Day 7: Review

**Week 3: Advanced**
- Day 1-2: Bit masking
- Day 3-4: Subset generation
- Day 5-6: Tricks and optimizations
- Day 7: Review

**Week 4: Applications**
- Day 1-2: Gray code, Hamming
- Day 3-4: Complex problems
- Day 5-6: Contest problems
- Day 7: Final review

### 16.2 Problem List (60 Essential Problems)

#### Easy (20 problems)
1. Single Number
2. Number of 1 Bits
3. Reverse Bits
4. Power of Two
5. Missing Number
6. Hamming Distance
7. Binary Watch
8. Prime Number of Set Bits
9. Binary Number with Alternating Bits
10. Number Complement
11. Convert to Base -2
12. Minimum Bit Flips
13. Sort Integers by The Number of 1 Bits
14. XOR Operation in Array
15. Decode XORed Array
16. Count Triplets XOR Equal to Zero
17. Find XOR Sum of All Pairs
18. Maximum XOR for Each Query
19. Minimum One Bit Operations
20. Count Subsets Sum Equal to Target

#### Medium (25 problems)
21. Single Number II (3 times)
22. Single Number III (two singles)
23. Counting Bits
24. Maximum XOR of Two Numbers
25. Total Hamming Distance
26. Binary Number to String
27. UTF-8 Validation
28. Power of Four
29. Bitwise AND of Range
30. Majority Element II
31. Find Missing and Duplicate
32. Subsets
33. Gray Code
34. Hamming Distance
35. Maximum Product of Word Lengths
36. Partition Equal Subset Sum
37. Beautiful Arrangement
38. Can I Win
39. Shortest Path Visiting All Nodes
40. Number of Ways to Wear Hats
41. Minimum Cost to Connect Cities
42. Maximum Students Taking Exam
43. Number of Squareful Arrays
44. Smallest Sufficient Team
45. Fair Distribution of Cookies

#### Hard (15 problems)
46. Maximum XOR With Element From Array
47. Maximum Genetic Difference Query
48. Minimum XOR Sum of Two Arrays
49. Count Subtrees With Max Distance
50. Number of Valid Move Combinations
51. Maximum Compatibility Score Sum
52. Minimize Hamming Distance After Swap
53. Minimum Number of Work Sessions
54. Maximum Value at a Given Index
55. Count All Valid Pickup and Delivery
56. Number of Ways to Reconstruct a Tree
57. Minimum Total Space Wasted With K Rooms
58. Minimum Incompatibility
59. Maximum Number of Achievable Tasks
60. Find Minimum Time to Finish All Jobs

---

## CONCLUSION

### Key Takeaways

1. **Master the Fundamentals**
   - Understand binary representation
   - Know all six bitwise operators
   - Practice basic operations daily

2. **Recognize Patterns**
   - XOR for finding unique elements
   - Power of 2 checks with n & (n-1)
   - Bitmasks for subset generation
   - Bit manipulation for optimization

3. **Think in Bits**
   - Visualize binary representations
   - Consider bit-level operations
   - Look for bit manipulation optimizations

4. **Common Applications**
   - Flags and status registers
   - Permissions and access control
   - Memory-efficient data structures
   - Performance optimizations
   - Combinatorial problems

5. **Practice Strategy**
   - Start with easy problems
   - Master one pattern at a time
   - Solve 5-10 problems per pattern
   - Review and optimize solutions

### Quick Reference Card

```
OPERATION          CODE               RESULT
────────────────────────────────────────────────
Set bit            n | (1 << i)      Sets bit i
Clear bit          n & ~(1 << i)     Clears bit i
Toggle bit         n ^ (1 << i)      Toggles bit i
Check bit          n & (1 << i)      Checks bit i
Power of 2         n & (n-1) == 0    Checks power of 2
Count bits         __builtin_popcount Counts 1s
XOR swap           a^=b; b^=a; a^=b  Swaps a and b
Isolate rightmost  n & -n            Gets rightmost 1
Clear rightmost    n & (n-1)         Removes rightmost 1
Next power 2       See algorithm     Finds next power
Absolute value     (n^(n>>31))-(n>>31) Gets |n|
```

### Final Tips

- **Debugging**: Print binary representations
- **Testing**: Try edge cases (0, 1, -1, MAX, MIN)
- **Optimization**: Bit operations are fast
- **Clarity**: Comment bit manipulation code
- **Practice**: Solve problems daily

**Remember**: Bitwise operations are powerful tools. With practice, they become intuitive and can significantly optimize your code!

---

**END OF GUIDE**