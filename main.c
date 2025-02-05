#include <stdio.h>
#include <math.h>
#include <stdint.h>

typedef uint64_t u64;

u64 powu64(u64 x, u64 e)
{
	u64 r = 1;
	for (u64 i=0; i < e; ++i)
		r *= x;
	return r;
}

int ftos(float f, char *buf)
{
	int x = *(int*) &f;
	int exp = ((x & (0b0111'1111'1 << 23)) >> 23) - 127;

	constexpr int bc = 22;

	u64 a[bc] = {};
	int b = bc;
	for (int i=1; i <= bc; ++i) {
		a[i-1] = powu64(5, i) * (bool)(x & (1 << b));
		--b;
	}

	u64 q = 0;
	int r[64] = {},s=-1,e=0;
	for(int i = bc-1; i >= 0; --i,++e) {
		q += a[i];
		r[bc-i-1] = q % 10;
		q /= 10;
		if (r[bc-i-1] && s == -1)
			s = bc-i-1;
	}

	r[e++] = 1;

	int bp = 0;
	if (f < 0)
		buf[bp++] = '-';

	int pos = e;
	bool c=0;
	if (exp >= 0) {
		for(int i=0; i < exp; ++i) {
			for(int j=s; j <= e; ++j) {
				int tmp = r[j];
				r[j] = (r[j] * 2 % 10) + c;
				c = tmp > 4;
			}
			s += r[s] == 0 || r[s] == 5;
			e += r[e] > 0;
		}
		while(e-- >= pos)
			buf[bp++] = (char) r[e] + '0';
		buf[bp++] = '.';
		for(int i=e; i > e-6; --i)
			buf[bp++] = (char) r[i] + '0';
	} else {
		for(int i=0; i < -exp; ++i) {
			for(int j=s; j <= e; ++j) {
				if (r[j] % 2)
					r[j-1] += 5;
				r[j] /= 2;
			}
			s -= r[s-1] == 5;
			e -= r[e] == 0;
		}
		buf[bp++] = '0';
		buf[bp++] = '.';
		--pos;
		while(--pos > e)
			buf[bp++] = (char) r[pos] + '0';
		for(int i=e; i > e-6; --i)
			buf[bp++] = (char) r[i] + '0';
	}
	buf[bp] = 0;
	return bp;
}

int main() {

	float f = -980.9988;
	char buf[128];
	int sz = ftos(f, buf);

	printf("\n");
	printf("libc = %.6f\n", f);
	printf("sol  = %s\n", buf);
	printf("size = %i\n", sz);

	return 0;
}
