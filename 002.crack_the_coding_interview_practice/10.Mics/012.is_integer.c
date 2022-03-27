
#include <stdio.h>
#include <math.h>

#define isint(num)    ((int) num == num)
#define isint2(num)   (floor(num) == ceil(num))

int main()
{
	printf("3.00000 %s\n", isint(3.00000) ? "true" : "false");
	printf("1.00000 %s\n", isint(1.00000) ? "true" : "false");
	printf("5.42341 %s\n", isint(5.42341) ? "true" : "false");
	printf("1.00001 %s\n", isint(1.00001) ? "true" : "false");
	printf("9.99999 %s\n", isint(9.99999) ? "true" : "false");

	return 0;
}
