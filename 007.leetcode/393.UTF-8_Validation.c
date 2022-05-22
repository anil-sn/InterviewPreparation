#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
Given an integer array data representing the data, return whether it is a valid UTF-8 encoding (i.e. it translates to a sequence of valid UTF-8 encoded characters).

A character in UTF8 can be from 1 to 4 bytes long, subjected to the following rules:

For a 1-byte character, the first bit is a 0, followed by its Unicode code.
For an n-bytes character, the first n bits are all one's, the n + 1 bit is 0, followed by n - 1 bytes with the most significant 2 bits being 10.
This is how the UTF-8 encoding would work:

     Number of Bytes   |        UTF-8 Octet Sequence
                       |              (binary)
   --------------------+-----------------------------------------
            1          |   0xxxxxxx
            2          |   110xxxxx 10xxxxxx
            3          |   1110xxxx 10xxxxxx 10xxxxxx
            4          |   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
x denotes a bit in the binary form of a byte that may be either 0 or 1.

Note: The input is an array of integers. Only the least significant 8 bits of each integer is used to store the data. This means each integer represents only 1 byte of data.
*/

static int  mask1      = 0x80, pattern1      = 0x00,
            mask2      = 0xe0, pattern2      = 0xc0,
            mask3      = 0xf0, pattern3      = 0xe0,
            mask4      = 0xf8, pattern4      = 0xf0,
            maskOther  = 0xc0, patternOther  = 0x80;

static int
validContByte (int c)
{
    return (c & maskOther) == patternOther;
}

static int
valid1 (int c)
{
    return (c & mask1) == pattern1;
}

static int
valid2 (int c)
{
    return (c & mask2) == pattern2;
}

static int
valid3 (int c)
{
    return (c & mask3) == pattern3;
}

static int
valid4 (int c)
{
    return (c & mask4) == pattern4;
}

bool
validUtf8 (int *data, int dataSize)
{
    int i, c, contBytes;

    for (i = 0; i < dataSize; i++) {
        c = data[i];

        if (valid1 (c)) {
            continue;
        } else if (valid2 (c)) {
            contBytes = 1;
        } else if (valid3 (c)) {
            contBytes = 2;
        } else if (valid4 (c)) {
            contBytes = 3;
        } else {
            return false;
        }

        while (contBytes-- > 0)
            if (++i >= dataSize || !validContByte (data[i])) {
                return false;
            }
    }

    return true;
}