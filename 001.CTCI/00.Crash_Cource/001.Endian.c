#include <stdio.h>
#include <stdlib.h>

/* function to show bytes in memory, from location start to start+n*/
void show_mem_rep(char *start, int n) {
    int i;

    for (i = 0; i < n; i++) {
        printf("[%d:%p] %.2x,  ", i, &start[i], start[i]);
    }

    printf("\n");
}

/*Main function to call above function for 0x01234567*/
int main() {

    int i = 0x01234567;
    unsigned int j = 1;
    char *c = (char *)&j;

    if (*c) {
        printf("I am Little Endian Machine \r\n");
    } else {
        printf("I am Big Endian Machine \r\n");
    }

    printf("int i = 0x01234567;\r\n");
    show_mem_rep((char *)&i, sizeof(i));

    unsigned char arr[2] = {0x01, 0x00};
    printf("unsigned char arr[2] = {0x01, 0x00};\r\n");

    unsigned short int x = *(unsigned short int *) arr;
    printf("Printing 'unsigned char arr[2] = {0x01, 0x00};' has uint16_t : %d\r\n", x);

    return 0;
}