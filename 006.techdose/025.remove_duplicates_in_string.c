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
1: Initialize:
    str  =  "test string"           // input string 
    ip_ind =  0                     // index to  keep track of location of next character in input string 
    res_ind  =  0                   // index to  keep track of location of next character in the resultant string 
    bin_hash[0..255] = {0,0, ….}    // Binary hash to see if character is already processed or not 
2: Do following for each character *(str + ip_ind) in input string:
              (a) if bin_hash is not set for *(str + ip_ind) then
                   // if program sees the character *(str + ip_ind) first time
                         (i)  Set bin_hash for *(str + ip_ind)
                         (ii)  Move *(str  + ip_ind) to the resultant string.
                              This is done in-place.
                         (iii) res_ind++
              (b) ip_ind++
  String obtained after this step is "the stringing" 
3: Remove extra characters at the end of the resultant string.
   String obtained after this step is "te string" 
*/

# define NO_OF_CHARS 256
 
/* Function removes duplicate characters from the string
   This function work in-place and fills null characters
   in the extra space left */
char *removeDups(char *str)
{
  bool bin_hash[NO_OF_CHARS] = {0};
  int ip_ind = 0, res_ind = 0;
  u_int8_t temp;   
 
  /* In place removal of duplicate characters*/
  while (*(str + ip_ind))
  {
    temp = *(str + ip_ind);
    if (bin_hash[temp] == 0)
    {
        bin_hash[temp] = 1;
        *(str + res_ind) = *(str + ip_ind);
        res_ind++;
    }
    ip_ind++;
  }     
 
  /* After above step string is string Removing extra iittg after string*/
  *(str+res_ind) = '\0';  
 
  return str;
}
 
/* Driver program to test removeDups */
int main()
{
    char str[] = "geeksforgeeks";
    printf("%s \r\n", removeDups(str));
    return 0;
}