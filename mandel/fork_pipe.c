#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
#include <malloc.h>
#include <stdlib.h>
#include "mandel.h"

#define PROCESS_NUM (16)
#define PIPE_READ (0)
#define PIPE_WRITE (1)

#define min(a, b) ((a < b) ? a : b)

void mandel_calc_child(int fd, uint64_t width, uint64_t height, double top, double left, double bottom, double right, size_t id)
{
    uint64_t band_height = (height + PROCESS_NUM - 1) / PROCESS_NUM;
    uint64_t band_top = band_height * id;

    uint8_t *dst = malloc(width * height);

    for (size_t y = band_top; y < band_top + band_height && y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            double re = left + interpolate(width, x) * (right - left);
            double im = bottom + interpolate(height, y) * (top - bottom);
            dst[(y - band_top) * width + x] = mandel_test(re, im);
        }
    }
    size_t datasize = sizeof(uint8_t) * (min(height, band_top + band_height) - band_top) * width;
    write(fd, dst, datasize);
    free(dst);
}

void mandel_calc(uint8_t *dst, uint64_t width, uint64_t height, double top, double left, double bottom, double right)
{
    pid_t pids[PROCESS_NUM];
    int pipe_read[PROCESS_NUM];
    int pipe_write[PROCESS_NUM];

    uint64_t band_height = (height + PROCESS_NUM - 1) / PROCESS_NUM;

    for (size_t i = 0; i < PROCESS_NUM; i++)
    {
        int pipefd[2];
        if (pipe(pipefd) == 0)
        {
            pipe_read[i] = pipefd[PIPE_READ];
            pipe_write[i] = pipefd[PIPE_WRITE];
        }
    }

    for (size_t i = 0; i < PROCESS_NUM; i++)
    {
        pid_t pid;
        if ((pid = fork()) < 0)
        {
            ERROR("fork failed");
        }
        else if (pid == 0) // child
        {
            close(pipe_read[i]);
            mandel_calc_child(pipe_write[i], width, height, top, left, bottom, right, i);
            close(pipe_write[i]);
            exit(0);
        }
        else // parent
        {
            pids[i] = pid;
            close(pipe_write[i]);
        }
    }
    for (size_t i = 0; i < PROCESS_NUM; i++)
    {
        uint64_t band_top = band_height * i;
        size_t datasize = sizeof(uint8_t) * (min(height, band_top + band_height) - band_top) * width;
        read(pipe_read[i], &dst[band_top * width], datasize);
        close(pipe_read[i]);
    }

    for (size_t i = 0; i < PROCESS_NUM; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
}