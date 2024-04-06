#include <stdio.h>
#include <stdlib.h>

int ilog2(int i)
{
    int log = -1;
    while (i) {
        i >>= 1;
        log++;
    }
    return log;
}

static size_t ilog2_2(size_t i)
{
    size_t result = 0;
    while (i >= 0xffff) {
        result += 16;
        i >>= 16;
    }
    while (i >= 0xff) {
        result += 8;
        i >>= 8;
    }
    while (i >= 0xf) {
        result += 4;
        i >>= 4;
    }
    while (i >= 2) {
        result += 1;
        i >>= 1;
    }
    return result;
}

int main()
{
	size_t n = 1024;

	printf("%d, %ld\n", ilog2(n), ilog2_2(n));

	return 0;
}
