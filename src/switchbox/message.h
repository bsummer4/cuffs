#pragma once

#include "socket.h"

bool send_string(Socket, char* string);
bool send_array(Socket, int num_bytes, byte* array);
