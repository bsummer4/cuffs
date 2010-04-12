#!/usr/bin/env tclsh

source sdl.tcl

sdl_init

white
flip
exec sleep 1

circle 40 {50 50} {255 0 0}
flip
exec sleep 1

circle 30 {50 50} {0 255 0}
flip
exec sleep 1

circle 20 {50 50} {0 0 255}
flip
exec sleep 1
