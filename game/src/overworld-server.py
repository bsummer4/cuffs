#!/usr/bin/python

"""
Usage: overServer.py
Maintains a list of players currently on the switchbox
interfaces with the rest of the game using stdin and stdout
"""

import sys, os, random, time

playerlist = []
game_port = 5151

def playerUpdate():
    while True:
        line_text = sys.stdin.readline()
        if not line_text: return # EOF
        line = line_text.split()
        origin = line[0]
        line = line[1:]
        if line[0] == '/login':
            playerlist.append(line[1])
        elif line[0] == '/logout':
            playerlist.remove(line[1])
        elif line[0] == '/list':
            sys.stdout.write("/players " + " ".join(playerlist) + "\n")
        elif line[0] == '/play':
            names = line[1:]
            pid = os.fork()
            if pid == 0:
                os.execlp("../../switchbox/src/switchbox", "../../switchbox/src/switchbox",  str(game_port))
                exit
            time.sleep(0.1) # just in case
            os.execlp("./sixty-nine", "./sixty-nine", "./ref.py %s annoations.txt"%(' '.join(names)),
                                                      "./switchbox-connect localhost %d"%(game_port))
        sys.stdout.flush()

if __name__ == '__main__':
    if len(sys.argv) != 1:
        print __doc__
        exit(1)
    playerUpdate()
