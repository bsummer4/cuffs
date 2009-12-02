#include "lib.h"

long long timeval_to_us(struct timeval *tv) {
  return 1000000LL * tv->tv_sec + tv->tv_usec; }

long long timeval_diff(struct timeval *start, struct timeval *end) {
  struct timeval result;
  result.tv_sec = end->tv_sec -start->tv_sec ;
  result.tv_usec = end->tv_usec - start->tv_usec;
  while (result.tv_usec < 0) { result.tv_usec += 1000000;
                               result.tv_sec--; }
  return (timeval_to_us(&result)); }
