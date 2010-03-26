#!/usr/bin/env tclsh8.5

source lib.tcl
set mousepos {0 0}
set size 0

proc circle args {
    global size mousepos 
    list_with $mousepos { list circle $size 255 0 0 $0 $1 }}

proc rectangle args {
    global size mousepos
    set hs [/ $size 2]
    list_with $mousepos {
        list rect 0 255 0 200 [- $0 $hs] [- $1 $hs] [+ $0 $hs] [+ $1 $hs] }}

loop {
    gets stdin line
    if {[eof stdin]} exit
    if {[lempty? $line]} continue
    bind {command args} [list [lindex $line 0] [lrange $line 1 end]]
    switch $command {
        space { puts white }
        mouse { set mousepos $args }
        1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 -
        9 { set size [* $command 10] }
        escape exit
        c { puts [circle] }
        r { puts [rectangle] }
        * continue }
    flush stdout }
