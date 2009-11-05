#!/usr/bin/python

import sys, os
sys.path.append("../src/switchbox")
import client
from client import Connection
from threading import Thread

command, hostname, port, target = sys.argv
target = int(target)
c = Connection(hostname, int(port))

nice_close = False

def send_data():
    global nice_close
    try:
        while True:
            if target < 0: c.broadcast(raw_input(""))
            else: c.send(target, raw_input(""))
    except Exception as e:
        nice_close = True
        c.close()

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
