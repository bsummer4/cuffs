Design
======

Designed to be awesome.


Switchbox
---------

A program/library for handling routing between programs.

First we define a simple message that is sent between two programs.

- size :: byte
- content :: [byte]
- message :: size:content
  (where the content has a length of 'size' bytes)

Then, inside the message content we define a routing protocol.

- broadcast = 0
- switchbox = 1
- target :: short
- data :: [byte]
- switchboxMessage :: target:data

Switchbox Control Protocol
^^^^^^^^^^^^^^^^^^^^^^^^^^

- *TODO* There needs to be a protocol for encoding messages to the
  switchbox itself for defining groups and such.
