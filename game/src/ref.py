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
        line = sys.stdin.readline()
        message = line.split()
        if len(message) != 3 or message[1] != '/identify':
            raise Exception("bad /identify message")
        who, command, player = message
        if player in players:
            players.remove(player)
            result.append(place_player(player))
            playerDict[who] = player
            sys.stderr.write(line)
            sys.stderr.flush()
    return result

        
def handle_line(line_text, logfile):
    line = line_text.split()
    if not len(line): raise Exception("No from field in message.  ")
    if len(line) == 1: raise Exception("Missing timestamp.  ")
    if len(line) == 2: return # Null Message
    (from_, timestamp, command), args = line[0:3], line[3:]
    if command == "/annotate":
        annotations.append(line_text)
        annotation_file.write(line_text)
        sys.stderr.write(line_text)
        sys.stderr.flush()
    elif command == "/gameover":
        exit(0)
    elif from_ == "-1":
        print '0.0 /gameover' # Hack 0.0 isn't the current time.
        sys.stdout.flush()


if __name__ == '__main__':
    playerDict = {}
    sys.stderr.write(" ".join(sys.argv) + '\n')
    if len(sys.argv) == 1: raise Exception("No logfile given.  ")
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
        if not line: break
        handle_line(line, annotation_file)
    annotation_file.close()
