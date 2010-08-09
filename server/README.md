# Server
1. Fork off another program to handle physics
2. Open a socket
3. Accept connections

   Each connection gets an interp and a buffer.  The buffer is filled
   with input until [info complete $buf].  Then, the input is evaled in a
   sandboxed interp and the result (if we like the message) is sent to the
   physics handler.  If the physics handler decides that somthing changed,
   we receive a message, translate it and broadcast it to all of the clients.

