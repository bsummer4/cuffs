#!/usr/bin/python
from client import Connection
c = Connection("localhost", 80021)
while True:
    print "reading...",
    print "got: '%s'"%(c.receive())
