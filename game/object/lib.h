#pragma once
#include <sys/time.h>
#include "macro.h"
/// A bunch of assorted utility functions/procedures/macros
long long timeval_to_us(struct timeval *tv);
long long timeval_diff(struct timeval *start, struct timeval *end);
