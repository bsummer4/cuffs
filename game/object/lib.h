#pragma once
#include <sys/time.h>
#include "macro.h"

#ifdef __cplusplus
extern "C"{
#endif 
/// A bunch of assorted utility functions/procedures/macros
long long timeval_to_us(struct timeval *tv);
long long timeval_diff(struct timeval *start, struct timeval *end);
#ifdef __cplusplus
}
#endif 
