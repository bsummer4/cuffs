#!/usr/bin/python

"""
This implements a game server for the overworld game.  Right now it
only handles user logins and the current map.
"""

repo_root = "../../../"
switchbox_dir = repo_root + 'src/switchbox/'

import sys
sys.path.append(switchbox_dir)
from client import *

class GameState:
    """"Maintains the state of 'overworld' game and allows it to be
        manipulated through 'message' strings.  """

    def __init__(self):
        self.users = {}
        self.map = None
        self.logins = {}

    def to_messages(self):
        """Returns a list of messages that can be used to replicate
           this state.  i.e:
             state2=GameState();
             [state2.update(m) for m in state1.to_messages()]
             assert(more_or_less_equal_p(state1, state2)) """
        return ([self.map_message()] +
                [self.user_message(username)
                 for username in self.users])

    def map_message(self): return '/map %s'%self.map
    def user_message(self, username):
        address = -1
        print self.users[username]
        if self.users[username]['logged-in?']:
            address = self.users[username]['address']
        return '/user %s %d'%(username, address)

    def is_message_valid(self, message):
        "Is the message valid in this state?  "
        return True

    def get_user(self, username):
        if username not in self.users:
            self.users[username] = {'logged-in?': False,
                                   'address': None}
        return self.users[username]

    def logout(self, user):
        address = user['address']
        if address in self.logins: del self.logins[address]
        user['logged-in?'] = False
        return user

    def login(self, username, address):
        user = self.get_user(username)
        user['address'] = address
        user['name'] = username
        user['logged-in?'] = True
        self.logins[address] = user
        return user

    def update(self, message):
        text = copy_message_string(message)
        client_address = message_from(message)
        parsed = text.strip().split()

        if login_message_p(parsed):
            username = parsed[1]
            if client_address in self.logins:
                self.logout(self.logins[client_address])
                self.logout(self.get_user(username))
            self.login(username, client_address)
            print username, 'logged in'
            return self.user_message(username)

        # Don't accept messages from non-loged in users
        if client_address not in self.logins: return None
        user = self.logins[client_address]

        print 'Accepting messages from', user['name']

        if text_message_p(parsed): return text

        if map_message_p(parsed):
            self.map = parsed[1]
            return text

        return None

def text_message_p(parsed): return not commandp(parsed)
def commandp(parsed): return len(parsed) > 0 and parsed[0][0] == '/'
def announce(game_message):
    print 'message:', game_message, not game_message
    if game_message: connection.broadcast(game_message)

def map_message_p(parsed):
    return len(parsed) == 2 and parsed[0].lower() == '/map'

def login_message_p(parsed):
    return len(parsed) == 2 and parsed[0].lower() == '/login'

def sync_states(state, address):
    for message_str in state.to_messages():
        print '->', message_str
        connection.send(address, message_str)

if __name__ == '__main__':
    global connection
    import sys
    prog_name, hostname, port = sys.argv
    port = int(port)
    connection = Connection(hostname, port)
    state = GameState()
    while True:
        message = connection.receive()
        parsed = copy_message_string(message).strip().split()
        if message_from(message) == connection.id: continue
        if not state.is_message_valid(message): continue
        announce(state.update(message))
        if login_message_p(parsed):
            sync_states(state, message_from(message))
