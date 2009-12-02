#!/usr/bin/python

"""
Generates random messages for this game.  The messages should all be
valid both syntactically and logically as this scripts maintains a
simple version of the game state.
"""

from time import sleep
from sys import stdout
import random, os

class state():
    def __init__(self):
        self.objects = set(["global"])
        self.id = 0
        self.pid = os.getpid()
    def new_id(self):
        self.id += 1
        name = "%d-obj-%d"%(self.pid, self.id-1)
        self.objects.add(name)
        return name
    def rand_id(self): return random.choice(list(self.objects))
    def del_id(self, id):
        choice = self.rand_id()
        if choice != "global":
            self.objects.remove(choice)
        return choice

def rand_pos(): return random.randint(0, 600)
def msg(): return "This is a message"
def query(*args): return "/query %s %s"%args
def move(*args): return "/move %s %d %d"%args
def new(*args): return "/new %s %s %d %d"%args
def delete(*args): return "/delete %s"%args
def rand_prop(*args): return random.choice(["x", "y", "id", "type"])

def gen_message(format):
    if type(format) == tuple:
        function = format[0]
        args = map(gen_message, format[1:])
        return function(*args)

    if type(format) == list:
        return gen_message(random.choice(format))

    return format

if __name__ == '__main__':
    s = state()
    count = 1
    format = [(new, (s.new_id,), 'obj', (rand_pos,), (rand_pos,)),
              (delete, (s.del_id, (s.rand_id,))),
              (move, (s.rand_id,), (rand_pos,), (rand_pos,)),
              (query, (s.rand_id,), (rand_prop,)),
              (msg,)]

    while True:
        print gen_message(format)
        stdout.flush()
        count += 1
        sleep(.006)
