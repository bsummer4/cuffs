
typedef byte char;
bool send_string(Socket, char* string);
bool send_array(Socket, int num_bytes, byte* array);
Socket open_socket(char* hostname, int port);

