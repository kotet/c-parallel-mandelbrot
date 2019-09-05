#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include "mandel.h"

#define THREAD_NUM (8)

struct mandel_calc_args
{
    uint8_t *dst;
    uint64_t width;
    uint64_t height;
    double top;
    double left;
    double bottom;
    double right;
    _Atomic uint64_t *index;
};

void *mandel_calc_thread(void *ptr)
{
    struct mandel_calc_args *args = (struct mandel_calc_args *)ptr;
    while (1)
    {
        uint64_t y = atomic_fetch_add(args->index, 1);
        if (y < args->height)
        {
            for (size_t x = 0; x < args->width; x++)
            {
                double re = args->left + interpolate(args->width, x) * (args->right - args->left);
                double im = args->bottom + interpolate(args->height, y) * (args->top - args->bottom);
                args->dst[y * args->width + x] = mandel_test(re, im);
            }
        }
        else
        {
            return (void *)0;
        }
    }
}

void mandel_calc(uint8_t *dst, uint64_t width, uint64_t height, double top, double left, double bottom, double right)
{
    int err;
    struct mandel_calc_args args[THREAD_NUM];
    pthread_t thread_id[THREAD_NUM];
    _Atomic uint64_t index;
    atomic_init(&index, 0);
    if (!atomic_is_lock_free(&index))
        printf("_Atomic uint64_t is not lock-free\n");
    for (size_t i = 0; i < THREAD_NUM; i++)
    {
        args[i] = (struct mandel_calc_args){dst, width, height, top, left, bottom, right, &index};
        err = pthread_create(&thread_id[i], NULL, mandel_calc_thread, (void *)&args[i]);
        if (err != 0)
            ERROR("can't create thread");
    }
    for (size_t i = 0; i < THREAD_NUM; i++)
    {
        err = pthread_join(thread_id[i], NULL);
        if (err != 0)
            ERROR("pthread_join failed");
    }
}