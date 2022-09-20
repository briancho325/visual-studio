#include <stdio.h>

int main()

{

	int* a, * b;



	a = (int*)malloc(sizeof(int));

	b = (int*)malloc(sizeof(int));

	*a = 15;

	*b = 25;

	*b = *a;

	a = b;

	printf("*a = %d : *b = %d\n", *a, *b);

}