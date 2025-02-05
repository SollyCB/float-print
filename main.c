#include <stdio.h>
#include <math.h>
#include <stdint.h>

typedef uint64_t u64;

#define p 22

static u64 powu64(u64 x, u64 e)
{
	u64 r = 1;
	for (u64 i=0; i < e; ++i)
		r *= x;
	return r;
}

int main() {

	float f = 8.1782;
	int x = *(int*) &f;
//	int x = 0b0100'0000'0000'0110'1100'0000'0000'0000;
//	int x = 0b0100'0000'0000'1110'0000'0000'0000'0000;
	printf("glibc = %.6f\n", *(float*) &x);

	int exp = ((x & (0b0111'1111'1 << 23)) >> 23) - 127;
	printf("exp = %i\n", exp);

	u64 a[p] = {};
	int b = 22;
	for (int i=1; i <= p; ++i) {
		a[i-1] = powu64(5, i) * (bool)(x & (1 << b));
		--b;
	}

	u64 q = 0;
	int r[64] = {},s=-1,e=0;
	for(int i = p-1; i >= 0; --i,++e) {
		q += a[i];
		r[p-i-1] = q % 10;
		printf("r[%i] = %i\n", p-i-1, r[p-i-1]);
		q /= 10;
		if (r[p-i-1] && s == -1)
			s = p-i-1;
	}
	printf("\n");

	r[e++] = 1;

	int pos = e;
	bool c=0;
	for(int i=0; i < exp; ++i) {
		for(int j=s; j <= e; ++j) {
			int tmp = r[j];
			r[j] = (r[j] * 2 % 10) + c;
			c = tmp > 4;
			printf("r[%i] = %i\n", j, r[j]);
		}
		s += r[s] == 0 || r[s] == 5;
		e += r[e] > 0;
		printf("s = %i, e = %i\n", s, e);
	}

#if 1
	printf("round = %i\n", r[e-8]);
	c = r[e-8] > 4;
	for(int i=e-7; c; ++i) {
		int tmp = r[i];
		r[i] += c;
		c = tmp == 9;
	}
#endif

	printf("\nglibc = %.6f\n", *(float*) &x);

	printf("sol   = ");
	while(e-- >= pos)
		printf("%i", r[e]);
	printf(".");

	for(int i=e; i > e-6; --i)
		printf("%i", r[i]);
	printf("\n");

	return 0;
}
