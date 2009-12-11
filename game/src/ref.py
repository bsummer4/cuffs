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
import map_parse

def randX(): return random.randint(0, 800)
def randY(): return random.randint(40, 120)

def log(obj):
    sys.stderr.write(str(obj))
    sys.stderr.flush()
    return obj

def change_gravity_wind(gamemap):
    gravity, wind = random.choice(list(gamemap.gravity_wind))
    return log(("/set global wind %s" % wind,
                "/set global gravity %s" % gravity))

def place_player(player, gamemap):
    point = random.choice(list(gamemap.spawnpoints))
    return log('/new player-%s player %d %d'%(player, point[0], point[1]))

def go(address_player, gamemap):
    print '/map %s' % gamemap.mapname
    print ("/start %s" % " ".join(["%s:%s"%(i, n)
                                   for i, n in address_player.iteritems()]))
    messages = ([place_player(player, gamemap)
                 for player in address_player.values()]
                + list(change_gravity_wind(gamemap)))
    remain = len(messages)
    for message in messages:
        remain -= 1
        sys.stdout.write("0.%d %s\n"%(remain, message))

# Return a dict of addresses -> playernames
def wait_for_players(players):
    # TODO Add a timeout to wait for people to join
    address_player = {}
    while True:
        if len(players) == len(address_player): break
        line_text = sys.stdin.readline()
        line = line_text.split()
        if len(line) <= 1: continue
        if len(line) != 3 or line[1] != '/identify':
            raise Exception("bad /identify line")
        address, command, player = line
        if player in players:
            address_player[address] = player
    return address_player


count = 1
def handle_line(line_text, logfile, gamemap):
    global count
    count += 1
    if not count % 1000:
        for line in change_gravity_wind(gamemap):
            print "0.0 %s" % line
        sys.stdout.flush()
    line = line_text.split()
    if len(line) <= 3: return
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
    sys.stderr.write(" ".join(sys.argv) + '\n')
    if len(sys.argv) == 1: raise Exception("No logfile given.  ")
    players, annotation_filename = sys.argv[1:-1], sys.argv[-1]

    os.chdir("../data")
    maps = map_parse.Maps("maps")
    if not len(maps.maps): raise Exception("No map files!  ")
    gamemap = random.choice(maps.maps.values())

    go(wait_for_players(players), gamemap)
    annotation_file = open(annotation_filename, "w+")

    # TODO: We will need to send messages to keep the syncronizer
    # running.  This large number hack works fine for now.
    sys.stdout.flush()
    time.sleep(1) # TODO Why sleep here?
    sys.stdout.write("1000000.0 hihihi\n")
    sys.stdout.flush()
    annotations = []
    while True:
        line = sys.stdin.readline()
        if not line: break
        handle_line(line, annotation_file, gamemap)
    annotation_file.close()
