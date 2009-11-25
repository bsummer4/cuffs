#!/usr/bin/python

"""
# Usage

./ref team-A-names team-B-players

example: ./ref fred,alex ben,james

The first group of names belong to team1 and the second group
belong to team 2.

Ref will:
  * Waits for all clients to "/identify $name" themselves
  * Starts Game '/start'
  * Place all the player objects ("0 /new player-$name player $x $y")
"""

import sys, os
sys.path.append("../../../switchbox") # where 'client' is
import client
from client import Connection
import random

def randPos(): return random.randint(0, 800)

def place_player(player):
    return '/new player-%s player %d %d'%(player, randPos(), 0)

def go(player_placements):
    print '/map big'
    print '/start'
    for placement_message in player_placements:
        print 0, placement_message

def wait_for_players(team_a, team_b):
    result = []
    while True:
        if len(team_a) + len(team_b) == 0: break
        message = raw_input()
        if not message.startswith('/identify '):
            raise Exception("bad /identify message")
        command, player_name = message.split()
        if player_name in team_a:
            team_a.remove(player_name)
        elif player_name in team_b:
            team_b.remove(player_name)
        else: continue
        result.append(place_player(player_name))
    return result

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print __doc__
        exit(1)
    command, team_a, team_b = sys.argv
    player_messages = wait_for_players(team_a.split(','), team_b.split(','))
    go(player_messages)
