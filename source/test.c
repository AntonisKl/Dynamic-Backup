#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int * p, *p1, *p2, *p3;
	p2 = malloc(sizeof(int));
	p3 = malloc(sizeof(int));
	(*p2) = 125;
	(*p3) = 3;	
	p1 = p2;
	p = p1;
	printf("%d\n", *p);
	p1 = p3;
	printf("%d\n", *p);
	
}
