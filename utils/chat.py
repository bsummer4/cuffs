#!/usr/bin/python

import sys, os
sys.path.append("../src/switchbox")

import client
from client import Connection
from threading import Thread

# * TODO I'm using os._exit to exit
#   This is because one thread is blocking in an external library, and I
#   don't know of any other way to get python to kill it.

name = raw_input("What's your name? ")
hostname = raw_input("Which server to connect to? ")
port = input("Which port to use? ")
c = Connection(hostname, port)
nice_close = False

def send_data():
    global nice_close
    try:
        while True:
            line = raw_input("")
            if line == '/exit': raise Exception(line)
            output = "%s: %s"%(name, line)
            c.broadcast(output)
    except Exception as e:
        nice_close = True
        c.close()
        if str(e) == '/exit': return
        raise e

def get_data():
    try:
        while True:
            message = c.receive()
            line = client.copy_message_string(message)
            sender = line.split(':')[0]
            print line
    except Exception as e:
        if nice_close: return
        raise e

[Thread(target=x).start() for x in get_data, send_data]
