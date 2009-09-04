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

