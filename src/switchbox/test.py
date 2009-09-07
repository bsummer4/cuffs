#/usr/bin/python
from client import Connection
c = Connection("localhost", 80021)
while True:
    c.send(0, "hi")
    assert(c.receive() == 'hi')
