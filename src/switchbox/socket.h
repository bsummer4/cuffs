#pragma once

#include <stdlib.h>

typedef byte char;
typedef Socket FILE*;

Socket open_socket(char* hostname, int port);
