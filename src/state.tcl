# Code to Manage the state of a game.
# We expect all input to come in through the handle_msg function; output
# is printed to stdout.

# It's probably best to use this as a standalone program or in a safe
# interpreter like this:
#
#     set state [interp create -safe]
#     $state invokehidden source {state.tcl}
#     interp transfer {} stdout $state
#     while 1 { $state eval [gets stdin] }


# Utillity Functions
proc prog1 args { lindex $args 0 }

set gensym_count 0
proc gensym base {
    global gensym_count
    return "g${base}_[incr gensym_count]" }

proc lexists {item list} { expr -1 != [lsearch $list $item] }
proc lremove {list string} {
  set result [list]
  foreach item $list {
    if {![string equal $item $string]} { lappend result $item }}
  return $result }

proc now {} { clock clicks -milliseconds }


# Messages that we accept
set globals [list]

proc gameover {} exit

proc getprop {id key} { global $id; dict get [set $id] $key }
proc setprop {id key value} { global $id; dict set $id $key $value }

proc msg args { puts "msg $args" }
proc query {id property} { msg "$id.$property = [getprop $id $property]" }

proc delete id {
  global $id
  global globals
  if {[lexists $id $globals]} {
    set globals [lremove $globals $id]
    unset $id }}

proc new {id render_proc args} { 
    global $id
    global globals
    delete $id
    lappend globals $id
    set $id [eval "dict create render_proc $render_proc created [now] $args"]}

proc map name { setprop global map $name }
proc explode {x y radius} {
    set id [gensym explosion]
    new [gensym explosion] explosion x $x y $y radius $radius }

# Default objects
set global [new global global wind 0 gravity 0 map {}]


# Rendering routine
proc render {} {
  global globals
  foreach id [lremove $globals global] {
    global $id render_proc
    dict with $id { puts "draw-center $render_proc $x $y" }}
  puts "draw global 0 0" }
