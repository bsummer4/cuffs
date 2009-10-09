.. contents::

Design
======

Designed to be awesome.


Notations
---------

To describe data, we loosely use a syntax similar to the one used by
the Haskell language's type system.  Here is a brief overview:

  - *x :: t* -- x has type t
  - *x := 3* -- x is a constant with value 3
  - *[Foo]* -- A sequence of type Foo
  - *Foo:[Bar]* -- A sequence where the first element has type Foo and
     the rest of the elements have type Bar.
  - *Foo:Bar:[Zaz]* -- A sequence where the first element has type Foo
     and the remaining elements have type *Bar:[Foo]*.

Switchbox
---------

A program/library for handling routing between programs.

Summary
^^^^^^^

We do stuff with this C structure::

  typedef struct switchbox_message {
    int size;
    int routing_type;
    int from;
    int to;
    char data[];
  } SBMessage;


Not A Summary
^^^^^^^^^^^^^

First we define a simple message that is sent between two clients.

- Size :: Int
- Content :: [Byte]
- Message :: Size:Content (where the size of Content + Size := 'size)

Then, inside the message content, we define a routing protocol.

- TransmissionType :: Int
- Target :: Int
- switchboxMessage :: TransmissionType:Target:Content
- magic numbers for TransmissionType:

  - broadcast := 0
  - unicast := 1
  - multicast := 2
  - admin := 3
  - invalid-type := 4
  - invalid-target := 5


Switchbox Control Protocol
^^^^^^^^^^^^^^^^^^^^^^^^^^

How data for admining the switchbox is encoded/sent.

Messages with *TransmissionType t = admin* are handled by the
switchbox itself.

- groupID :: Int
- ActionType :: Int
- Message :: ActionId:Content
- Where the meaning of Content depends on ActionType

- Action -- "Define Group":

  - actionType := 0
  - Content :: actionType:groupID:[target]

  Defines a multicast group target to be a set of unicast targets.


- Action -- "Undefine Group":

  - actionType := 1
  - Content := actionType:GroupId:[]
  
  Removes a multicast Group.

General Game Design
-------------------

Game Messaging Language (GML)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The definition of the messages sent in the game. 

All Messages prepended by Timestamp
No slash = message display on console/screen

- /MAP [name]
- /ZONE ID OWNER CONTESTABLE BATTLE_ID
- /USER ID NAME ADDRESS [-1 if not logged in]
- /GROUP ADD ID USERID
- /GROUP DEL ID USERID
- /PARTY INVITE ID USERID
- /PARTY JOIN ID USERID
- /FIGHT PARTYID ZONE ID
- /START TURN PLAYERID
- /SHOOT ANGLE POWER WEAPONID
- /FORK GAMETYPE ZONEID SWITCHBOX_IP SWITCHBOX_PORT


Event Generator (interfaces): Chris
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Random Event Generator
- Assume: All generators can run on the same system – That way they can use system time to use same game time. 
- Game Client
- Random event generator w/ the protocol
- Implementation
- Writes client messages to stdout. 
- Versions

  - “cat” - Reads from a file
  - Generate Time stamps with blank messages, (maybe not in order?) 
  - Generate valid gameplay messages with timestamps
  - Follows State update messages above with no particular ordering

    - Print out a valid gameplay message sequence. 
    - What we expect to see in actual gameplay
    - “Simulator”

Event Synchronizer: 
^^^^^^^^^^^^^^^^^^^

- Orders messages correctly
- Hands messages in order to Event Interpreter
- Implementation

  - Sends messages to Event Interpreter
  - Reads from switchbox and handles synchronization
  - Handles Global Virtual Time

- Version

  - One that just sends message to Interpreter Immediately
  - One that implements the CMB algorithm.

Event Interpreter (interfaces): 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Print method → prints whatever it sees.
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

