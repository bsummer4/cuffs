#!/usr/bin/python

from client import Connection
from threading import Thread
import sys, os

# TODO I'm using os._exit to exit
# This is because one thread is blocking in an external library, and I
# don't know of any other way to get python to kill it.

name = raw_input("What's your name? ")
hostname = raw_input("Which server to connect to? ")
port = input("Which port to use? ")
c = Connection(hostname, port)

def send_data():
    while True:
        try:
            line = raw_input("")
            if line == '/exit': raise Exception(line)
            output = "%s: %s"%(name, line)
            c.broadcast(output)
        except:
            c.close() # TODO why doesn't this cause the blocking
                      # read() in get_data() to fail?
            os._exit(0)

def get_data():
    while True:
        line = c.receive().strip()
        sender = line.split(':')[0]
        print line

[Thread(target=x).start() for x in get_data, send_data]
