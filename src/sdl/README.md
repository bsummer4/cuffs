# SDL Tool
This is a wrapper around SDL and Tcl.  Basically this is a unix-style
program that maintains an image on the screen, changes the image based
on input from stdin, and displays all information regarding keyboard
and mouse movement to stdout.

## Basic Design
### Primitive Drawing Objects (prims)
All changes to the screen come from standard input.  The simplest case
for inputs is "prims".  These are primitive shape like a circle,
arrow, a single sound, or a single image.  You can create new
primitives by, for example, loading images and binding them to a
primitive name.  Primitives look like procedures and take arguments
like position, size, etc.

If you'd like, you can deal only with primitives like this:

    white
    circle {10 10} 10 {FF FF 00 33}
    circle {30 20} 10 {FF FF 00 33}
    circle {50 30} 10 {FF FF 00 33}
    flip
    white

However, this is a lot of IO (send a message every time the mouse
moves a pixel; sending a ton of messages every frame to maintain a
GUI, etc), and forces another tool to deal with the complexities of
clearing the screen, etc.  So there is a mechanism for managing a set
of objects, and leaving the actually drawing, clearing, and refreshing
to this tool.  We call these objects ents.

### Entities (ents)
Ents are basically just a named, mutable hunk of drawing code.  This
lets you to create multiple "instances" of a primitive, modify them,
delete then, and let the actual drawing be handled by tool.

    entloop
    ent c1 {circle {10 10} 10 {FF FF 00 33}}
    ent c2 {circle {30 20} 10 {FF FF 00 33}}
    ent c3 {circle {50 30} 10 {FF FF 00 33}}
    apply ;# This means we are done with a atomic set of inputs.
    c1 -pos {12 12}
    c3 -pos {48 28} -color {FF 00 00 88}
    ent c2 {circle {31 19} 11 {30 30 30 DD}}
    apply
    c3 del
    c2 -color {88 88 88 88}
    apply

### User Input
Basically, whenever there is a change in user input such as mouse
movement, key press, key release, etc., a message is printed:

    mouse 0 0
    mouse 0 1
    mouse 1 2
    mouse 2 3
    keys a
    keys a {right shift}
    mbuttons left
    keys
    mbuttons
    mouse 4 4

### Event Hooks: Dealing with UIs and Animations
Doing any serious work with this system requires an obscene amount
of IO and moves a lot of complexity to the tools we're talking to.
To deal with this we allow you to define TCL Procedures, bind
hunks of code to various events, and suppress certain types of
output:

    suppress mouse  ;# Don't send mouse events
    ent cursor {circle {0 0} 5 {0 0 0 0}}
    bind mouse mcursor {x y} { cursor -pos [list $x $y] }

    suppress keys   ;# Don't send keyboard events
    bind keys quitkey args {
        if {[memberof $args escape]} { puts done; exit }}

#### TODO MOAR
Work out how to do animations with time-based events.
