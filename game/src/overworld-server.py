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
        if line[0] == '/login':
            playerlist.append(line[1])
        elif line[0] == '/logout':
            playerlist.remove(line[1])
        elif line[0] == '/list':
            sys.stdout.write("/players " + " ".join(playerlist) + "\n")
        elif line[0] == '/play':
            firstname = line[1]
            names = line[2:]
            pid = os.fork()
            if pid == 0:
                os.system("../../switchbox/src/switchbox %d"%game_port)
                exit
            time.sleep(0.1) # just in case
            os.system(
                "./sixty-nine './ref.py %s annoations.txt' './switchbox-connect localhost %d'"%(
                    ' '.join(names),
                    game_port))
        sys.stdout.flush()

if __name__ == '__main__':
    if len(sys.argv) != 1:
        print __doc__
        exit(1)
    playerUpdate()
