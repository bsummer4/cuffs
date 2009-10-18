======
Design
======

Designed to be awesome.

.. contents::

Switchbox
=========

A program for handling routing between programs.  There is a client
library for connecting to and creating messages to be sent through the
switchbox.  This library is available in python, c, and c++.

Basically, when you want to do things with the switchbox, you:

- Open a socket with 'open_connection' (from message.h)
- Create a message with one of the functions: (maybe 'string_to_message')
- Send the message with 'switchbox_send'
- Receive with 'switchbox_receive'

Messages have a binary header, and then a payload.  The switchbox does
different routing based on the information in the header.  There are
special "routing types" for actually controlling the switchbox.  These
let you create, and remove groups.

See 'src/switchbox/switchbox_client.h' for details.


General Game Design
===================

Game Messaging Language
-----------------------

The definition of the messages sent in the game.

All Messages prepended by Timestamp.  A slash before the first word
indicates that it is a command, otherwise it is a text message (sorta
like IRC).

Overworld Messages
^^^^^^^^^^^^^^^^^^^^^^^
- /LOGIN NAME
- /MAP [name]
- /ZONE ID OWNER CONTESTABLE BATTLE_ID
- /USER NAME ADDRESS [-1 if not logged in]
- /GROUP ADD ID USERID
- /GROUP DEL ID USERID
- /PARTY INVITE ID USERID
- /PARTY JOIN ID USERID
- /PARTY LEAVE ID USERID
- /FIGHT PARTYID ZONE ID
- /LAUNCHGAME GAMETYPE ZONEID SWITCHBOX_IP SWITCHBOX_PORT

Battle Messages
^^^^^^^^^^^^^^^^^^^^^^^
- /SHOOT USERID ANGLE POWER WEAPONID
- /MAP [name]
- /GAMESTART
- /GAMESTOP


Event Generator (interfaces)
----------------------------

- Random Event Generator
- Assume: All generators can run on the same system – That way they
  can use system time to use same game time.
- Game Client
- Random event generator w/ the protocol
- Implementation
- Writes client messages to stdout.
- Versions

  - "cat" - Reads from a file
  - Generate Time stamps with blank messages, (maybe not in order?)
  - Generate valid gameplay messages with timestamps
  - Follows State update messages above with no particular ordering

    - Print out a valid gameplay message sequence.
    - What we expect to see in actual gameplay
    - "Simulator"

Event Synchronizer
------------------

- Orders messages correctly
- Hands messages in order to Event Interpreter
- Implementation

  - Sends messages to Event Interpreter
  - Reads from switchbox and handles synchronization
  - Handles Global Virtual Time

- Version

  - One that just sends message to Interpreter Immediately
  - One that implements the CMB algorithm.


Event Interpreter (interfaces)
------------------------------

- Print method: prints whatever it sees.
- Games will use it.
- Design

  - handleMessage() function called from the Synchronizer.

- Versions

  - Simple Interpreter that just print to stdout in order received
  - Validator

    - Makes sure messages are well formed. (Syntactically correct)
    - Pretty prints to screen.

  - Real version – Calls proper API call for the given message.

State Object – Mutators and accessors for specific state of game.
