#include "mandel/mandel.h"

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define width (10000)
#define height (10000)
#define top (2.0)
#define left (-2.0)
#define bottom (-2.0)
#define right (2.0)

int main(int argc, char const *argv[])
{
    uint8_t *dst = calloc(width * height, sizeof(int8_t));
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    mandel_calc(dst, width, height, top, left, bottom, right);
    gettimeofday(&end, NULL);

    double diff_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("%f[s]\n", diff_time);

    char filename[256];
    if (0 < argc)
    {
        strcpy(filename, argv[0]);
    }
    else
    {
        strcpy(filename, "test");
    }
    strcat(filename, ".pgm");

    output(filename, dst, width, height);

    free(dst);

    return 0;
}
