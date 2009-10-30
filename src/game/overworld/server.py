#!/usr/bin/python

"""

This implements a game server for the overworld game.  Right now it
only handles user logins and the current map.

TODO
====

- Handle party messages correctly
- Write code for forking off another game, and accepting results from
  it.
- more...

"""


repo_root = "../../../"
switchbox_dir = repo_root + 'src/switchbox/'

import sys
sys.path.append(switchbox_dir)
from client import *
from lib import *
import overworld_map, random

class GameState:
    """"Maintains the state of 'overworld' game and allows it to be
        manipulated through 'message' strings.  """

    def __init__(self):
        self.users = {}
        self.map = None
        self.logins = {}
        self.groups = {}
        self.zone_states = {}

    def init_zones(self):
        """Arbitrarily sets up all the zones states.  """
        for zone in self.map['zones']:
            zone_id = zone[0]
            print 'init', zone_id, (zone)
            self.zone_states[zone_id] = {
                'owner': random.choice(range(1)),
                'contestable': random.choice((True, False)),
                'battle-id': -1}
            print self.zone_states

    def to_messages(self):
        """Returns a list of messages that can be used to replicate
           this state.  i.e:
             state2=GameState();
             [state2.update(m) for m in state1.to_messages()]
             assert(more_or_less_equal(state1, state2)) """
        return (self.map_messages() +
                [self.user_message(username)
                 for username in self.users])

    def zone_messages(self): return [self.zone_message(zone[0])
                                     for zone in self.map['zones']]
    def zone_message(self, zone_id):
        print 'zone_id', zone_id
        print self.zone_states
        zstate = self.zone_states[zone_id]
        print zstate
        return '/zone %s %s %s %d'%(
            zone_id,
            zstate['owner'],
            zstate['contestable'],
            zstate['battle-id'])

    def map_messages(self):
        return ['/map %s'%self.map['name']] + self.zone_messages()

    def user_message(self, username):
        address = -1
        print self.users[username]
        if self.users[username]['logged-in?']:
            address = self.users[username]['address']
        return '/user %s %d'%(username, address)

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

        if (map_message_p(parsed) or
            user_message_p(parsed)):
            # No one is allowed to change these things
            return None

        if party_message_p(parsed):
            return text

        return None

def user_message_p(parsed):
    return len(parsed) == 3 and parse[0].lower() == '/user'

def party_message_p(parsed):
    return (len(parsed) == 4 and parsed[0] == '/party' and
            parsed[1] in ('join', 'leave', 'invite'))

def text_message_p(parsed): return not commandp(parsed)
def commandp(parsed): return len(parsed) > 0 and parsed[0][0] == '/'
def announce(game_message):
    print 'announce>'
    print ' ', game_message
    if game_message: connection.broadcast(game_message)

def map_message_p(parsed):
    return len(parsed) == 2 and parsed[0].lower() == '/map'

def login_message_p(parsed):
    return len(parsed) == 2 and parsed[0].lower() == '/login'

def sync_states(state, address):
    print 'sync_states>'
    for message_str in state.to_messages():
        print '  ->', message_str
        connection.send(address, message_str)

if __name__ == '__main__':
    def from_us(message): return message_from(message) == connection.id
    global connection
    import sys
    prog_name, hostname, port, map_name = sys.argv
    port = int(port)
    connection = Connection(hostname, port)
    state = GameState()
    state.map = overworld_map.load_map(map_name + '.map')
    state.init_zones()
    while True:
        message = connection.receive()
        if from_us(message): continue
        parsed = copy_message_string(message).strip().split()
        print 'message(', message_from(message), parsed, ')'
        announce(state.update(message))
        if login_message_p(parsed):
            sync_states(state, message_from(message))
