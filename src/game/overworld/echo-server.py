#!/usr/bin/python

"""
This implements a simple, non-validating server for the overworld
game.  It simply broadcasts everything that is receives, and sends the
entire message history to anyone joining.

This is mostly for testing purposes.
"""

repo_root = "../../../"
switchbox_dir = repo_root + 'src/switchbox/'

import sys
sys.path.append(switchbox_dir)
from client import *

class GameState:
    """"Maintains the state of 'overworld' game and allows it to be
        manipulated through 'message' strings.  """

class NaiveGameState(GameState):
    """This implementation is (very) naive for simplicity.  We need to
       maintain a data structure of the current game state.  So we
       currently do this by just keeping all the messages (in reverse
       order).  This will not scale, but it's almost no work.  So, I
       will use this until I have working code, then rewrite."""

    def __init__(self):
        self.messages = []

    def to_messages(self):
        """Returns a list of messages that can be used to replicate
           this state.  i.e:
             state2=GameState();
             [state2.update(m) for m in state1.to_messages()]
             assert(more_or_less_equal_p(state1, state2)) """
        return self.messages

    def is_message_valid(self, message):
        "Is the message valid in this state?  "
        return True

    def update(self, message):
        self.messages.append(message)
        return message


def announce(game_message):
    connection.broadcast(game_message)

def login_message_p(message):
    parsed = message.strip().split()
    print parsed
    return len(parsed) == 2 and parsed[0].lower() == '/login'

def sync_states(state, address):
    for message_str in state.to_messages():
        connection.send(address, message_str)

if __name__ == '__main__':
    global connection
    import sys
    prog_name, hostname, port = sys.argv
    port = int(port)
    connection = Connection(hostname, port)
    state = NaiveGameState()
    while True:
        message = connection.receive()
        if message_from(message) == connection.id: continue
        if not state.is_message_valid(message): continue
        if login_message_p(message_data(message)):
            sync_states(state, message_from(message))
        announce(state.update(message_data(message)))
