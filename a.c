#include <stdio.h>

int plus(int *a, int b) {
	printf("%d", *a);
	return *a + b;
}

int main() {
	int a = 1, b = 2;
	printf("%p  %p\n", &a - 0x1, &b);
	int r = plus(&a - 0x1, b);
	printf("%d", r);
	return 0;
}
