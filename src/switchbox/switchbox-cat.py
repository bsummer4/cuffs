#!/usr/bin/python

import client
from client import Connection
from threading import Thread
import sys, os

# * TODO I'm using os._exit to exit
#   This is because one thread is blocking in an external library, and I
#   don't know of any other way to get python to kill it.

command, hostname, port = sys.argv
c = Connection(hostname, int(port))

nice_close = False

def send_data():
    global nice_close
    try:
        while True: c.broadcast(raw_input(""))
    except Exception as e:
        nice_close = True
        c.close()
        raise e

def get_data():
    try:
        while True:
            message = c.receive()
            line = client.copy_message_string(message)
            print line
    except Exception as e:
        if nice_close: return
        raise e

[Thread(target=x).start() for x in get_data, send_data]
