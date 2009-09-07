#!/usr/bin/python

import sys
from client import Connection

c = Connection("localhost", 80021)
string = "lalala"

type = 'unicast'
if len(sys.argv) == 1: type = 'broadcast'
else: target = int(sys.argv[1])

while True:
    if type == 'broadcast':
        print "broadcasting: '%s'..."%(string),
        c.broadcast(string)
        result = c.receive()
        print 'got %s'%result
    else:
        print "sending: '%s' to %d..."%(string, target),
        c.send(target, string)
        print "sent"
