#!/usr/bin/env tclsh8.5

source sdl.tcl

proc every {ms command} {
    uplevel #0 $command
    after $ms [list every $ms $command] }

set buf {}
proc interpret line {
    # Interprets a line that might be an incomplete input.
    global buf
    # TODO Why doesn't this work:
    # append buf "$line\n"
    set buf [concat $buf $line "\n"]
    if {[info complete $buf]} {
        puts $buf
        catch "uplevel #0 {$buf}"
        set buf {}}}

proc go {} {
    if {[eof stdin]} exit
    set line [gets stdin]
    puts "-- $line"
    interpret $line }

proc entloop {} {
    fileevent stdin readable go
    hai_init
    entloop_ }

entloop
