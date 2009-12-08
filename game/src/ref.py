#!/usr/bin/python

"""
# Usage

./ref team-A-names team-B-players

example: ./ref player1 [...] annotation_filename

The referee will:
  * Wait for all clients to "/identify $name" themselves.
  * Starts the game with '/start'
  * Place all the player objects in their initial locations:
      ("0 /new player-$name player $x $y")
  * Record all /annotate messages
"""

import sys, os, random, time

def randX(): return random.randint(0, 800)
def randY(): return random.randint(40, 120)

def place_player(player):
    return '/new player-%s player %d %d'%(player, randX(), randY())

def go(player_placements):
    sys.stdout.write('/map big' + "\n")
    startstr = '/start '
    for i,n in playerDict.iteritems():
        startstr += str(i) + ":" + str(n) + " "
    startstr += "\n"
    sys.stdout.write(startstr)
    remain = len(player_placements)
    for placement_message in player_placements:
        remain -= 1
        sys.stdout.write("0.%d %s\n"%(remain, placement_message))

def wait_for_players(players_, playerDict):
    # TODO Add a timeout to wait for people to join
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
            playerDict[who] = player
            sys.stderr.write(message + '\n')
            sys.stderr.flush()
    return result

if __name__ == '__main__':
    playerDict = {}
    sys.stderr.write(" ".join(sys.argv))
    go(wait_for_players(sys.argv[1:len(sys.argv)-1],playerDict))
    annotation_file = open(sys.argv[-1], "w+")


    # TODO: We will need to send messages to keep the syncronizer
    # running.  This hack sorta works for now
    sys.stdout.flush()
    time.sleep(1)
    sys.stdout.write("100000.0 hihihi\n")
    sys.stderr.write("100000.0 hihihi\n")
    sys.stdout.flush()
    sys.stderr.flush()
    annotations = []
    while True:
        line = sys.stdin.readline()
        line_ = line.split()
        if (len(line_) > 2 and line_[2] == "/annotate"):
            annotations.append(line)
            annotation_file.write(line)
            sys.stderr.write(line)
            sys.stderr.flush()
        if (len(line_) == 3 and line_[2] == "/gameover"):
            exit(0)
        # TODO Handle disconnects
    close(annotation_file)
