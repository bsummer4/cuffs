# Switchbox
A program for routing messages between clients, and a set of client
programs for sending messages.

## Running
### The Server
    usage: switchbox [port]

If you pass a port of 0 or you don't pass a port, it will just use
some available port.  If you want to be able to easily start and stop
a switchbox, the you can use the wrapper-script 'switchbox.sh':

    usage: switchbox.sh {start|stop|restart|status} port

Example:

    $ switchbox.sh start 8888
    $ switchbox-connect localhost 8888
    -1 new_connection 0
    Hi!
    0 Hi!
    Bye...
    0 Bye...
    ^D
    $ switchbox.sh stop 8888

### The client programs
Right now, there are two client programs, switchbox-connect and
switchbox-cat.  switchbox-connect makes more information available,
but is slightly more complicated.

You can use switchbox-cat as a dead-simple chat client.  Have one
computer start the switchbox: 'switchbox.sh start 8000', and have two
other computers (Alice and Bob) start switchbox-cat:
'switchbox-cat $hostname 8000'.  Then anything Alice types will be
seen on Bob's screen and anything Bob types will be seen on Alice's
screen.

In addition to the message, switchbox-connect puts the address of the
client who sent it at the front.  It also sends special messages (from
-1) when people join and leave:

    $ switchbox-connect localhost 8000
    -1 new_connection 0 # This is our address
    -1 new_connection 1 # Some other asshole
    1 Hi everyone!  My address is 1!
    1 .....
    -1 lost_connection 1

Since you will want to write programs that use the switchbox instead
of just typing stuff into it, you will need to tie switchbox-connect
to your program.  'sixty-nine' is a utility to make this easy; It runs
two programs and ties their standard inputs and outputs together.
This is meant to be use like this:

    sixty-nine 'your-program args arrrrggs' 'switchbox-connect localhost 8000'

## Implementation
There is a library (defined in "switchbox-client.h") that was
originally designed to be used by external programs.  However it is
recommend that you just use "switchbox-connect" instead.

Basically, when you do things with the switchbox, you:

- Open a socket with 'open_connection' (from message.h)
- Create a message with one of the functions: (maybe
  'string_to_message')
- Send the message with 'switchbox_send'
- Receive with 'switchbox_receive'

Messages have a 16 byte binary header for addressing and size
information, and then a payload. The switchbox does different types
routing based on the information in the header (unicast, multicast and
broadcast). There are special "routing types" for actually controlling
the switchbox. These let you create and remove groups, etc.

The library headers file 'src/switchbox_client.h' and 'src/message.h'
are designed to be human readable. See them for more specific
information.
