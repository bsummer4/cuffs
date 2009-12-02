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

import sys, os, random, time

def randPos(): return random.randint(0, 800)

def place_player(player):
    return '/new player-%s player %d %d'%(player, randPos(), 80)

def go(player_placements):
    sys.stdout.write('/map big' + "\n")
    sys.stdout.write('/start' + "\n")
    remain = len(player_placements)
    for placement_message in player_placements:
        remain -= 1
        sys.stdout.write("0.%d %s\n"%(remain, placement_message))

def wait_for_players(team_a, team_b):
    result = []
    while True:
        if len(team_a) + len(team_b) == 0: break
        message = sys.stdin.readline()
        message_ = message.split()
        if message_[1] != '/identify':
            raise Exception("bad /identify message")
        who, command, player_name = message_
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

    # TODO: We will need to send messages to keep the syncronizer
    # running.  This hack sorta works for now
    sys.stdout.flush()
    time.sleep(1)
    sys.stdout.write("100000.0 hihihi\n")
    sys.stdout.flush()
    while True:
        sys.stderr.write(sys.stdin.readline())
        sys.stderr.flush()
