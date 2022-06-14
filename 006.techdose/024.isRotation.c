#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void
stringncat(char *dest, const char *src, size_t n)
{
    size_t dest_len = strlen(dest);
    size_t i;

    for (i = 0 ; i < n && src[i] != '\0' ; i++)
        dest[dest_len + i] = src[i];
    dest[dest_len + i] = '\0';

    return;
}

/* Change question into pattren search 
   and search b in the contacatinated string a */
bool checkRotation(char s1[], char s2[], int len1, int len2) 
{
        /* Comparing and checking string lengths */
        if (len1 != len2) {
            return false; //returning false if strings are unequal
        }

        char string[1000] = {'\0'};
        //storing concatenated string 
        stringncat(string, s1, len1);
        stringncat(string, s1, len1);

        int n = strlen(string);
        int m = len2;
        
        // checking if s2 is present in temp
        for(int i = 0; i < n-m; i++)
        {
            int flag = 1, j;
            for(j = 0; j < m-1; j++)
            {
                if(s2[j] != string[i+j])
                {
                    flag = 0;
                    break;
                }
            }
            
            if(flag == 1){
                return true;// return true if s2 is present in temp
            }
        }
        
        return false;
}

/* Driver code */
int main() {
    char a[] = "HELLO";
    char b[] = "LOHEL";
    if (checkRotation(a, b, strlen(a), strlen(b)))
        printf("Given Strings are rotations of each other.\r\n");
    else
        printf("Given Strings are not rotations of each other. \r\n");
    return 0;
}