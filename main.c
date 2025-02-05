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

	int x = 0b0100'0000'0000'0110'1100'0000'0000'0000;
//	int x = 0b0011'1111'1111'1110'0000'0000'0000'0000;
	printf("glibc = %.32f\n", *(float*) &x);

	int e = ((x & (0b0111'1111'1 << 23)) >> 23) - 127;

	u64 a[p] = {};
	int b = 22;
	for (int i=1; i <= p; ++i) {
		a[i-1] = powu64(5, i) * (bool)(x & (1 << b));
		--b;
//		printf("a[%i] = %lu\n", i, a[i-1]);
	}

	u64 q = 0;
	int r[p] = {};
	for(int i = p-1; i >= 0; --i) {
		printf("acc = %lu -> %lu, a[%i] = %lu, x = %lu\n",
			q, q + a[i], i, a[i], (q + a[i]) % 10);
		q += a[i];
		r[p-i-1] = q % 10;
		q /= 10;
	}
	for(int i=0; i < p; ++i)
		printf("r[%i] = %i\n", i, r[i]);
	printf("\n");

	// 0 : ctz % 4 == 0 && ctz == 0
	// 2 : ctz % 4 == 1
	// 4 : ctz % 4 == 2
	// 8 : ctz % 4 == 3
	// 6 : ctz % 4 == 0 && ctz > 0

	int pos = e > 0 ? e % 4 : 0;

	int s[26] = {};
	bool c = 0;
	for(int i=0; i < p; ++i) {
		r[i] <<= pos;
		s[i] = (r[i] + c) % 10;
		c = (r[i] >> 1) % 10 >= 5;
		printf("s[%i] = %i, carry = %i\n", i, s[i], c);
	}

//	printf("%li.", powu64(2, e));
	printf("<tbd>.");
	for(int i=p-1; i >= 0; --i)
		printf("%i", s[i]);
	printf("\n");

	return 0;
}
