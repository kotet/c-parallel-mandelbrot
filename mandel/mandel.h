#include <stdint.h>

#define ERROR(...) _ERROR(__FILE__, __LINE__, __VA_ARGS__)
void _ERROR(const char *filename, int line, const char *fmt, ...);
void output(char *filename, uint8_t *src, uint64_t width, uint64_t height);
double interpolate(uint64_t max, uint64_t x);
uint8_t mandel_test(double re, double im);

void mandel_calc(uint8_t *dst, uint64_t width, uint64_t height, double top, double left, double bottom, double right);