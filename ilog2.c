#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>

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
    while (i >= 0x10000) {
        result += 16;
        i >>= 16;
    }
    while (i >= 0x100) {
        result += 8;
        i >>= 8;
    }
    while (i >= 0x10) {
        result += 4;
        i >>= 4;
    }
    while (i >= 0x2) {
        result += 1;
        i >>= 1;
    }
    return result;
}

int ilog32(uint32_t v)
{
    return (31 - __builtin_clz(v | 1));
}

int __ilog2_u32(uint32_t n)
{
	return fls(n) - 1;
}

int ceil_ilog2(uint32_t x)
{
    uint32_t r, shift;

    //x--;
    r = (x > 0xFFFF) << 4;
    x >>= r;
    printf("[0xFFFF] x: 0x%x\n", x);

    shift = (x > 0xFF) << 3;
    x >>= shift;
    r |= shift;
    printf("[0xFF] x: 0x%x\n", x);

    shift = (x > 0xF) << 2;
    x >>= shift;
    r |= shift;
    printf("[0xF] x: 0x%x\n", x);

    shift = (x > 0x3) << 1;
    x >>= shift;
    printf("[0x3] x: 0x%x\n", x);

    return (r | shift | x > 1) + 1;

}

static inline int is_power_of_2(unsigned long n)
{
    return (n != 0 && ((n & (n - 1)) == 0));
}

/* Exponentially weighted moving average (EWMA) */
struct ewma {
    unsigned long internal;
    unsigned long factor;
    unsigned long weight;
};

/* This section contains universal functions designed for computing the EWMA.
 * It maintains a structure housing the EWMA parameters alongside a scaled
 * internal representation of the average value, aimed at minimizing rounding
 * errors. Initialization of the scaling factor and the exponential weight (also
 * known as the decay rate) is achieved through the ewma_init(). Direct access
 * to the structure is discouraged; instead, interaction should occur
 * exclusively via the designated helper functions.
 */

/**
 * ewma_init() - Initialize EWMA parameters
 * @avg: Average structure
 * @factor: Scaling factor for the internal representation of the value. The
 *     highest achievable average is determined by the formula
 *     ULONG_MAX / (factor * weight). To optimize performance, the scaling
 *     factor must be a power of 2.
 * @weight: Exponential weight, also known as the decay rate, dictates the rate
 *     at which older values' influence diminishes. For efficiency, the weight
 *     must be set to a power of 2.
 *
 * Initialize the EWMA parameters for a given struct ewma @avg.
 */
void ewma_init(struct ewma *avg, unsigned long factor, unsigned long weight)
{
    if (!is_power_of_2(weight) || !is_power_of_2(factor))
        assert(0 && "weight and factor have to be a power of two!");

    avg->weight = ilog2(weight);
    avg->factor = ilog2(factor);
    avg->internal = 0;
}

/**
 * ewma_add() - Exponentially weighted moving average (EWMA)
 * @avg: Average structure
 * @val: Current value
 *
 * Add a sample to the average.
 */
struct ewma *ewma_add(struct ewma *avg, unsigned long val)
{
    avg->internal = avg->internal
                        ? (((avg->internal << avg->weight) - avg->internal) +
                           (val << avg->factor)) >> avg->weight
                        : (val << avg->factor);
    return avg;
}

/**
 * ewma_read() - Get average value
 * @avg: Average structure
 *
 * Returns the average value held in @avg.
 */
unsigned long ewma_read(const struct ewma *avg)
{
    return avg->internal >> avg->factor;
}

int main()
{
	//size_t n = 0x0f456780;
    size_t n = 0x10000000;

	//for (int i = 1; i < 0xffffffff; i++) {
    //    if (ilog32(i) != ceil_ilog2(i)){
    //        printf("Test[%d]: %d, %ld, %d, %d\n", i, ilog2(i), ilog2_2(i), ilog32(i), ceil_ilog2(i));
    //        break;
    //    } else
    //        printf("Test[%d]: pass\n", i);
    //}

    //printf("Test[0x%zx]: %d, %d\n", n, ceil_ilog2(n), __ilog2_u32(n));
    printf("Test[%zu]: %d, %ld, %d, %d, %d\n", n, ilog2(n), ilog2_2(n), ilog32(n), ceil_ilog2(n), __ilog2_u32(n));

	return 0;
}
