#!/usr/bin/python

import sys, random
from functools import partial

mousepos = (0, 0)
size = 0

def mouse(args):
    global mousepos
    mousepos = tuple(map(int, args))

def setsize(size_, ignore):
    global size
    size = size_

def die(*ignore): exit(0)

def circle(ignore):
    return "circle %d 255 0 0 %d %d" % (size, mousepos[0], mousepos[1])

def rectangle(ignore):
    return "rect 0 255 0 200 %d %d %d %d" % (
        mousepos[0] - size/2, mousepos[1] - size/2,
        mousepos[0] + size/2, mousepos[1] + size/2)

controls = {
    "space": lambda x: "white",
    "mouse": mouse,
    "1": partial(setsize, 10),
    "2": partial(setsize, 20),
    "3": partial(setsize, 30),
    "4": partial(setsize, 40),
    "5": partial(setsize, 50),
    "6": partial(setsize, 60),
    "7": partial(setsize, 70),
    "8": partial(setsize, 80),
    "9": partial(setsize, 90),
    "escape": die,
    "c": circle,
    "r": rectangle}

while True:
    input = sys.stdin.readline()
    if not input: exit(0) # EOF
    input = input.strip()
    if not input: continue
    inputs = input.split()
    command, args = inputs[0], inputs[1:]
    if command not in controls: continue
    output = controls[command](args)
    if output: print output
    sys.stdout.flush()
