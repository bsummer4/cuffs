#!/usr/bin/env tclsh8.5

set doc {
    Generates random messages for this game.  The messages should all be
    valid both syntactically and logically as this scripts maintains a
    simple version of the game state.  }

package require XOTcl
namespace import xotcl::*
source lib.tcl

proc instvars args {
    foreach var $args {
        uplevel [list my instvar $var] }}

shellproc sleep
Class create State

State instproc init {} {
       my set objects global
       my set id 0
       my set pid [pid] }

State instproc new_id {} {
    instvars id pid objects
    set name "$pid-obj-$id"
    incr id
    lappend objects $name
    return $name }

State instproc rand_id {} { random_choice [my set objects] }
State instproc del_id id {
    my instvar objects
    set choice [my rand_id]
    if {$choice != "global"} { set objects [lwithout $objects $choice] }
    return $choice }

proc rand_pos {} { random_from_range 0 600 }
proc msg {} { return "This is a message" }
proc query {x y} { list /query $x $y }
proc move {x y z} { list /move $x $y $z }
proc new {x y z a} { list /new $x $y $z $a }
proc delete arg { list /delete $arg }
proc rand_prop {} { random_choice {x y id type} }

State create s

proc gen_message {} {
    choice {
        new [s new_id] obj [rand_pos] [rand_pos]
        delete [s del_id [s rand_id]]
        move [s rand_id] [rand_pos] [rand_pos]
        query [s rand_id] [rand_prop]
        msg }}

loop {
    puts [gen_message]
    flush stdout
    sleep .1 }
