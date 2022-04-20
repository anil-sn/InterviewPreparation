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
    Dont practice until you get it right
    practice until you can't get it worng
*/

int main() {
    char message[100], ch;
    int i, key;

    printf("Enter a message to encrypt: ");
    scanf("%s", message);

    printf("Enter key: ");
    scanf("%d", &key);

    for(i = 0; message[i] != '\0'; ++i) {
        ch = message[i];
        if(ch >= 'a' && ch <= 'z') {
            ch = ch + key;
            if(ch > 'z') {
                ch = ch - 'z' + 'a' - 1;
            }
            message[i] = ch;
        } else if(ch >= 'A' && ch <= 'Z') {
            ch = ch + key;
            if(ch > 'Z') {
                ch = ch - 'Z' + 'A' - 1;
            }
            message[i] = ch;
        }
    }
    printf("Encrypted message: %s\r\n", message);

    for(i = 0; message[i] != '\0'; ++i) {
        ch = message[i];
        if(ch >= 'a' && ch <= 'z') {
            ch = ch - key;
            if(ch < 'a') {
                ch = ch + 'z' - 'a' + 1;
            }
            message[i] = ch;
        } else if(ch >= 'A' && ch <= 'Z') {
            ch = ch - key;
            if(ch < 'A') {
                ch = ch + 'Z' - 'A' + 1;
            }
            message[i] = ch;
        }
    }
    printf("Decrypted message: %s\r\n", message);

    return 0;
}