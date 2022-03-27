#include<stdio.h>
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type)
int main() {
    double x;
    printf("sizeof [double x] %ld\r\n", my_sizeof(x));
    return 0;
}
