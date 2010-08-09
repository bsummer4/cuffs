#!/usr/bin/env tclsh8.5

source sdl.tcl

proc every {ms command} {
    uplevel #0 $command
    after $ms [list every $ms $command] }

set buf {}

# Interprets a line that might be an incomplete input.
proc interpret line {
	append ::buf $line "\n"
	if {[info complete $::buf]} {
		puts [string trim $::buf]
		catch "uplevel #0 {$::buf}"
		set ::buf {}}}

proc go {} {
    if {[eof stdin]} exit
    set line [gets stdin]
    interpret $line }

proc entloop {} {
    fileevent stdin readable go
    hai_init
    entloop_ }

entloop
