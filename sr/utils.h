#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <time.h>

/* LOG */
#define LOG_ERROR(M, ...)                                                      \
  fprintf(stderr, "[ERROR] [%s] " M "\n", __func__, ##__VA_ARGS__)
#define LOG_WARN(M, ...) fprintf(stderr, "[WARN] " M "\n", ##__VA_ARGS__)
#define LOG_INFO(M, ...) fprintf(stderr, "[INFO] " M "\n", ##__VA_ARGS__)

/* FATAL */
#define FATAL(M, ...) \
  fprintf(stderr, "[FATAL] " M "\n", ##__VA_ARGS__); \
  exit(-1);

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

int randi(int ub, int lb) { return rand() % lb + ub; }

float randf(float a, float b) {
  float random = ((float) rand()) / (float) RAND_MAX;
  float diff = b - a;
  float r = random * diff;
  return a + r;
}

int fltcmp(const double f1, const double f2) {
  if (fabs(f1 - f2) <= 0.0001) {
    return 0;
  } else if (f1 > f2) {
    return 1;
  } else {
    return -1;
  }
}

#endif
