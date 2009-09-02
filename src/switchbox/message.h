#pragma once

typedef byte char;
bool send_string(Socket, char* string);
bool send_array(Socket, int num_bytes, byte* array);
