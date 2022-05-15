#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// I solved this question on codewars.com

// check whether any odd bit of x is 1
// it should return 1 if odd bit equal to 1 exists, 0 otherwise

int any_odd(unsigned int x) {
  int cnt = 0;

  while (x) {
    if ((cnt++ & 1) && (x & 1))
      return 1;
    x >>= 1;
  }

  return 0;
}

int main ()
{
  unsigned int input = 0b01010101;

  printf("Odd bit %s set in input 0x%x\r\n",any_odd(input) ? "is" : "is not", input);
}
