Design
======

Designed to be awesome.

To describe data, we loosely use a syntax similar to the one used by
the Haskell language's type system.  Here is a brief overview:

  - *x :: t* -- x has type t
  - *x = 3* -- x is a constant with value 3
  - *[Foo]* -- A sequence of type Foo
  - *Foo:[Bar]* -- A sequence where the first element has type Foo and
     the rest of the elements have type Bar.
  - *Foo:Bar:[Zaz]* -- A sequence where the first element has type Foo
     and the remaining elements have type *Bar:[Foo]*.
  - *;;* -- denotes the beginning of a comment which ends at the next
        newline.


Switchbox
---------

A program/library for handling routing between programs.

First we define a simple message that is sent between two programs.

- size :: byte
- content :: [byte]
- message :: size:content
  ;; (where the content has a length of 'size' bytes)

Then, inside the message content, we define a routing protocol.

- target :: short    ;; (or whatever; we need to decide on *something*)
- data :: [byte]
- switchboxMessage :: target:data
- magic numbers for target:

  - broadcast = 0
  - switchbox = 1


Switchbox Control Protocol
^^^^^^^^^^^^^^^^^^^^^^^^^^

How data to be sent to the switchbox is encoded.

- data :: actionId:data
- actionId :: byte

- Action -- "Define Group":

  - actionId = 0
  - groupID :: target
  - data :: groupID:[target]
  
- Action -- "Undefine Group":

  - actionId = 1
  - data = actionId:target:[]
