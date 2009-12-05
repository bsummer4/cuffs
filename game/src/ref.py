#!/usr/bin/python

"""
# Usage

./ref team-A-names team-B-players

example: ./ref player1 [...]

The referee will:
  * Wait for all clients to "/identify $name" themselves.
  * Starts the game with '/start'
  * Place all the player objects in their initial locations:
      ("0 /new player-$name player $x $y")
"""

import sys, os, random, time

def randX(): return random.randint(0, 800)
def randY(): return random.randint(40, 120)

def place_player(player):
    return '/new player-%s player %d %d'%(player, randX(), randY())

def go(player_placements):
    sys.stdout.write('/map big' + "\n")
    sys.stdout.write('/start' + "\n")
    remain = len(player_placements)
    for placement_message in player_placements:
        remain -= 1
        sys.stdout.write("0.%d %s\n"%(remain, placement_message))

def wait_for_players(players_):
    players = list(players_) # Copy because we modify it
    result = []
    while True:
        if not players: break
        message = sys.stdin.readline().split()
        if len(message) != 3 or message[1] != '/identify':
            raise Exception("bad /identify message")
        who, command, player = message
        if player in players:
            players.remove(player)
            result.append(place_player(player))
    return result

if __name__ == '__main__':
    sys.stderr.write(" ".join(sys.argv))
    go(wait_for_players(sys.argv[1:]))

    # TODO: We will need to send messages to keep the syncronizer
    # running.  This hack sorta works for now
    sys.stdout.flush()
    time.sleep(1)
    sys.stdout.write("100000.0 hihihi\n")
    sys.stdout.flush()
    while True:
        sys.stderr.write(sys.stdin.readline())
        sys.stderr.flush()
