#!/usr/bin/python

"""

This implements a game server for the overworld game.  Right now it
only handles user logins and the current map.

TODO
====

- Handle disconnects
- Handle repeat logins better
- Write code for forking off another game, and accepting results from
  it.
- /fight
- Automated testing

"""


repo_root = "../../../"
switchbox_dir = repo_root + 'src/switchbox/'

import sys
sys.path.append(switchbox_dir)
from client import *
from lib import *
import overworld_map, random
from pprint import pprint

def launch_battle(ref_program, teams):
    """launch_battle("random", (['fred', 'alex'], ['diana', 'abby']))"""
    # start battle switchbox
    switchbox_dir + 'switchbox'

class GameState:
    """"Maintains the state of 'overworld' game and allows it to be
        manipulated through 'message' strings.  """

    def __init__(self):
        self.map = None
        self.users = {}   # username -> user
        self.logins = {}  # address -> user
        self.parties = {} # username->other-users-in-party
        self.invites = {} # username->other-users-invited-but-not-yet-in-party
        self.zone_states = {}

    def shit(self):
        pprint({'map': self.map,
                'users': self.users,
                'logins': self.logins,
                'parties': self.parties,
                'invites': self.invites,
                'zone_states': self.zone_states})

    def update(self, message):
        text = copy_message_string(message)
        client_address = message_from(message)
        parsed = text.strip().split()
        sender = self.find_user(client_address)
        if sender: sender = sender['name']
        else: sender = client_address

        for (predicate, handler) in [(login_message_p, self.handle_login),
                                     (text_message_p, self.handle_text),
                                     (map_message_p, none),
                                     (user_message_p, none),
                                     (party_message_p, self.handle_party),
                                     (fight_message_p, self.handle_fight)]:
            print '\\\\', predicate, predicate(parsed)
            if predicate(parsed):
                return handler(parsed, sender, text)
        return None

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


    # Update Message Generation
    # =========================

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
        if self.map == None: raise Exception("No map set.  ")
        return ['/map %s'%self.map['name']] + self.zone_messages()

    def user_message(self, username):
        address = -1
        print self.users[username]
        if self.users[username]['logged-in?']:
            address = self.users[username]['address']
        return '/user %s %d'%(username, address)


    # Utility
    # =======

    def logout(self, user):
        address = user['address']
        if address in self.logins: del self.logins[address]
        user['logged-in?'] = False
        return user

    def login(self, username, address):
        print "####", 'login', username, address
        user = self.get_user(username)
        user['address'] = address
        user['name'] = username
        user['logged-in?'] = True
        print user
        self.logins[address] = user
        print self.logins
        return user

    def find_user(self, address):
        if address not in self.logins: return None
        return self.logins[address]

    def get_user(self, username):
        if username not in self.users:
            self.users[username] = {'logged-in?': False,
                                   'address': None}
        return self.users[username]

    def get_party(self, owner):
        if owner not in self.parties: self.parties[owner] = []
        return self.parties[owner]

    def get_invites(self, owner):
        if owner not in self.invites: self.invites[owner] = []
        return self.invites[owner]

    # Message Handlers
    # ================

    def handle_fight (self, parsed, sender, text):
        if type(sender) == int: return None
        print '#### handle_fight'

    def handle_party (self, parsed, sender, text):
        print text

        print '#### handle_party'
        if type(sender) == int: return None
        ignore, command, party_owner, user = parsed
        if party_owner == user: return None
        party = self.get_party(party_owner)
        invites = self.get_invites(party_owner)

        if command == 'join':
            if (sender != user or
                user in party or
                user not in invites): return None
            party.append(user)
            invites.remove(user)

        if command == 'leave':
            if (sender != user and sender != party): return None
            if (user in party):
                party.remove(user);
                return text
            if (user in invites):
                invites.remove(user);
                return text
            return None

        if command == 'invite':
            if (sender != party_owner or
                user in party or
                user in self.invites[party_owner]): return None
            invites.append(user)

        return text

    def handle_text(self, parsed, sender, text):
        print '#### handle_text'
        if type(sender) == int: return None
        if sender: return text

    def handle_login(self, parsed, client_address, text):
        print '#### handle_login'
        if type(client_address) != int:
            return '/error you are already logged in!'
        command, username = parsed
        if client_address in self.logins:
            self.logout(self.logins[client_address])
            self.logout(self.get_user(username))
        self.login(username, client_address)
        print username, 'logged in'
        return self.user_message(username)

# Text Message Formatting
# =======================

def fight_message_p(parsed):
    return (len(parsed) == 4 and
            parsed[0] == 'fight')

def user_message_p(parsed):
    return len(parsed) == 3 and parse[0].lower() == '/user'

def party_message_p(parsed):
    return (len(parsed) == 4 and parsed[0] == '/party' and
            parsed[1] in ('join', 'leave', 'invite'))

def text_message_p(parsed): return not commandp(parsed)
def commandp(parsed): return len(parsed) > 0 and parsed[0][0] == '/'
def map_message_p(parsed):
    return len(parsed) == 2 and parsed[0].lower() == '/map'

def login_message_p(parsed):
    return len(parsed) == 2 and parsed[0].lower() == '/login'

# Utility
# =======

def announce(game_message):
    print 'announce>'
    print ' ', game_message
    if game_message: connection.broadcast(game_message)

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
            print
            print "|||| shitting"
            state.shit()
            print "|||| shitting"
            print
