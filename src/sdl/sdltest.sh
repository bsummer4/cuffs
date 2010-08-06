#!/bin/sh

( for cmd in \
    "white\nflip" \
    "circle 40 {50 50} {255 0 0}\nflip" \
    "circle 30 {50 50} {0 255 0}\nflip" \
    "circle 20 {50 50} {0 0 255}\nflip"
  do /bin/echo -e $cmd; sleep 1
  done
) | ./entloop.tcl
