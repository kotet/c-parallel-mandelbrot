#include <stdint.h>
#include <stddef.h>
#include "mandel.h"

void mandel_calc(uint8_t *dst, uint64_t width, uint64_t height, double top, double left, double bottom, double right)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            double re = left + interpolate(width, x) * (right - left);
            double im = bottom + interpolate(height, y) * (top - bottom);
            dst[y * width + x] = mandel_test(re, im);
        }
    }
}