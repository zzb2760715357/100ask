

#include <stdio.h>

typedef struct {
	int a;
	int b;
}T_AB;



int main(int argc, char **argv)
{
	char c;
	int a;
	T_AB tTest;
	int *p;
	int **pp;

	/* 1. */
	p = &a;
	printf("p = 0x%x, a'addr = 0x%x\n", p, &a);

	/* 2. */
	*p = 0x12345678;
	printf("a = 0x%x\n", a);

	/* 3. */
	p = &c;
	printf("p = 0x%x, c'addr = 0x%x\n", p, &c);

	/* 4. */
	*p = 'A';
	printf("c = %c\n", c);

	/* 5. */
	p = &tTest;
	printf("p = 0x%x, tTest'addr = 0x%x\n", p, &tTest);

	/* 6. */
	*p = &tTest;
	printf("tTest.a = 0x%x, tTest'addr = 0x%x\n", tTest.a, &tTest);

	/* 7. */
	pp = &p;
	printf("pp = 0x%x, p'addr = 0x%x\n", pp, &p);

	/* 8. */
	**pp = 0xABCD1234;
	printf("tTest.a = 0x%x\n", tTest.a);
	return 0;
	
}

