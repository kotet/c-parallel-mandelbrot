#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include "mandel.h"

void _ERROR(const char *filename, int line, const char *fmt, ...)
{
    fprintf(stderr, "%s:%d: FATAL ERROR: ", filename, line);

    va_list al;
    va_start(al, fmt);
    vfprintf(stderr, fmt, al);
    va_end(al);

    if (fmt[strlen(fmt) - 1] != '\n')
        putchar('\n');

    exit(1);
}

void output(char *filename, uint8_t *src, uint64_t width, uint64_t height)
{
    FILE *output = fopen(filename, "w");
    if (output == NULL)
        ERROR("fopen failed");
    fputs("P5\n", output);
    fprintf(output, "%ld %ld\n", width, height);
    fputs("255\n", output);
    fwrite(src, sizeof(int8_t), width * height, output);
    fclose(output);
}

double interpolate(uint64_t max, uint64_t x)
{
    return (double)x / max;
}

uint8_t mandel_test(double re, double im)
{
    double z_re = 0;
    double z_im = 0;
    for (size_t i = 0; i < 512; i++)
    {
        if (2.0 < z_re * z_re + z_im * z_im)
        {
            return 255 - (i/2);
        }
        double new_re = z_re * z_re - z_im * z_im;
        double new_im = 2 * z_re * z_im;
        z_re = new_re + re;
        z_im = new_im + im;
    }
    return 0;
}